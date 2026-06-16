/* sfo.c - SFO file utility (C implementation of sfo.py)
 *
 * Supports the same options as sfo.py:
 *   -h / --help
 *   -v / --version
 *   -d / --debug
 *   -p / --pretty
 *   -l / --list <sfofile>
 *   -t / --toxml   <sfofile> <xmlfile>
 *   -f / --fromxml <xmlfile> <sfofile>
 *   --title=<title>  (override TITLE when using --fromxml)
 *   --appid=<appid>  (override TITLE_ID when using --fromxml)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <getopt.h>

#define PS3SFO_APP_VERSION "0.3"

/* ------------------------------------------------------------------ */
/* SFO constants                                                        */
/* ------------------------------------------------------------------ */
#define SFO_MAGIC   0x46535000U
#define SFO_STRING  2
#define SFO_INT     4

#define SFO_HDR_SIZE   20
#define SFO_ENTRY_SIZE 16

/* ------------------------------------------------------------------ */
/* Little-endian helpers                                               */
/* ------------------------------------------------------------------ */
static uint16_t rd_le16(const uint8_t *p)
{
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

static uint32_t rd_le32(const uint8_t *p)
{
    return (uint32_t)p[0]       | ((uint32_t)p[1] << 8) |
           ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

static void wr_le16(uint8_t *p, uint16_t v)
{
    p[0] = v & 0xff;
    p[1] = (uint8_t)(v >> 8);
}

static void wr_le32(uint8_t *p, uint32_t v)
{
    p[0] = v & 0xff;
    p[1] = (v >> 8)  & 0xff;
    p[2] = (v >> 16) & 0xff;
    p[3] = (v >> 24) & 0xff;
}

static uint32_t align_val(uint32_t n, uint32_t a)
{
    return (n + a - 1) & ~(a - 1);
}

/* ------------------------------------------------------------------ */
/* Key-value pair                                                       */
/* ------------------------------------------------------------------ */
#define KV_MAX_KEY   128
#define KV_MAX_STR  2048
#define KV_MAX_PAIRS 256

typedef struct {
    char     key[KV_MAX_KEY];
    int      type;        /* SFO_STRING or SFO_INT */
    uint32_t int_val;
    char     str_val[KV_MAX_STR];
    /* raw entry fields (for debug output) */
    uint16_t key_off;
    uint8_t  unk1;
    uint32_t value_len;
    uint32_t padded_len;
    uint32_t value_off;
} sfo_kv_t;

/* ------------------------------------------------------------------ */
/* File I/O helpers                                                    */
/* ------------------------------------------------------------------ */
static uint8_t *read_file(const char *path, size_t *out_len)
{
    FILE *fp = fopen(path, "rb");
    if (!fp) { perror(path); return NULL; }
    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (len < 0) { fclose(fp); return NULL; }
    uint8_t *buf = (uint8_t *)malloc((size_t)len + 1);
    if (!buf) { fclose(fp); return NULL; }
    if (fread(buf, 1, (size_t)len, fp) != (size_t)len) {
        free(buf); fclose(fp); return NULL;
    }
    buf[len] = '\0';
    fclose(fp);
    *out_len = (size_t)len;
    return buf;
}

/* ------------------------------------------------------------------ */
/* Parse SFO binary into kv array                                      */
/* ------------------------------------------------------------------ */
static int parse_sfo(const uint8_t *data, size_t data_len,
                     sfo_kv_t *kvs, int *out_count,
                     uint32_t *out_key_offset, uint32_t *out_value_offset)
{
    if (data_len < SFO_HDR_SIZE) {
        fprintf(stderr, "SFO: file too small\n");
        return -1;
    }

    uint32_t magic       = rd_le32(data);
    uint32_t unk1        = rd_le32(data + 4);
    uint32_t key_offset  = rd_le32(data + 8);
    uint32_t value_offset = rd_le32(data + 12);
    uint32_t pair_count  = rd_le32(data + 16);

    if (magic != SFO_MAGIC) {
        fprintf(stderr, "SFO: bad magic %08x\n", magic);
        return -1;
    }
    if (unk1 != 0x00000101) {
        fprintf(stderr, "SFO: unexpected unk1 %08x\n", unk1);
        return -1;
    }
    if (pair_count > KV_MAX_PAIRS) {
        fprintf(stderr, "SFO: too many pairs (%u)\n", pair_count);
        return -1;
    }

    if (out_key_offset)   *out_key_offset   = key_offset;
    if (out_value_offset) *out_value_offset = value_offset;

    size_t entry_off = SFO_HDR_SIZE;
    for (uint32_t i = 0; i < pair_count; i++) {
        if (entry_off + SFO_ENTRY_SIZE > data_len) {
            fprintf(stderr, "SFO: truncated entry %u\n", i);
            return -1;
        }
        uint16_t k_off   = rd_le16(data + entry_off);
        uint8_t  e_unk1  = data[entry_off + 2];
        uint8_t  vtype   = data[entry_off + 3];
        uint32_t vlen    = rd_le32(data + entry_off + 4);
        uint32_t padded  = rd_le32(data + entry_off + 8);
        uint32_t v_off   = rd_le32(data + entry_off + 12);

        /* Read key (null-terminated) */
        if (key_offset + k_off >= data_len) { return -1; }
        const uint8_t *kp = data + key_offset + k_off;
        size_t klen = 0;
        while (kp + klen < data + data_len && kp[klen] != '\0') klen++;
        if (klen >= KV_MAX_KEY) klen = KV_MAX_KEY - 1;
        memcpy(kvs[i].key, kp, klen);
        kvs[i].key[klen] = '\0';

        kvs[i].type      = vtype;
        kvs[i].key_off   = k_off;
        kvs[i].unk1      = e_unk1;
        kvs[i].value_len = vlen;
        kvs[i].padded_len = padded;
        kvs[i].value_off = v_off;

        if (vtype == SFO_STRING) {
            if (value_offset + v_off >= data_len) { return -1; }
            const uint8_t *vp = data + value_offset + v_off;
            size_t vlen2 = 0;
            while (vp + vlen2 < data + data_len && vp[vlen2] != '\0') vlen2++;
            if (vlen2 >= KV_MAX_STR) vlen2 = KV_MAX_STR - 1;
            memcpy(kvs[i].str_val, vp, vlen2);
            kvs[i].str_val[vlen2] = '\0';
            kvs[i].int_val = 0;
        } else if (vtype == SFO_INT) {
            if (value_offset + v_off + 4 > data_len) { return -1; }
            kvs[i].int_val = rd_le32(data + value_offset + v_off);
            kvs[i].str_val[0] = '\0';
        } else {
            kvs[i].int_val = 0;
            kvs[i].str_val[0] = '\0';
        }

        entry_off += SFO_ENTRY_SIZE;
    }
    *out_count = (int)pair_count;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Print dict representation (matches Python's print(stuff))          */
/* ------------------------------------------------------------------ */
static void print_repr_str(const char *s)
{
    /* Output Python single-quoted string repr */
    putchar('\'');
    for (; *s; s++) {
        unsigned char c = (unsigned char)*s;
        if (c == '\\')       fputs("\\\\", stdout);
        else if (c == '\'')  fputs("\\'", stdout);
        else if (c == '\n')  fputs("\\n", stdout);
        else if (c == '\r')  fputs("\\r", stdout);
        else if (c == '\t')  fputs("\\t", stdout);
        else                 putchar(c);
    }
    putchar('\'');
}

static void print_dict(const sfo_kv_t *kvs, int count)
{
    putchar('{');
    for (int i = 0; i < count; i++) {
        if (i > 0) fputs(", ", stdout);
        print_repr_str(kvs[i].key);
        fputs(": ", stdout);
        if (kvs[i].type == SFO_STRING)
            print_repr_str(kvs[i].str_val);
        else
            printf("%u", kvs[i].int_val);
    }
    puts("}");
}

/* ------------------------------------------------------------------ */
/* list_sfo: -l option                                                 */
/* ------------------------------------------------------------------ */
static void list_sfo(const char *file, int debug, int pretty)
{
    size_t data_len;
    uint8_t *data = read_file(file, &data_len);
    if (!data) return;

    if (data_len < SFO_HDR_SIZE) {
        fprintf(stderr, "SFO: file too small\n");
        free(data);
        return;
    }

    uint32_t magic        = rd_le32(data);
    uint32_t unk1         = rd_le32(data + 4);
    uint32_t key_offset   = rd_le32(data + 8);
    uint32_t value_offset = rd_le32(data + 12);
    uint32_t pair_count   = rd_le32(data + 16);

    if (debug) {
        printf("[X] Magic: %08x\n", magic);
        printf("[ ] Unk1: %08x\n", unk1);
        printf("[X] Key Offset: %08x\n", key_offset);
        printf("[X] Value Offset: %08x\n", value_offset);
        printf("[X] Pair Count: %08x\n\n", pair_count);
    }

    if (magic != SFO_MAGIC || unk1 != 0x00000101) {
        fprintf(stderr, "SFO: invalid header\n");
        free(data);
        return;
    }
    if (pair_count > KV_MAX_PAIRS) {
        fprintf(stderr, "SFO: too many pairs\n");
        free(data);
        return;
    }

    sfo_kv_t kvs[KV_MAX_PAIRS];
    int count;
    if (parse_sfo(data, data_len, kvs, &count, NULL, NULL) != 0) {
        free(data);
        return;
    }

    if (debug) {
        for (int i = 0; i < count; i++) {
            if (!pretty) {
                printf("[X] Key Offset: %04x\n", kvs[i].key_off);
                printf("[ ] Unk1: %02x\n", kvs[i].unk1);
                printf("[/] Value Type: %02x\n", kvs[i].type);
                printf("[X] Value Length: %08x\n", kvs[i].value_len);
                printf("[X] Padded Length: %08x\n", kvs[i].padded_len);
                printf("[X] Value Offset: %08x\n\n", kvs[i].value_off);
            } else {
                /* Pretty print */
                printf("[X] Key: '%s'[%04x]\n", kvs[i].key, kvs[i].key_off);
                printf("[/] Unk: %02x\n", kvs[i].unk1);
                printf("[/] Value Type: %02x\n", kvs[i].type);
                printf("[X] Value Length: %08x\n", kvs[i].value_len);
                printf("[X] Padded Length: %08x\n", kvs[i].padded_len);
                if (kvs[i].type == SFO_STRING) {
                    printf("[X] Value Offset: %08x\n[X] Value: '%s'[%08x]\n\n",
                           kvs[i].value_off,
                           kvs[i].str_val,
                           value_offset + kvs[i].value_off);
                } else if (kvs[i].type == SFO_INT) {
                    printf("[X] Value Offset: %08x\n[X] Value: %u[%08x]\n\n",
                           kvs[i].value_off,
                           kvs[i].int_val,
                           value_offset + kvs[i].value_off);
                } else {
                    printf("[X] Value Offset: %08x\n[X] Value Type Unknown\n\n",
                           kvs[i].value_off);
                }
            }
        }
    } else {
        print_dict(kvs, count);
    }

    free(data);
}

/* ------------------------------------------------------------------ */
/* convert_to_xml: --toxml option                                      */
/* ------------------------------------------------------------------ */
static void convert_to_xml(const char *sfofile, const char *xmlfile, int debug)
{
    size_t data_len;
    uint8_t *data = read_file(sfofile, &data_len);
    if (!data) return;

    sfo_kv_t kvs[KV_MAX_PAIRS];
    int count;
    if (parse_sfo(data, data_len, kvs, &count, NULL, NULL) != 0) {
        free(data);
        return;
    }

    if (!debug)
        print_dict(kvs, count);

    free(data);

    FILE *fp = fopen(xmlfile, "w");
    if (!fp) { perror(xmlfile); return; }

    fputs("<?xml version=\"1.0\" ?>\n", fp);
    fputs("<sfo>\n", fp);
    for (int i = 0; i < count; i++) {
        if (kvs[i].type == SFO_STRING) {
            fprintf(fp, "\t<value name=\"%s\" type=\"string\">%s</value>\n",
                    kvs[i].key, kvs[i].str_val);
        } else if (kvs[i].type == SFO_INT) {
            fprintf(fp, "\t<value name=\"%s\" type=\"integer\">%u</value>\n",
                    kvs[i].key, kvs[i].int_val);
        }
    }
    fputs("</sfo>\n", fp);
    fclose(fp);
}

/* ------------------------------------------------------------------ */
/* Minimal XML parser for SFO XML format                               */
/* ------------------------------------------------------------------ */

/* Skip whitespace */
static const char *skip_ws(const char *p)
{
    while (*p && isspace((unsigned char)*p)) p++;
    return p;
}

/* Strip leading and trailing whitespace (in-place on a copy) */
static void strip_ws(char *s)
{
    /* Trim trailing */
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1]))
        s[--len] = '\0';
    /* Trim leading */
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s)
        memmove(s, start, strlen(start) + 1);
}

