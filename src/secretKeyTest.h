
#ifndef DES_H
#define DES_H

void mikeypermutation(const unsigned char *main_key, unsigned char *key,unsigned char *subkey);
void mimsgpermutation(const unsigned char *main_key, unsigned char *key);
void miround(unsigned char *result,unsigned char *desD,const unsigned char *key,const unsigned char *e);
void miAddroundKey(unsigned char *result,unsigned char *key,unsigned char *msg);
void byteSub(unsigned char state[][4]);
#endif
