
#ifndef DES_H
#define DES_H

void mikeypermutation(const unsigned char *main_key, unsigned char *key,unsigned char *subkey);
void mimsgpermutation(const unsigned char *main_key, unsigned char *key);
void miround(unsigned char *result,unsigned char *desD,const unsigned char *key,const unsigned char *e);
void miAddroundKey(unsigned char *result,unsigned char *key,unsigned char *msg);
void byteSub(unsigned char state[][4]);
int calculate_d_RSA(int numx, int numy);
int modInverse(int a, int m);
long mod_exp (long b, long e, long m);
void cipher_message (long msg, long e, long modulo);
void decipher_message (long msg, long e, long modulo);
#endif
