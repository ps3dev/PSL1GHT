// SELF structs
//  released under GPLv3, see http://gplv3.fsf.org/

// all big endian
typedef struct {
  u32 s_magic;      // set to 0x53434500(SCE)
  u32 s_hdrversion; // set to 2
  u16 s_flags;      // 1 is retail
  u16 s_hdrtype;    // 1=SELF, 3=PKG
  u32 s_esize;      // extended self header size(metadata - sizeof(Self_Shdr))
// at +0x10
  u64 s_shsize;     // self header size
  u64 s_exsize;     // extracted file size
} Self_Shdr;    // self header

typedef struct {
  u64 e_magic;      // set to 3
  u64 e_ihoff;      // info header offset in file
// at +0x10
  u64 e_ehoff;      // elf header offset
  u64 e_phoff;      // elf phdr offset
// at +0x20
  u64 e_shoff;      // elf shdr offset
  u64 e_pmoff;      // elf phdr map offset
// at +0x30
  u64 e_svoff;      // SDK version offset
  u64 e_cfoff;      // control flags offset
// at +0x40
  u64 e_cfsize;     // control flags size
  u64 e_padding;
} Self_Ehdr;    // extended self header

typedef struct {
  u64 i_authid;     // authid
  u32 i_magic;      // 0x01000002
  u32 i_apptype;    // 4 is application
  u64 i_version;    // 0x0001004000001000 was a good version
  u64 i_padding;
} Self_Ihdr;    // self info header

typedef struct {
  u8 version[0x10];
} Self_SDKversion;

typedef struct {
  u8 cflags1[0x10];
  u8 cflags2[0x20];
  u8 cflags3[0x10];
  u8 hashes[0x30];
} Self_Cflags;

typedef struct {
  u32 block_type;  // this is 3(NPDRM)
  u32 block_size;  // this is 0x90(sizeof(Self_NPDRM))
  u32 unknown1;
  u32 unknown2;
  u32 magic;       // 0x4E504400(NPD)
  u32 unknown3;    // this is 1
  u32 unknown4;    // this is 3
  u32 unknown5;    // this is 1
  u8 titleid[0x30];
  u8 hash_unknown[0x10];
  u8 hash1[0x10];
  u8 hash2[0x10];
  u8 padding[0x10];
} Self_NPDRM;

typedef struct {
  u64 pm_offset;
  u64 pm_size;
  u32 pm_compressed; // 2 for compressed, 1 for pt
  u32 pm_unknown2;
  u32 pm_unknown3;
  u32 pm_encrypted;  // 1 for encrypted
} Self_PMhdr;   // phdr map

typedef struct {
  u64 signature_offset;
  u32 version;
  u32 segment_count;
  u32 crypt_len;  // /0x10
  u32 unknown2; // seems to be 0x30
  u64 padding;
} segment_certification_header;

typedef struct {
  u64 segment_offset;
  u64 segment_size;
  u32 segment_type;
  u32 segment_number;
  u32 segment_hashed_flag;
  u32 segment_sha1_index;
  u32 segment_encrypted_flag;
  u32 segment_erk_index;
  u32 segment_riv_index;
  u32 segment_compressed_flag;
} segment_certification_segment;

typedef struct {
  u8 erk[0x10];
  u8 padding1[0x10];
  u8 riv[0x10];
  u8 padding2[0x10];
} metadata_crypt_header;

typedef struct {
  u8 sha1[0x20];
  u8 hmac[0x40];
  u8 erk[0x10];
  u8 riv[0x10];
} segment_certification_crypt;

typedef struct {
  u8 padding1[1];
  u8 R[0x14];
  u8 padding2[1];
  u8 S[0x14];
  u8 padding3[6];
} segment_certification_sign;

typedef struct {
  Self_PMhdr pmhdr;
  segment_certification_segment enc_segment;
  segment_certification_crypt crypt_segment;
  u8 incrypt;
  u8 compressed;
  u8 encrypted;
  u8* data;
  u32 len;
  u32 padding; // amount of padding
  u32 rlen;
  void* next_segment;
} Self_Segment;
