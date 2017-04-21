#ifndef _SCI_H
#define _SCI_H

#include "derivative.h"

extern byte SCI0Buffer[10];
extern byte SCI0status;

enum scistatus
{ EMPTY,      /* 0 */
  RECEIVED
};

extern void SCI_Init(void);
extern unsigned char getchar(void);
extern void putchar(unsigned char ch);
extern void putstr(char ch[],byte cnt);

#endif