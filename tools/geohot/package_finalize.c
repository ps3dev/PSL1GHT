// package_finalize by geohot
// part of geohot's awesome tools for the PS3
//  released under GPLv3, see http://gplv3.fsf.org/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/ps3_common.h"
#include "include/oddkeys.h"
#include <openssl/modes.h>
#include <openssl/sha.h>
#include <openssl/aes.h>

typedef struct {
  u32 magic;
  u32 debugFlag;
  u32 infoOffset;
  u32 unknown1;
  u32 headSize;
  u32 itemCount;
  u64 packageSize;
  u64 dataOffset;
  u64 dataSize;
} pkg_header;

int main(int argc, char* argv[]) {
  u8* data;
  u8 sha_key[0x40];
  
  if(argc < 2) {
    printf("usage: %s my.pkg\n", argv[0]);
    return -1;
  }

  int i;
  FILE *f=fopen(argv[1], "rb");
  fseek(f, 0, SEEK_END);
  int nlen = ftell(f);
  fseek(f, 0, SEEK_SET);
  data = (u8*)malloc(nlen);
  fread(data, 1, nlen, f);
  fclose(f);

  pkg_header *header = (pkg_header *)data;
  int data_offset = get_u64(&(header->dataOffset));
  int data_size = get_u64(&(header->dataSize));

// decrypt debug
  u8 sha_crap[0x40];
  memset(sha_crap, 0, 0x40);
  memcpy(sha_crap, &data[0x60], 8);
  memcpy(sha_crap+0x8, &data[0x60], 8);
  memcpy(sha_crap+0x10, &data[0x68], 8);
  memcpy(sha_crap+0x18, &data[0x68], 8);

  int dptr;
  for(dptr = data_offset; dptr < (data_offset+data_size); dptr+=0x10) {
    u8 hash[0x14];
    SHA1(sha_crap, 0x40, hash);
    for(i=0;i<0x10;i++) data[dptr+i] ^= hash[i];
    set_u64(sha_crap+0x38, get_u64(sha_crap+0x38)+1);
  }

// recrypt retail
  u8 pkg_key[0x10];
  memcpy(pkg_key, &data[0x70], 0x10);

  AES_KEY aes_key;
  AES_set_encrypt_key(retail_pkg_aes_key, 128, &aes_key);

  int num=0; u8 ecount_buf[0x10]; memset(ecount_buf, 0, 0x10);
  CRYPTO_ctr128_encrypt(&data[data_offset], &data[data_offset], data_size, &aes_key, pkg_key, ecount_buf, &num, (block128_f)AES_encrypt);

// write back
  FILE *g = fopen(argv[1], "wb");
  data[4] = 0x80;   // set finalize flag
  memset(&data[(data_offset+data_size)], 0, 0x60);

  // add hash
  SHA1(data, nlen-0x20, &data[nlen-0x20]);
  fwrite(data, 1, nlen, g);
  fclose(g);
}
