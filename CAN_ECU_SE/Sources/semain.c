#include <hidef.h>      /* common defines and macros */
#include <string.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "init.h"
#include "CAN.h"
#include "LCD.h"
#include "SCI.h"
#include "des.h"

const byte initKey[8]={0x01,0x23,0x45,0x67,0x89,0xAB,0xCD,0xEF};  //initial key
byte sessionKey[8];    //session key
keySet initSubkey[17],sessionSubkey[17];
byte processedMessage[8];
byte hex[17];

#pragma CODE_SEG DEFAULT
void main(void) {
  Init_Dev();
  PUCR_PUPBE=1;
  EnableInterrupts;
  genSubkeys(initKey,initSubkey);
  while(!SCI0status);
  SCI0status=EMPTY;
  procMessage(SCI0Buffer+1,processedMessage,initSubkey,decryption);
  putstr(processedMessage,8);
  while(!SCI0status);
  SCI0status=EMPTY;
  procMessage(SCI0Buffer+1,processedMessage,initSubkey,encryption);
  putstr(processedMessage,8);
  while(!SCI0status);
  SCI0status=EMPTY;
  procMessage(SCI0Buffer+1,processedMessage,initSubkey,decryption);
  //copy processedMessage to sessionKey
  sessionKey[0]=processedMessage[1]&0xFE;
  sessionKey[1]=((processedMessage[2]>>1)&0xFE)|((processedMessage[1]&0x01)<<7);
  sessionKey[2]=((processedMessage[3]>>2)&0xFE)|((processedMessage[2]&0x03)<<6);
  sessionKey[3]=((processedMessage[4]>>3)&0xFE)|((processedMessage[3]&0x07)<<5);
  sessionKey[4]=((processedMessage[5]>>4)&0xFE)|((processedMessage[4]&0x0F)<<4);
  sessionKey[5]=((processedMessage[6]>>5)&0xFE)|((processedMessage[5]&0x1F)<<3);
  sessionKey[6]=((processedMessage[7]>>6)&0xFE)|((processedMessage[6]&0x3F)<<2);
  sessionKey[7]=(processedMessage[7]&0x7F)<<1;
  putstr(processedMessage,8);
  putstr(sessionKey,8);
  genSubkeys(sessionKey,sessionSubkey);
  for(;;) 
  {
      _FEED_COP(); /* feeds the dog */
      while(!SCI0status);
      SCI0status=EMPTY;
      procMessage(SCI0Buffer+1,processedMessage,sessionSubkey,SCI0Buffer[0]);
      putstr(processedMessage,8);
  } /* loop forever */
  /* please make sure that you never leave main */
}