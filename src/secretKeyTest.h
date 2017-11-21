
#ifndef DES_H
#define DES_H

void mikeypermutation(const unsigned char *main_key, unsigned char *key,unsigned char *subkey);
void mimsgpermutation(const unsigned char *main_key, unsigned char *key);
void miround(unsigned char *result,unsigned char *desD,const unsigned char *key,const unsigned char *e);
void miAddroundKey(unsigned char *result,unsigned char *key,unsigned char *msg);
void byteSub(unsigned char state[][4]);
long calculate_d_RSA(int numx, int numy);
long modInverse(long a, long m);
long mod_exp (long b, long e, long m);
void cipher_message (long msg, long e, long modulo);
void decipher_message (long msg, long e, long modulo);
void generate_public_key_ElGammal(long alpha,long privateKey,long group);
void decipher_ElGammal(long message, long alphabv,long privateKey,long group);
#endif