/* Extract a quoted attribute value.
 * Searches for attr="..." or attr='...' in the string starting at src.
 * Copies result into buf (at most bufsize-1 chars + NUL).
 * Returns pointer past the closing quote, or NULL if not found. */
static const char *extract_attr(const char *src, const char *attrname,
                                  char *buf, size_t bufsize)
{
    size_t namelen = strlen(attrname);
    for (;;) {
        const char *p = strstr(src, attrname);
        if (!p) return NULL;
        /* Check there's an '=' (possibly with spaces) after the name */
        const char *q = p + namelen;
        q = skip_ws(q);
        if (*q != '=') { src = p + 1; continue; }
        q++;
        q = skip_ws(q);
        char quote = *q;
        if (quote != '"' && quote != '\'') { src = p + 1; continue; }
        q++;
        const char *end = strchr(q, quote);
        if (!end) return NULL;
        size_t len = (size_t)(end - q);
        if (len >= bufsize) len = bufsize - 1;
        memcpy(buf, q, len);
        buf[len] = '\0';
        return end + 1;
    }
}

/* Parse <value name="..." type="...">content</value> elements */
static int parse_xml(const char *xmldata,
                     sfo_kv_t *kvs, int *out_count,
                     const char *forcetitle, const char *forceappid)
{
    int count = 0;
    const char *p = xmldata;

    while (*p && count < KV_MAX_PAIRS) {
        /* Find next <value */
        const char *tag = strstr(p, "<value");
        if (!tag) break;

        /* Check this is actually the start of a tag (not e.g. </value>) */
        if (tag[6] != ' ' && tag[6] != '\t' && tag[6] != '\n' &&
            tag[6] != '\r' && tag[6] != '>') {
            p = tag + 1;
            continue;
        }

        /* Find the '>' that closes this tag */
        const char *tag_end = strchr(tag, '>');
        if (!tag_end) break;

        /* Extract name and type attributes from tag content */
        char name[KV_MAX_KEY]  = {0};
        char type_s[32]        = {0};

        /* Make a temporary copy of the tag so extract_attr works correctly */
        size_t tag_inner_len = (size_t)(tag_end - tag);
        char *tag_buf = (char *)malloc(tag_inner_len + 1);
        if (!tag_buf) break;
        memcpy(tag_buf, tag, tag_inner_len);
        tag_buf[tag_inner_len] = '\0';

        int has_name = (extract_attr(tag_buf, "name", name, sizeof(name)) != NULL);
        int has_type = (extract_attr(tag_buf, "type", type_s, sizeof(type_s)) != NULL);
        free(tag_buf);

        if (!has_name || !has_type) {
            p = tag_end + 1;
            continue;
        }

        /* Extract content between > and </value> */
        const char *content_start = tag_end + 1;
        const char *close_tag = strstr(content_start, "</value>");
        if (!close_tag) break;

        size_t clen = (size_t)(close_tag - content_start);
        char *content = (char *)malloc(clen + 1);
        if (!content) break;
        memcpy(content, content_start, clen);
        content[clen] = '\0';
        strip_ws(content);

        /* Apply forcetitle / forceappid overrides */
        if (forcetitle && strcmp(name, "TITLE") == 0) {
            free(content);
            content = (char *)malloc(strlen(forcetitle) + 1);
            if (!content) break;
            strcpy(content, forcetitle);
        } else if (forceappid && strcmp(name, "TITLE_ID") == 0) {
            free(content);
            content = (char *)malloc(strlen(forceappid) + 1);
            if (!content) break;
            strcpy(content, forceappid);
        }

        /* Fill kv entry */
        strncpy(kvs[count].key, name, KV_MAX_KEY - 1);
        kvs[count].key[KV_MAX_KEY - 1] = '\0';

        if (strcmp(type_s, "integer") == 0) {
            kvs[count].type    = SFO_INT;
            kvs[count].int_val = (uint32_t)strtoul(content, NULL, 10);
            kvs[count].str_val[0] = '\0';
        } else {
            kvs[count].type = SFO_STRING;
            strncpy(kvs[count].str_val, content, KV_MAX_STR - 1);
            kvs[count].str_val[KV_MAX_STR - 1] = '\0';
            kvs[count].int_val = 0;
        }

        free(content);
        count++;
        p = close_tag + 8; /* skip "</value>" */
    }

    *out_count = count;
    return 0;
}

