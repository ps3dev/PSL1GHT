/* lol, I didn't know how real HMACs worked when I reversed this
 * released under GPLv3, see http://gplv3.fsf.org/
 */

#include <openssl/sha.h>

void SHA1_ghetto_init(SHA_CTX *c, u8* sha1_hmac) {
  u8 section[0x40];
  int i;
  SHA1_Init(c);
  memset(section, 0x36, 0x40);
  for(i=0;i<0x40;i++) section[i] = sha1_hmac[i] ^ 0x36;
  SHA1_Update(c, section, 0x40);
}

void SHA1_ghetto_final(u8* digest_out, SHA_CTX *c, u8* sha1_hmac) {
  u8 section[0x40];
  int i;
  u8 digest[0x14];
  SHA1_Final(digest, c);
  SHA1_Init(c);
  memset(section, 0x5C, 0x40);
  for(i=0;i<0x40;i++) section[i] = sha1_hmac[i] ^ 0x5C;
  SHA1_Update(c, section, 0x40);
  SHA1_Update(c, digest, 0x14);
  SHA1_Final(digest_out, c);
}
