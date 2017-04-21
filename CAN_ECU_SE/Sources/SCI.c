#include "SCI.h"


void SCI_Init(void) 
{
   SCI0CR2=0x2c; //enable Receive Full Interrupt,RX enable,Tx enable
   SCI0BDH=0x00; //busclk  8MHz,19200bps,SCI0BDL=0x1a
   SCI0BDL=0x68; //SCI0BDL=busclk/(16*SCI0BDL)                 
}

unsigned char getchar(void)
{  
   while(!(SCI0SR1&0x20)) ; 		 //keep waiting when not empty  
   return SCI0DRL;
}
/*
void uart_init(void) {
  SCI0CR2=0x0c;
  SCI0BDH=0x00;//16MHz,19200bps,SCI0BDL=0x1a
  SCI0BDL=0x34;//16MHz,9600bps,SCI0BDL=0x34 
}
*/ 
void putchar(unsigned char ch)
{ 
  if (ch == '\n')  
  {
      while(!(SCI0SR1&0x80)) ;     
      SCI0DRL= 0x0d;       				 //output'CR'
	    return;
  }
  while(!(SCI0SR1&0x80)) ; 		    //keep waiting when not empty  
  SCI0DRL=ch;
}

void putstr(char ch[],byte cnt)
{
  unsigned char ptr=0;
  while(ptr<cnt){
      putchar((unsigned char)ch[ptr++]);
  }      
}

#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt 20 SCI0_ISR(void)
{ 
  byte i;    
  SCI0CR2_RIE=0;
  //此处为串口中断需要处理的事情
  SCI0status=RECEIVED;
  for(i=0;i<9;++i) 
  {
     SCI0Buffer[i]=getchar();
  }
  SCI0CR2_RIE = 1;    
}