/* ------------------------------------------------------------------ */
/* convert_from_xml: --fromxml option                                  */
/* ------------------------------------------------------------------ */
static void convert_from_xml(const char *xmlfile, const char *sfofile,
                               const char *forcetitle, const char *forceappid)
{
    size_t xml_len;
    uint8_t *xmldata = read_file(xmlfile, &xml_len);
    if (!xmldata) return;

    sfo_kv_t kvs[KV_MAX_PAIRS];
    int count = 0;
    if (parse_xml((const char *)xmldata, kvs, &count, forcetitle, forceappid) != 0) {
        free(xmldata);
        return;
    }
    free(xmldata);

    /* Compute key and value offsets/sizes */
    uint32_t keyoff   = 0;
    uint32_t valueoff = 0;

    /* First pass: compute padded_len and offsets */
    uint32_t *key_offs   = (uint32_t *)calloc(count, sizeof(uint32_t));
    uint32_t *val_offs   = (uint32_t *)calloc(count, sizeof(uint32_t));
    uint32_t *pad_lens   = (uint32_t *)calloc(count, sizeof(uint32_t));
    uint32_t *val_lens   = (uint32_t *)calloc(count, sizeof(uint32_t));
    if (!key_offs || !val_offs || !pad_lens || !val_lens) {
        free(key_offs); free(val_offs); free(pad_lens); free(val_lens);
        return;
    }

    for (int i = 0; i < count; i++) {
        key_offs[i] = keyoff;
        keyoff += (uint32_t)strlen(kvs[i].key) + 1;

        val_offs[i] = valueoff;
        if (kvs[i].type == SFO_INT) {
            pad_lens[i] = 4;
            val_lens[i] = 4;
        } else {
            uint32_t slen = (uint32_t)strlen(kvs[i].str_val) + 1;
            uint32_t alignment = 4;
            if (strcmp(kvs[i].key, "TITLE") == 0)
                alignment = 0x80;
            else if (strcmp(kvs[i].key, "LICENSE") == 0)
                alignment = 0x200;
            else if (strcmp(kvs[i].key, "TITLE_ID") == 0)
                alignment = 0x10;
            val_lens[i]  = slen;
            pad_lens[i]  = align_val(slen, alignment);
        }
        valueoff += pad_lens[i];
    }

    /* Header layout */
    uint32_t header_key_offset   = SFO_HDR_SIZE + SFO_ENTRY_SIZE * (uint32_t)count;
    uint32_t header_value_offset = align_val(header_key_offset + keyoff, 4);
    uint32_t keypad              = header_value_offset - (header_key_offset + keyoff);

    /* Write SFO */
    FILE *fp = fopen(sfofile, "wb");
    if (!fp) { perror(sfofile); goto cleanup; }

    /* Header */
    uint8_t hdr[SFO_HDR_SIZE];
    wr_le32(hdr,      SFO_MAGIC);
    wr_le32(hdr + 4,  0x00000101);
    wr_le32(hdr + 8,  header_key_offset);
    wr_le32(hdr + 12, header_value_offset);
    wr_le32(hdr + 16, (uint32_t)count);
    fwrite(hdr, 1, SFO_HDR_SIZE, fp);

    /* Entries */
    for (int i = 0; i < count; i++) {
        uint8_t entry[SFO_ENTRY_SIZE];
        wr_le16(entry,     (uint16_t)key_offs[i]);
        entry[2] = 4;  /* unk1 is always 4 */
        entry[3] = (uint8_t)kvs[i].type;
        wr_le32(entry + 4,  val_lens[i]);
        wr_le32(entry + 8,  pad_lens[i]);
        wr_le32(entry + 12, val_offs[i]);
        fwrite(entry, 1, SFO_ENTRY_SIZE, fp);
    }

    /* Key table */
    for (int i = 0; i < count; i++) {
        fputs(kvs[i].key, fp);
        fputc('\0', fp);
    }
    /* Key padding */
    for (uint32_t j = 0; j < keypad; j++)
        fputc('\0', fp);

    /* Value table */
    for (int i = 0; i < count; i++) {
        if (kvs[i].type == SFO_INT) {
            uint8_t vbuf[4];
            wr_le32(vbuf, kvs[i].int_val);
            fwrite(vbuf, 1, 4, fp);
        } else {
            size_t slen = strlen(kvs[i].str_val);
            fwrite(kvs[i].str_val, 1, slen, fp);
            fputc('\0', fp);
            /* Padding after string+NUL to reach padded_len */
            uint32_t written = (uint32_t)slen + 1;
            for (uint32_t j = written; j < pad_lens[i]; j++)
                fputc('\0', fp);
        }
    }

    fclose(fp);

cleanup:
    free(key_offs);
    free(val_offs);
    free(pad_lens);
    free(val_lens);
}

