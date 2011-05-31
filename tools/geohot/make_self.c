// make_self_npdrm by geohot
// part of geohot's awesome tools for the PS3
//  released under GPLv3, see http://gplv3.fsf.org/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gmp.h>
#include <time.h>

#include <openssl/sha.h>
#include <openssl/aes.h>
#include "zlib.h"

#include "include/ps3_common.h"
#include "include/elf.h"
#include "include/keys.h"
#include "include/oddkeys.h"
#include "include/sha1_hmac.h"

#include "include/self.h"

#include "include/aes_omac.h"

//#define NO_CRYPT
//#define NPDRM
//#define SPRX

#ifdef NPDRM
#define KEY(SUFFIX) npdrm_##SUFFIX
#else
#define KEY(SUFFIX) appold_##SUFFIX
#endif

u8 nubpadding_static[] = {
  0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
#ifdef SPRX
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7B,0x00,0x00,0x00,0x01,0x00,0x02,0x00,0x00
#else
#ifdef NPDRM
// this broke lv2diag.self
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3B,0x00,0x00,0x00,0x01,0x00,0x00,0x20,0x00
#else
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3B,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00
#endif
#endif
};
  // 0x1B in retail
  // 0x3B in lv2diag

u8 cflags_static[] = {
  0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
  0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,
#ifndef NPDRM
  0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x62,0x7C,0xB1,0x80,0x8A,0xB9,0x38,0xE3,0x2C,0x8C,0x09,0x17,0x08,0x72,0x6A,0x57,
  0x9E,0x25,0x86,0xE4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
#else
  0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x40,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,
  0x62,0x7C,0xB1,0x80,0x8A,0xB9,0x38,0xE3,0x2C,0x8C,0x09,0x17,0x08,0x72,0x6A,0x57,
  0x9E,0x25,0x86,0xE4,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5D,0xC0,
#endif
};

u8 sdkversion_static[] = {
  0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00
  //0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x00
};


AES_KEY aes_key;

u8* input_elf_data;

#define ZLIB_LEVEL 6
#define DEFLATION_BUFFER_SIZE 0x1000000
u8 def_buffer[DEFLATION_BUFFER_SIZE];

