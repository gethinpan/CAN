
#ifndef _DES_H
#define _DES_H

#define encryption 0
#define decryption 1
typedef struct
{
	unsigned char k[8];
	unsigned char c[4];
	unsigned char d[4];
}keySet;

void genKey(unsigned char*);  //generate key
void genSubkeys(unsigned char*, keySet*); //generate subkey
void procMessage(unsigned char*, unsigned char*, keySet*, int);  //process message(encrypt or decryt)
#endif