/* ------------------------------------------------------------------ */
/* Usage / version                                                     */
/* ------------------------------------------------------------------ */
static void usage(void)
{
    puts("sfo v" PS3SFO_APP_VERSION " - A tool to manage PlayStation 3 SFO files.\n");
    puts("usage:");
    puts("    sfo [options]\n");
    puts("     -h / --help \t\t\t\t display this help and exit.");
    puts("     -v / --version \t\t\t\t output version information and exit.");
    puts("     -d / --debug \t\t\t\t display extra information such as header and table data.");
    puts("     -p / --pretty \t\t\t\t when used with -l and -d, display the key name and value.");
    puts("     -l / --list <sfofile> \t\t\t list the contents of the provided SFO file.");
    puts("     -t / --toxml   <sfofile> <xmlfile>  convert a SFO file to an XML file.");
    puts("     -f / --fromxml <xmlfile> <sfofile>  convert an XML file to a SFO file.");
    puts("       --title=<title> \t\t\t\t (override TITLE when using --fromxml)");
    puts("       --appid=<appid> \t\t\t\t (override TITLE_ID when using --fromxml)\n");
}

static void version(void)
{
    puts("sfo " PS3SFO_APP_VERSION);
}

/* ------------------------------------------------------------------ */
/* main                                                                */
/* ------------------------------------------------------------------ */
int main(int argc, char *argv[])
{
    int debug       = 0;
    int pretty      = 0;
    int do_list     = 0;
    int do_toxml    = 0;
    int do_fromxml  = 0;
    char *filelist  = NULL;
    char *forcetitle = NULL;
    char *forceappid = NULL;

    static struct option long_opts[] = {
        {"help",    no_argument,       NULL, 'h'},
        {"debug",   no_argument,       NULL, 'd'},
        {"version", no_argument,       NULL, 'v'},
        {"pretty",  no_argument,       NULL, 'p'},
        {"list",    required_argument, NULL, 'l'},
        {"toxml",   no_argument,       NULL, 't'},
        {"fromxml", no_argument,       NULL, 'f'},
        {"title",   required_argument, NULL, 1  },
        {"appid",   required_argument, NULL, 2  },
        {NULL, 0, NULL, 0}
    };

    int opt;
    while ((opt = getopt_long(argc, argv, "hdvpl:tf", long_opts, NULL)) != -1) {
        switch (opt) {
        case 'h': usage();   return 0;
        case 'v': version(); return 0;
        case 'd': debug  = 1; break;
        case 'p': pretty = 1; break;
        case 'l': filelist = optarg; do_list = 1; break;
        case 't': do_toxml   = 1; break;
        case 'f': do_fromxml = 1; break;
        case 1:   forcetitle = optarg; break;
        case 2:   forceappid = optarg; break;
        default:  usage(); return 2;
        }
    }

    if (do_list) {
        list_sfo(filelist, debug, pretty);
    } else if (do_toxml && !do_fromxml && (argc - optind) == 2) {
        convert_to_xml(argv[optind], argv[optind + 1], debug);
    } else if (do_fromxml && !do_toxml && (argc - optind) == 2) {
        convert_from_xml(argv[optind], argv[optind + 1], forcetitle, forceappid);
    } else {
        usage();
        return 2;
    }

    return 0;
}
