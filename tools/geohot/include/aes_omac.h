/* I mean if OpenSSL doesn't include it...
 * released under GPLv3, see http://gplv3.fsf.org/
 */

void rol1(u8* worthless) {
  int i;
  u8 xor = (worthless[0]&0x80)?0x87:0;
  for(i=0;i<0xF;i++) {
    worthless[i] <<= 1;
    worthless[i] |= worthless[i+1]>>7;
  }
  worthless[0xF] <<= 1;
  worthless[0xF] ^= xor;
}

void aesOmac1Mode(u8* output, u8* input, int len, u8* aes_key_data, int aes_key_bits) {
  int i,j;
  AES_KEY aes_key;
  AES_set_encrypt_key(aes_key_data, aes_key_bits, &aes_key);

  u8 running[0x10]; memset(running, 0, 0x10);
  u8 hash[0x10];
  u8 worthless[0x10];
  u8 final[0x10];

  AES_encrypt(running, worthless, &aes_key);
  rol1(worthless);

  if(len > 0x10) {
    for(i=0;i<(len-0x10);i+=0x10) {
      for(j=0;j<0x10;j++) hash[j] = running[j] ^ input[i+j];
      AES_encrypt(hash, running, &aes_key);
    }
  }
  int overrun = len&0xF;
  if( (len%0x10) == 0 ) overrun = 0x10;

  memset(hash, 0, 0x10);
  memcpy(hash, &input[i], overrun);

  if(overrun != 0x10) {
    hash[overrun] = 0x80;
    rol1(worthless);
  }

  for(j=0;j<0x10;j++) hash[j] ^= running[j] ^ worthless[j];
  AES_encrypt(hash, output, &aes_key);
}
