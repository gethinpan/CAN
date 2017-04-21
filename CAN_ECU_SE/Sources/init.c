/********************************************
Target : MC9S12XEP100
Crystal: 16.000Mhz
busclock:16.000MHz
pllclock:32.000MHz
---------------------------------------------
function：initialize device
============================================*/
#include "derivative.h"   //y
//#include "LQprintp.h"      
#include "LCD.h"
#include "CAN.h"
#include "ADC.h"
#include "SCI.h"
  
//extern unsigned long CAN0_ID;
//extern unsigned long CAN4_ID; 


#pragma CODE_SEG DEFAULT
void delayms(int ms)
{   
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<1335;jj++);    //16MHz--1ms
     //for(jj=0;jj<4006;jj++);  //48MHz--1ms 
     //for(jj=0;jj<5341;jj++);  //64MHz--1ms 
}

void InitPorts(void) 
{
  RDRT=0xff;          // reduce the power of T port   
  RDRIV=0x93;         // reduce the power   of
  DDRB=0xff;          // set  port B bit0 as output
  PORTB=0X00;
  DDRT_DDRT4=1;       // set portT bit 4 as output
  DDRP_DDRP3=1;       // reduce the power of port P3
  RDRP_RDRP3=1;       // set portP bit 3 as output
}

// PLL初始化子程序   BUS Clock=8M
void setbusclock(void)
{   
    CLKSEL=0X00;				// disengage PLL to system
    PLLCTL_PLLON=1;			// turn on PLL
    SYNR=0x00 | 0x01; 	// VCOFRQ[7:6];SYNDIV[5:0]
                        // fVCO= 2*fOSC*(SYNDIV + 1)/(REFDIV + 1)
                        // fPLL= fVCO/(2 × POSTDIV) 
                        // fBUS= fPLL/2 
                        // VCOCLK Frequency Ranges  VCOFRQ[7:6]
                        // 32MHz <= fVCO <= 48MHz    00
                        // 48MHz <  fVCO <= 80MHz    01
                        // Reserved                  10
                        // 80MHz <  fVCO <= 120MHz   11				
    REFDV=0x80 | 0x01;  // REFFRQ[7:6];REFDIV[5:0]
                        // fREF=fOSC/(REFDIV + 1)
                        // REFCLK Frequency Ranges  REFFRQ[7:6]
                        // 1MHz <= fREF <=  2MHz       00
                        // 2MHz <  fREF <=  6MHz       01
                        // 6MHz <  fREF <= 12MHz       10
                        // fREF >  12MHz               11                         
                        // pllclock=2*osc*(1+SYNR)/(1+REFDV)=32MHz;
    POSTDIV=0x00;       // 4:0, fPLL= fVCO/(2xPOSTDIV)
                        // If POSTDIV = $00 then fPLL is identical to fVCO (divide by one).
    _asm(nop);          // BUS CLOCK=16M
    _asm(nop);
    while(!(CRGFLG_LOCK==1));	  //when pll is steady ,then use it;
    CLKSEL_PLLSEL =1;		        //engage PLL to system;
}
  
void Init_Dev(void)
{
  setbusclock();
  InitPorts();
  SCI_Init();
  InitCAN0();
  InitCAN4();
  LCD_Init();
  ADC_Init();
}
  