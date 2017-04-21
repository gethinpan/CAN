
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "des.h"

//initial permutation
int IP[] =
{
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17,  9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

//inverse initial  permutation
int IIP[] =
{
	40,  8, 48, 16, 56, 24, 64, 32,
	39,  7, 47, 15, 55, 23, 63, 31,
	38,  6, 46, 14, 54, 22, 62, 30,
	37,  5, 45, 13, 53, 21, 61, 29,
	36,  4, 44, 12, 52, 20, 60, 28,
	35,  3, 43, 11, 51, 19, 59, 27,
	34,  2, 42, 10, 50, 18, 58, 26,
	33,  1, 41,  9, 49, 17, 57, 25
};

//expansion permutation
int EP[]=
{
	32,  1,  2,  3,  4,  5,
	4,  5,  6,  7,  8,  9,
	8,  9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32,  1
};

//S-Boxs
int S[][64] =
{
	//S1
	14,  4, 13,  1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
	//S2
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
	//S3
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
	//S4
	7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
	3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
	//S5
	2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
	14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
	4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
	11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
	//S6
	12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
	10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
	9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
	4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
	//S7
	4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
	13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
	1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
	6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
	//S8
	13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
	1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
	7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
	2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

//P-Box
int P[] =
{
	16,  7, 20, 21,
	29, 12, 28, 17,
	1, 15, 23, 26,
	5, 18, 31, 10,
	2,  8, 24, 14,
	32, 27,  3,  9,
	19, 13, 30,  6,
	22, 11,  4, 25
};

//initial permutation for key
int PC1[] =
{
	57, 49,  41, 33,  25,  17,  9,
	1, 58,  50, 42,  34,  26, 18,
	10,  2,  59, 51,  43,  35, 27,
	19, 11,   3, 60,  52,  44, 36,
	63, 55,  47, 39,  31,  23, 15,
	7, 62,  54, 46,  38,  30, 22,
	14,  6,  61, 53,  45,  37, 29,
	21, 13,   5, 28,  20,  12,  4
};

//permutation for sub key
int PC2[] =
{
	14, 17, 11, 24,  1,  5,
	3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8,
	16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

//key shift,the first number is unused
int KS[] = { -1, 1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1 };



//generate key
void genKey(unsigned char *key)
{
  int i;
	for (i = 0; i < 8; ++i)
	{
		key[i] = rand() % 255;
	}
}

//generate subkeys
void genSubkeys(unsigned char *mainKey, keySet *subkey)
{
	int i,j;
	int bitIndex,chosenBit,shiftSize,bitMask;
	int shiftBits[4];

	for (i = 0; i < 8; ++i)
	{
		subkey[0].k[i] = 0;
	}

	//initial permutation,place the result in subkey.k
	for (i = 0; i < 56; ++i)
	{
		bitIndex = PC1[i];
		chosenBit = 0x80 >> ((bitIndex - 1) % 8);
		chosenBit &= mainKey[(bitIndex - 1) / 8];
		chosenBit <<= ((bitIndex - 1) % 8);
		subkey[0].k[i / 8] |= (chosenBit >> (i % 8));
	}

	//divide subkey.k into c and d
	for (i = 0; i < 3; ++i)
	{
		subkey[0].c[i] = subkey[0].k[i];
	}
	subkey[0].c[3] = subkey[0].k[3] & 0xF0;

	for (i = 0; i < 3; ++i)
	{
		subkey[0].d[i] = (subkey[0].k[i + 3] & 0x0F) << 4;   //pay attentionv to shift
		subkey[0].d[i] |= (subkey[0].k[i + 4] & 0xF0) >> 4;
	}
	subkey[0].d[3] = (subkey[0].k[6] & 0x0F) << 4;

	for(i=1;i<17;++i)
	{
		for (j = 0; j < 4; ++j)
		{
			subkey[i].c[j] = subkey[i - 1].c[j];
			subkey[i].d[j] = subkey[i - 1].d[j];
		}
		memset(subkey[i].k, 0, 8);

		shiftSize = KS[i];
		if (shiftSize == 1)
		{
			bitMask = 0x80;
		}
		else
		{
			bitMask = 0xC0;
		}

		//process c
		shiftBits[0] = bitMask&subkey[i].c[0];
		shiftBits[1] = bitMask&subkey[i].c[1];
		shiftBits[2] = bitMask&subkey[i].c[2];
		shiftBits[3] = bitMask&subkey[i].c[3];
		
		for (j = 0; j < 3; ++j)
		{
			subkey[i].c[j] <<= shiftSize;
			subkey[i].c[j] |= (shiftBits[j+1] >> (8 - shiftSize));
		}
		subkey[i].c[3] <<= shiftSize;
		subkey[i].c[3] |= (shiftBits[0] >> (4 - shiftSize));

		//process d
		shiftBits[0] = bitMask&subkey[i].d[0];
		shiftBits[1] = bitMask&subkey[i].d[1];
		shiftBits[2] = bitMask&subkey[i].d[2];
		shiftBits[3] = bitMask&subkey[i].d[3];

		for (j = 0; j < 3; ++j)
		{
			subkey[i].d[j] <<= shiftSize;
			subkey[i].d[j] |= (shiftBits[j + 1] >> (8 - shiftSize));
		}
		subkey[i].d[3] <<= shiftSize;
		subkey[i].d[3] |= (shiftBits[0] >> (4 - shiftSize));
		
		for (j = 0; j < 48; ++j)
		{
			bitIndex = PC2[j];
			if (bitIndex <= 28)
			{
				chosenBit = 0x80 >> ((bitIndex - 1) % 8);
				chosenBit &= subkey[i].c[(bitIndex - 1) / 8];
				chosenBit <<= ((bitIndex - 1) % 8);
			}
			else
			{
				chosenBit = 0x80 >> ((bitIndex - 29) % 8);
				chosenBit &= subkey[i].d[(bitIndex - 29) / 8];
				chosenBit <<= ((bitIndex - 29) % 8);
			}
			subkey[i].k[j / 8] |= (chosenBit >> (j % 8));
		}
	}
}

void procMessage(unsigned char *rawMessage, unsigned char *processedMessage, keySet *subkey, int mode)
{
	int i,j;  //variable for iteration
	int bitIndex, chosenBit,keyIndex;
	unsigned char row, column;
	unsigned char tempMessage[8],L[4],R[4],TL[4],TR[4],ER[6],SR[4],preIIP[8];   //ER is result after expansion permutation,
	                                                            //SR is result after substitution
	memset(tempMessage, 0, 8);
	memset(processedMessage, 0, 8);
	//initial permutation
	for (i = 0; i < 64; ++i)
	{
		bitIndex = IP[i];
		chosenBit = 0x80 >> ((bitIndex - 1) % 8);
		chosenBit &= rawMessage[(bitIndex - 1) / 8];
		chosenBit <<= ((bitIndex - 1) % 8);
		tempMessage[i / 8] |= (chosenBit >> (i % 8));
	}

	//divide tempMessage into L and R
	memcpy(L, tempMessage, 4);
	memcpy(R, tempMessage + 4, 4);
	
	for (i = 0; i < 16; ++i)
	{
		memcpy(TL, R, 4);
		memset(ER, 0, 6);
		//expansion permutation
		for (j = 0; j < 48; ++j)
		{
			bitIndex = EP[j];
			chosenBit = 0x80 >> ((bitIndex - 1) % 8);
			chosenBit &= R[(bitIndex - 1) / 8];
			chosenBit <<= ((bitIndex - 1) % 8);
			ER[j / 8] |= (chosenBit >> (j % 8));
		}
		if (mode == encryption)
		{
			keyIndex = i + 1;
		}
		else
		{
			keyIndex = 16 - i;
		}
		//key mixing
		for (j = 0; j < 6; ++j)
		{
			ER[j] ^= subkey[keyIndex].k[j];
		}
		memset(SR, 0, 4);
		
		//subtitution(S-Box)
		// Byte 1
		row = 0;
		row |= ((ER[0] & 0x80) >> 6);
		row |= ((ER[0] & 0x04) >> 2);

		column = 0;
		column |= ((ER[0] & 0x78) >> 3);

		SR[0] |= ((unsigned char)S[0][row * 16 + column] << 4);

		row = 0;
		row |= (ER[0] & 0x02);
		row |= ((ER[1] & 0x10) >> 4);

		column = 0;
		column |= ((ER[0] & 0x01) << 3);
		column |= ((ER[1] & 0xE0) >> 5);

		SR[0] |= (unsigned char)S[1][row * 16 + column];

		// Byte 2
		row = 0;
		row |= ((ER[1] & 0x08) >> 2);
		row |= ((ER[2] & 0x40) >> 6);

		column = 0;
		column |= ((ER[1] & 0x07) << 1);
		column |= ((ER[2] & 0x80) >> 7);

		SR[1] |= ((unsigned char)S[2][row * 16 + column] << 4);

		row = 0;
		row |= ((ER[2] & 0x20) >> 4);
		row |= (ER[2] & 0x01);

		column = 0;
		column |= ((ER[2] & 0x1E) >> 1);

		SR[1] |= (unsigned char)S[3][row * 16 + column];

		// Byte 3
		row = 0;
		row |= ((ER[3] & 0x80) >> 6);
		row |= ((ER[3] & 0x04) >> 2);

		column = 0;
		column |= ((ER[3] & 0x78) >> 3);

		SR[2] |= ((unsigned char)S[4][row * 16 + column] << 4);

		row = 0;
		row |= (ER[3] & 0x02);
		row |= ((ER[4] & 0x10) >> 4);

		column = 0;
		column |= ((ER[3] & 0x01) << 3);
		column |= ((ER[4] & 0xE0) >> 5);
		SR[2] |= (unsigned char)S[5][row * 16 + column];

		// Byte 4
		row = 0;
		row |= ((ER[4] & 0x08) >> 2);
		row |= ((ER[5] & 0x40) >> 6);

		column = 0;
		column |= ((ER[4] & 0x07) << 1);
		column |= ((ER[5] & 0x80) >> 7);

		SR[3] |= ((unsigned char)S[6][row * 16 + column] << 4);

		row = 0;
		row |= ((ER[5] & 0x20) >> 4);
		row |= (ER[5] & 0x01);

		column = 0;
		column |= ((ER[5] & 0x1E) >> 1);

		SR[3] |= (unsigned char)S[7][row * 16 + column];

		memset(TR, 0, 4);

		//P-Box
		for (j = 0; j < 32; ++j)
		{
			bitIndex = P[j];
			chosenBit = 0x80 >> ((bitIndex - 1) % 8);
			chosenBit &= SR[(bitIndex - 1) / 8];
			chosenBit <<= ((bitIndex - 1) % 8);
			TR[j / 8] |= (chosenBit >> j % 8);
		}

		//XOR
		for (j = 0; j < 4; ++j)
		{
			TR[j] ^= L[j];
		}

		//exchange
		for (j = 0; j < 4; ++j)
		{
			L[j] = TL[j];
			R[j] = TR[j];
		}
	}

	//preprocess before inverse initial permutation
	for (j = 0; j < 4; ++j)
	{
		preIIP[j] = R[j];
		preIIP[j + 4] = L[j];
	}

	//inverse initial permutation
	for (j = 0; j < 64; ++j)
	{
		bitIndex = IIP[j];
		chosenBit = 0x80 >> ((bitIndex - 1) % 8);
		chosenBit &= preIIP[(bitIndex - 1) / 8];
		chosenBit <<= ((bitIndex - 1) % 8);
		processedMessage[j / 8] |= (chosenBit >> (j % 8));
	}
}