int def(u8 *source, int source_size, u8 *dest, int* dest_size) {
  int ret;
  unsigned have;
  z_stream strm;

  /* allocate inflate state */
  strm.zalloc = Z_NULL;
  strm.zfree = Z_NULL;
  strm.opaque = Z_NULL;
  strm.avail_in = source_size;
  strm.next_in = source;
  strm.avail_out = *dest_size;
  strm.next_out = dest;
  ret = deflateInit(&strm, ZLIB_LEVEL);
  if(ret != Z_OK)
    return ret;

  ret = deflate(&strm, Z_FINISH);
  (*dest_size) -= strm.avail_out;

  (void)deflateEnd(&strm);
  return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

void init_Self_Shdr(Self_Shdr* hdr) {
  set_u32(&(hdr->s_magic), 0x53434500);
  set_u32(&(hdr->s_hdrversion), 2);
#ifdef SPRX
  // on 3.41
  //set_u16(&(hdr->s_flags), 4);
  // on 3.55
  set_u16(&(hdr->s_flags), 7);
#else
  set_u16(&(hdr->s_flags), 1);
#endif
  set_u16(&(hdr->s_hdrtype), 1);
}

void init_Self_Ihdr(Self_Ihdr* hdr) {
#ifdef NPDRM
  set_u64(&(hdr->i_authid), 0x1010000001000003LL);
  set_u32(&(hdr->i_apptype), 8);
#else
#ifdef SPRX
  set_u64(&(hdr->i_authid), 0x1070000052000001LL);
#else
  //set_u64(&(hdr->i_authid), 0x10700003FD000001LL);
  set_u64(&(hdr->i_authid), 0x10700003FF000001LL);
#endif
  set_u32(&(hdr->i_apptype), 4);
#endif
  //set_u64(&(hdr->i_authid), 0x1070000500000001LL);

  set_u32(&(hdr->i_magic), 0x01000002);
  set_u64(&(hdr->i_version), 0x0003005500000000LL);
  //set_u64(&(hdr->i_version), 0x0003004000000000LL);
  //set_u64(&(hdr->i_version), 0x0003000000000000LL);
  //set_u64(&(hdr->i_version), 0x0001004000001000LL);
  //set_u64(&(hdr->i_version), 0x0001000000000000LL);
}

void init_Self_Ehdr(Self_Ehdr* hdr) {
  set_u64(&(hdr->e_magic), 3);
#ifdef NPDRM
  set_u64(&(hdr->e_cfsize), sizeof(Self_Cflags)+sizeof(Self_NPDRM));
#else
  set_u64(&(hdr->e_cfsize), sizeof(Self_Cflags));
#endif
}

int input_elf_len;

void read_elf_file(char* filename) {
  FILE *input_elf_file = fopen(filename, "rb");
  fseek(input_elf_file, 0, SEEK_END);
  input_elf_len = ftell(input_elf_file);
  fseek(input_elf_file, 0, SEEK_SET);
  input_elf_data = (u8*)malloc(input_elf_len);
  fread(input_elf_data, 1, input_elf_len, input_elf_file);
  fclose(input_elf_file);
}

gmp_randstate_t r_state;

Elf64_Ehdr* input_elf_header;
Self_Segment first_segment;

u8 zero_padding[0x10000];

void enumerate_segments() {
  int i,num;
  size_t countp;
  u8 ecount_buf[0x10],iv[0x10];
  Self_Segment* segment_ptr = &first_segment;
  Elf64_Phdr* elf_segment = (Elf64_Phdr*)(&input_elf_data[get_u64(&(input_elf_header->e_phoff))]);

  mpz_t riv, erk, hmac;
  mpz_init(riv); mpz_init(erk); mpz_init(hmac);

  for(i=0;i<get_u16(&(input_elf_header->e_phnum));i++) {
    
    memset(segment_ptr, 0, sizeof(Self_Segment));

// these are choices you can make
    /*segment_ptr->compressed = (i<2);
    segment_ptr->incrypt = (i<6); // **TESTING
    segment_ptr->encrypted = (i<5);*/

#ifdef NPDRM
    segment_ptr->encrypted = (i<5);
    segment_ptr->compressed = (i<4);
    segment_ptr->incrypt = (i<7);
#else
    segment_ptr->encrypted = 1;
    segment_ptr->compressed = 1;
    segment_ptr->incrypt = 1;
#endif
    
    set_u32(&(segment_ptr->enc_segment.segment_number), i);

    set_u32(&(segment_ptr->enc_segment.unknown2), 2);
    set_u32(&(segment_ptr->enc_segment.unknown3), 3);

    mpz_urandomb(hmac, r_state, 512);
    mpz_export(segment_ptr->crypt_segment.hmac, &countp, 1, 0x40, 1, 0, hmac);

    if(segment_ptr->encrypted) {
      mpz_urandomb(erk, r_state, 128);
      mpz_urandomb(riv, r_state, 128);
      mpz_export(segment_ptr->crypt_segment.erk, &countp, 1, 0x10, 1, 0, erk);
      mpz_export(segment_ptr->crypt_segment.riv, &countp, 1, 0x10, 1, 0, riv);
    }

    segment_ptr->rlen = get_u64(&(elf_segment->p_filesz));

    u32 in_data_offset = get_u64(&(elf_segment->p_offset));
    u8* in_data = &input_elf_data[in_data_offset];

    if(segment_ptr->compressed) {
      int def_size = DEFLATION_BUFFER_SIZE;
      printf("deflated...", def(in_data, segment_ptr->rlen, def_buffer, &def_size)); fflush(stdout);
      segment_ptr->len = def_size;
      segment_ptr->data = (u8*)malloc(segment_ptr->len);
      memcpy(segment_ptr->data, def_buffer, def_size);
    } else {
      segment_ptr->len = segment_ptr->rlen;
      segment_ptr->data = (u8*)malloc(segment_ptr->len);
      memcpy(segment_ptr->data, in_data, segment_ptr->len);
    }

    /*if(i==0) {
      segment_ptr->padding = 0x26A4;
    } else if(i==1) {
      segment_ptr->padding = 0xC;
    } else {
      segment_ptr->padding = 0;
    }*/

    segment_ptr->padding = (0x10-(segment_ptr->len&0xF))&0xF;

// hacks to make it match
    /*if(segment_ptr->len == 0x14BCC8) {
      segment_ptr->padding += 0x4330;
    }*/

    printf("processing segment %d with rlen %x len %x offset %x...", i, segment_ptr->rlen, segment_ptr->len, in_data_offset); fflush(stdout);

    //hexdump((u8*)elf_segment, sizeof(Elf64_Phdr));

    set_u64(&(segment_ptr->enc_segment.segment_size), segment_ptr->len);
    set_u32(&(segment_ptr->enc_segment.segment_crypt_flag), 1+segment_ptr->encrypted);
    set_u32(&(segment_ptr->enc_segment.segment_compressed_flag), 1+segment_ptr->compressed);

    set_u64(&(segment_ptr->pmhdr.pm_size), segment_ptr->len);
    set_u32(&(segment_ptr->pmhdr.pm_compressed), 1+segment_ptr->compressed);
    set_u32(&(segment_ptr->pmhdr.pm_encrypted), segment_ptr->encrypted);

// compute sha1
    SHA_CTX c;
    SHA1_ghetto_init(&c, segment_ptr->crypt_segment.hmac);
    SHA1_Update(&c, segment_ptr->data, segment_ptr->len);
    SHA1_ghetto_final(segment_ptr->crypt_segment.sha1, &c, segment_ptr->crypt_segment.hmac);

    if(segment_ptr->encrypted) {
      printf("encrypted...");  fflush(stdout);
      memset(ecount_buf, 0, 16); num=0;
      AES_set_encrypt_key(segment_ptr->crypt_segment.erk, 128, &aes_key);
      memcpy(iv, segment_ptr->crypt_segment.riv, 16);
#ifndef NO_CRYPT
      AES_ctr128_encrypt(segment_ptr->data, segment_ptr->data, segment_ptr->len, &aes_key, iv, ecount_buf, &num);
#endif
    }

    if(i != get_u16(&(input_elf_header->e_phnum))-1) {
      segment_ptr->next_segment = malloc(sizeof(Self_Segment));
    }
    elf_segment += 1;  // 1 is sizeof(Elf64_Phdr)
    segment_ptr = segment_ptr->next_segment;
    printf("\n");
  }
}


void init_Self_NPDRM(Self_NPDRM* npdrm, char* titleid, char* filename) {
  set_u32(&npdrm->block_type, 3);
  set_u32(&npdrm->block_size, sizeof(Self_NPDRM));
  set_u32(&npdrm->magic, 0x4E504400);
  set_u32(&npdrm->unknown3, 1);
  set_u32(&npdrm->unknown4, 3);
  set_u32(&npdrm->unknown5, 1);
  strncpy(npdrm->titleid, titleid, 0x30);

  char *true_filename = strrchr(filename,'/');
  if(true_filename == NULL) {
    true_filename = strrchr(filename,'\\');
  }
  if(true_filename == NULL) {
    true_filename = filename;
  } else {
    true_filename++;
  }
  
  u8 npdrm_omac_key[0x10]; int i;
  for(i=0;i<0x10;i++) npdrm_omac_key[i] = npdrm_omac_key1[i] ^ npdrm_omac_key2[i];

  int buf_len = 0x30+strlen(true_filename);
  char *buf = (char*)malloc(buf_len+1);
  memcpy(buf, npdrm->titleid, 0x30);
  strcpy(buf+0x30, true_filename);
  aesOmac1Mode(npdrm->hash1, buf, buf_len, npdrm_omac_key3, sizeof(npdrm_omac_key3)*8);
  free(buf);
  aesOmac1Mode(npdrm->hash2, (u8*)&(npdrm->magic), 0x60, npdrm_omac_key, sizeof(npdrm_omac_key)*8);
}

u8 segment_crypt_data[0x2000];
int segment_crypt_data_len = 0;
void build_segment_crypt_data() {
  Self_Segment* segment_ptr;
  segment_ptr = &first_segment;
  while(segment_ptr != NULL) {
    if(segment_ptr->incrypt) {
      set_u32(&(segment_ptr->enc_segment.segment_sha1_index), segment_crypt_data_len/0x10);
      memcpy(&segment_crypt_data[segment_crypt_data_len], segment_ptr->crypt_segment.sha1, sizeof(segment_ptr->crypt_segment.sha1)); segment_crypt_data_len += sizeof(segment_ptr->crypt_segment.sha1);
      memcpy(&segment_crypt_data[segment_crypt_data_len], segment_ptr->crypt_segment.hmac, sizeof(segment_ptr->crypt_segment.hmac)); segment_crypt_data_len += sizeof(segment_ptr->crypt_segment.hmac);

      if(segment_ptr->encrypted) {
        set_u32(&(segment_ptr->enc_segment.segment_erk_index), segment_crypt_data_len/0x10);
        memcpy(&segment_crypt_data[segment_crypt_data_len], segment_ptr->crypt_segment.erk, sizeof(segment_ptr->crypt_segment.erk)); segment_crypt_data_len += sizeof(segment_ptr->crypt_segment.erk);
        set_u32(&(segment_ptr->enc_segment.segment_riv_index), segment_crypt_data_len/0x10);
        memcpy(&segment_crypt_data[segment_crypt_data_len], segment_ptr->crypt_segment.riv, sizeof(segment_ptr->crypt_segment.riv)); segment_crypt_data_len += sizeof(segment_ptr->crypt_segment.riv);
      } else {
        set_u32(&(segment_ptr->enc_segment.segment_erk_index), 0xFFFFFFFF);
        set_u32(&(segment_ptr->enc_segment.segment_riv_index), 0xFFFFFFFF);
      }
    }
    segment_ptr = segment_ptr->next_segment;
  }
}

typedef struct  {
  void* data;
  int len;
  void* next;
} file_ll;

file_ll start_file;
file_ll *file_ll_ptr = &start_file;
int running_size;

void add_file_section(void* data, int len) {
  if((file_ll_ptr != &start_file) || (file_ll_ptr->len != 0)) {
    file_ll_ptr->next = (file_ll *)malloc(sizeof(file_ll));
    file_ll_ptr = file_ll_ptr->next;
    memset(file_ll_ptr, 0, sizeof(file_ll));
  }
  file_ll_ptr->data = data;
  file_ll_ptr->len = len;
  running_size += len;
}

u8* output_self_data;
void write_self_file_in_memory() {
  output_self_data = (u8*)malloc(running_size);
  file_ll_ptr = &start_file;
  u8* output_self_data_ptr = output_self_data;
  while(file_ll_ptr != NULL) {
    //printf("adding %X\n", file_ll_ptr->len);
    memcpy(output_self_data_ptr, file_ll_ptr->data, file_ll_ptr->len);
    output_self_data_ptr += file_ll_ptr->len;
    file_ll_ptr = file_ll_ptr->next;
  }
}

int main(int argc, char* argv[]) {
  int i;
  u8 ecount_buf[0x10], iv[0x10];
  size_t countp;
  int num;
  Self_Segment* segment_ptr;

  memset(zero_padding, 0, sizeof(zero_padding));

#ifdef NPDRM
  if(argc < 4) {
    printf("usage: %s input.elf output.self <content_id>\n", argv[0]);
    printf("  warning NPDRM cares about the output file name, do not rename\n");
    return 1;
  }
#else
  if(argc < 3) {
    printf("usage: %s input.elf output.self\n", argv[0]);
    return 1;
  }
#endif

// init randomness
  gmp_randinit_default(r_state);
  gmp_randseed_ui(r_state, time(NULL));

// read elf file
  read_elf_file(argv[1]);
  input_elf_header = (Elf64_Ehdr*)input_elf_data;

  printf("ELF header size @ %x\n", get_u16(&(input_elf_header->e_ehsize)) );
  printf("%d program headers @ %llx\n", get_u16(&(input_elf_header->e_phnum)), get_u64(&(input_elf_header->e_phoff)));
  printf("%d section headers @ %llx\n", get_u16(&(input_elf_header->e_shnum)), get_u64(&(input_elf_header->e_shoff)));

// loop through the segments
  enumerate_segments();
  printf("segments enumerated\n");

// setup self headers
  Self_Shdr output_self_header; memset(&output_self_header, 0, sizeof(output_self_header));
  Self_Ehdr output_extended_self_header; memset(&output_extended_self_header, 0, sizeof(output_extended_self_header));
  Self_Ihdr output_self_info_header; memset(&output_self_info_header, 0, sizeof(output_self_info_header));
  
  init_Self_Shdr(&output_self_header);
  init_Self_Ehdr(&output_extended_self_header);
  init_Self_Ihdr(&output_self_info_header);

  set_u64(&output_self_header.s_exsize, input_elf_len);

// setup segment header
  segment_certification_header segment_header; memset(&segment_header, 0, sizeof(segment_header));
  set_u32(&(segment_header.version), 1);

// NPDRM
#ifdef NPDRM
  Self_NPDRM npdrm; memset(&npdrm, 0, sizeof(npdrm));
  init_Self_NPDRM(&npdrm, argv[3], argv[2]);
#endif
// useless bullshit
  Self_SDKversion sdkversion;
  Self_Cflags cflags;
  memcpy(&sdkversion, sdkversion_static, sizeof(Self_SDKversion));
  memcpy(&cflags, cflags_static, sizeof(Self_Cflags));

// generate metadata encryption keys
  metadata_crypt_header md_header; memset(&md_header, 0, sizeof(md_header));
  memcpy(&md_header, KEY(keypair_d), sizeof(md_header));

// can't generate random without symmetric keys
/*mpz_t bigriv, bigerk;
  mpz_init(bigriv); mpz_init(bigerk);
  mpz_urandomb(bigerk, r_state, 128);
  mpz_urandomb(bigriv, r_state, 128);

  mpz_export(md_header.erk, &countp, 1, 0x10, 1, 0, bigerk);
  mpz_export(md_header.riv, &countp, 1, 0x10, 1, 0, bigriv);*/

// init signing shit
  mpz_t n,k,da,kinv,r,cs,z;
  mpz_init(n); mpz_init(k); mpz_init(da); mpz_init(r); mpz_init(cs); mpz_init(z); mpz_init(kinv);
  mpz_import(r, 0x14, 1, 1, 0, 0, KEY(R));
  mpz_import(n, 0x14, 1, 1, 0, 0, KEY(n));
  mpz_import(k, 0x14, 1, 1, 0, 0, KEY(K));
  mpz_import(da, 0x14, 1, 1, 0, 0, KEY(Da));
  mpz_invert(kinv, k, n);
  segment_certification_sign all_signed; memset(&all_signed, 0, sizeof(all_signed));
  mpz_export(all_signed.R, &countp, 1, 0x14, 1, 0, r);

// **** everything here is still length independent ***
  build_segment_crypt_data();
  set_u32(&(segment_header.crypt_len), (segment_crypt_data_len)/0x10);
  set_u32(&(segment_header.unknown2), 0x30);    // needed??
  printf("built crypt data\n");

// start building metadata in theory, ordering is fixed now
  memset(&start_file, 0, sizeof(file_ll));
  running_size = 0;
  // 0x000 -- Self_Shdr
  add_file_section(&output_self_header, sizeof(output_self_header));
  // 0x020 -- Self_Ehdr
  add_file_section(&output_extended_self_header, sizeof(output_extended_self_header));
  // 0x070 -- Self_Ihdr
  set_u64(&(output_extended_self_header.e_ihoff), running_size);
  add_file_section(&output_self_info_header, sizeof(output_self_info_header));
  // 0x090 -- elf data
  set_u64(&(output_extended_self_header.e_ehoff), running_size);
  set_u64(&(output_extended_self_header.e_phoff), running_size+get_u64(&(input_elf_header->e_phoff)));
  add_file_section(input_elf_data, get_u64(&(input_elf_header->e_phoff)) + get_u16(&(input_elf_header->e_phnum)) * sizeof(Elf64_Phdr));
  add_file_section(zero_padding, (0x10-(running_size&0xF))&0xF);
  // 0x*** -- all Self_PMhdr(including not in crypt)
  set_u64(&(output_extended_self_header.e_pmoff), running_size);
  segment_ptr = &first_segment;
  while(segment_ptr != NULL) {
    add_file_section(&(segment_ptr->pmhdr), sizeof(segment_ptr->pmhdr));
    segment_ptr = segment_ptr->next_segment;
  }
  // 0x*** -- Self_SDKversion
  set_u64(&(output_extended_self_header.e_svoff), running_size);
  add_file_section(&sdkversion, sizeof(sdkversion));
  // 0x*** -- ???
#ifdef NPDRM
  add_file_section(zero_padding, 0x20);
#endif
  // 0x*** -- Self_Cflags
  set_u64(&(output_extended_self_header.e_cfoff), running_size);
  add_file_section(&cflags, sizeof(cflags));
#ifdef NPDRM
  // 0x*** -- npdrm data
  add_file_section(&npdrm, sizeof(npdrm));
#endif
  // 0x*** -- metadata_crypt_header
  set_u32(&(output_self_header.s_esize), running_size - sizeof(output_self_header));
  add_file_section(&md_header, sizeof(md_header));
  // 0x*** -- segment_certification_header
  add_file_section(&segment_header, sizeof(segment_header));
  // 0x*** -- all segment_certification_segment incrypt
  int incrypt_count = 0;
  segment_ptr = &first_segment;
  while(segment_ptr != NULL) {
    if(segment_ptr->incrypt) {
      add_file_section(&(segment_ptr->enc_segment), sizeof(segment_ptr->enc_segment));
      incrypt_count++;
    }
    segment_ptr = segment_ptr->next_segment;
  }
  set_u32(&(segment_header.segment_count), incrypt_count);
  // 0x*** -- segment_crypt_data
  add_file_section(segment_crypt_data, segment_crypt_data_len);
  // 0x*** -- nubpadding_static
  add_file_section(nubpadding_static, sizeof(nubpadding_static));
  // 0x*** -- segment_certification_sign
  set_u64(&(segment_header.signature_offset), running_size);
  add_file_section(&all_signed, sizeof(all_signed));
  // 0x*** -- data must be 0x80 aligned
  if((running_size%0x80) != 0) {
    add_file_section(zero_padding, 0x80-(running_size%0x80));
  }
  // 0x*** -- data
  set_u64(&(output_self_header.s_shsize), running_size);
  // ...data...
  segment_ptr = &first_segment;
  while(segment_ptr != NULL) {
    set_u64(&(segment_ptr->enc_segment.segment_offset), running_size);
    set_u64(&(segment_ptr->pmhdr.pm_offset), running_size);
    add_file_section(segment_ptr->data, segment_ptr->len);
    add_file_section(zero_padding, segment_ptr->padding);
    segment_ptr = segment_ptr->next_segment;
  }
  // 0x*** -- section table
#ifndef SPRX
  set_u64(&(output_extended_self_header.e_shoff), running_size);
  add_file_section(input_elf_data+get_u64(&(input_elf_header->e_shoff)), get_u16(&(input_elf_header->e_shnum)) * sizeof(Elf64_Shdr));
#endif
  // ***DONE***

  printf("file built\n");

// write self file in memory <-- useful comment
  write_self_file_in_memory();
  printf("self written in memory\n");

// sign shit
  u8 digest[0x14];
  SHA1(output_self_data, get_u64(&(segment_header.signature_offset)), digest);

  mpz_import(z, 0x14, 1, 1, 0, 0, digest);
  mpz_mul(cs, r, da); mpz_mod(cs, cs, n);
  mpz_add(cs, cs, z); mpz_mod(cs, cs, n);
  mpz_mul(cs, cs, kinv); mpz_mod(cs, cs, n);
 
  //mpz_export(all_signed.S, &countp, 1, 0x14, 1, 0, cs);
  mpz_export(&output_self_data[get_u64(&output_self_data[get_u32(output_self_data+0xC)+0x60])+0x16], &countp, 1, 0x14, 1, 0, cs);

// encrypt metadata
  int metadata_offset = get_u32(&(output_self_header.s_esize)) + sizeof(Self_Shdr);

#ifndef NO_CRYPT
  memset(ecount_buf, 0, 16); num=0;
  AES_set_encrypt_key(&output_self_data[metadata_offset], 128, &aes_key);
  memcpy(iv, &output_self_data[metadata_offset+0x20], 16);
  AES_ctr128_encrypt(&output_self_data[0x40+metadata_offset], &output_self_data[0x40+metadata_offset], get_u64(&(output_self_header.s_shsize))-metadata_offset-0x40, &aes_key, iv, ecount_buf, &num);
  memcpy(&output_self_data[metadata_offset], KEY(keypair_e), sizeof(md_header));
  /*AES_set_encrypt_key(KEY(erk), 256, &aes_key);
  AES_cbc_encrypt(&output_self_data[metadata_offset], &output_self_data[metadata_offset], 0x40, &aes_key, iv, AES_ENCRYPT);*/
#else
  printf("NO_CRYPT is enabled...self is broken\n");
#endif
  
// write the output self
  FILE *output_self_file = fopen(argv[2], "wb");
  fwrite(output_self_data, 1, running_size, output_self_file);
  fclose(output_self_file);
  return 0;
}
