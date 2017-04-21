#include <hidef.h>      /* common defines and macros */
#include <string.h>
#include <stdlib.h>
#include "derivative.h"      /* derivative-specific definitions */
#include "init.h"
#include "CAN.h"
#include "LCD.h"
#include "SCI.h"
#include "des.h"
#include "data.h"

#pragma CODE_SEG DEFAULT
void main(void) 
{
  byte randomNumber1,randomNumber2;
  unsigned long TxID0=ConvertID2MSCAN_ID(ID_CAN0);
  unsigned long TxID4=ConvertID2MSCAN_ID(ID_CAN4);
  Init_Dev();
  PUCR_PUPBE=1;
  EnableInterrupts;
  //ECU Certification Process
  for(;;)
  {
    while(RB0status!=NEWDATA);
    RB0status=READDATA;
    DESCommand=decryption;
    memcpy(SCI0TxBuffer+1,receiveBuffer0+1,8);
    putstr(SCI0TxBuffer,9);
    while(!SCI0status);
    SCI0status=EMPTY;
    if(SCI0RxBuffer[0]==ID_GW)    //SCI0RxBuffer--ID_GW|N1
    {
      memset(SCI0TxBuffer,0,8);
      randomNumber1=SCI0RxBuffer[1];
      srand(0);
      randomNumber2=rand()%256;
      DESCommand=encryption;
      SCI0TxBuffer[1]=ID_ECU_1;
      SCI0TxBuffer[2]=randomNumber1;
      SCI0TxBuffer[3]=randomNumber2;
      putstr(SCI0TxBuffer,9);
      while(!SCI0status);
      SCI0status=EMPTY;
      CAN0SendFrame(TxID0,0x00,8,SCI0RxBuffer);  //E(K,ID_ECU_x|N1|N2)
      while(RB0status!=NEWDATA);
      RB0status=READDATA;
      DESCommand=decryption;
      memcpy(SCI0TxBuffer+1,receiveBuffer0+1,8);
      putstr(SCI0TxBuffer,9);
      while(!SCI0status);
      SCI0status=EMPTY;
      if(SCI0RxBuffer[0]==randomNumber2)  //Certificate successfully
      {
        SCI0TxBuffer[0]=0x4F;
        SCI0TxBuffer[1]=0x4B;
        putstr(SCI0TxBuffer,9);
        SCI0RxBuffer[0]=0x4F;
        SCI0RxBuffer[1]=0x4B;
        CAN0SendFrame(TxID0,0x00,8,SCI0RxBuffer);
        break;
      }
      else    //fail
      {
        SCI0TxBuffer[0]=0x4E;
        SCI0TxBuffer[0]=0x4F;
        putstr(SCI0TxBuffer,9);
        SCI0RxBuffer[0]=0x4E;
        SCI0RxBuffer[1]=0x4F;
        CAN0SendFrame(TxID0,0x00,8,SCI0RxBuffer);
      }
    }
  }
  
  for(;;) 
  {
    _FEED_COP(); /* feeds the dog */
  } /* loop forever */
  /* please make sure that you never leave main */
}
