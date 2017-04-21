#include "derivative.h"
#include "LCD.h"
#include "CAN.h"

/*CAN0 Acceptance Code Definitions */ 
#define ACC_CAN0_EX_ID ConvertID2MSCAN_ID(CAN0_ID)//|(0X00180000)&(0xFFFFFFFE))//È·±£Ö¡ÀàÐÍÎªÀ©Õ¹Êý¾ÝÖ¡
#define ACC_CAN0_EX_ID_R0 ((ACC_CAN0_EX_ID&0xFF000000)>>24)
#define ACC_CAN0_EX_ID_R1 ((ACC_CAN0_EX_ID&0x00FF0000)>>16)
#define ACC_CAN0_EX_ID_R2 ((ACC_CAN0_EX_ID&0x0000FF00)>>8)
#define ACC_CAN0_EX_ID_R3  (ACC_CAN0_EX_ID&0x000000FF)

/*CAN0 Mask Code Definitions */
#define MASK_CAN0_EX_ID 0x00070000
#define MASK_CAN0_EX_ID_R0 ((MASK_CAN0_EX_ID&0xFF000000)>>24)
#define MASK_CAN0_EX_ID_R1 ((MASK_CAN0_EX_ID&0x00FF0000)>>16)
#define MASK_CAN0_EX_ID_R2 ((MASK_CAN0_EX_ID&0x0000FF00)>>8)
#define MASK_CAN0_EX_ID_R3  (MASK_CAN0_EX_ID&0x000000FF)

/*CAN4 Acceptance Code Definitions */ 
#define ACC_CAN4_EX_ID ConvertID2MSCAN_ID(CAN4_ID)//|(0X00180000)&(0xFFFFFFFE))
#define ACC_CAN4_EX_ID_R0 ((ACC_CAN4_EX_ID&0xFF000000)>>24)
#define ACC_CAN4_EX_ID_R1 ((ACC_CAN4_EX_ID&0x00FF0000)>>16)
#define ACC_CAN4_EX_ID_R2 ((ACC_CAN4_EX_ID&0x0000FF00)>>8)
#define ACC_CAN4_EX_ID_R3  (ACC_CAN4_EX_ID&0x000000FF)

/*CAN4 Mask Code Definitions */
#define MASK_CAN4_EX_ID 0x00070000
#define MASK_CAN4_EX_ID_R0 ((MASK_CAN4_EX_ID&0xFF000000)>>24)
#define MASK_CAN4_EX_ID_R1 ((MASK_CAN4_EX_ID&0x00FF0000)>>16)
#define MASK_CAN4_EX_ID_R2 ((MASK_CAN4_EX_ID&0x0000FF00)>>8)
#define MASK_CAN4_EX_ID_R3  (MASK_CAN4_EX_ID&0x000000FF)

unsigned long ConvertID2MSCAN_ID(unsigned long OID)
{
   unsigned long tmID=0;
   tmID=OID<<1;   
   tmID=((tmID&0xffe00000)<<2)|(tmID&0x000fffff|0x00180000);   
   return tmID;   
}  

// ----------------------initial CAN0rig--------------------
void InitCAN0(void) 
{
  CAN0CTL0 = 0x01;            /* Enter Initialization Mode 
                               *  
                               *  0b00000001
                               *    ||||||||__ Enter Initialization Mode
                               *    |||||||___ Sleep Mode Request bit
                               *    ||||||____ Wake-Up disabled
                               *    |||||_____ Time stamping disabled
                               *    ||||______ Synchronized Status
                               *    |||_______ CAN not affected by Wait
                               *    ||________ Receiver Active Status bit
                               *    |_________ Received Frame Flag bit
                               */                                 
  while(CAN0CTL1_INITAK!=1); 	// Wait for Initialization Mode acknowledge INITRQ bit = 1  
  CAN0CTL1 = 0x80;            /* Enable MSCAN module and not LoopBack Mode
                               *  
                               *  0b10100000
                               *    ||||||||__ Initialization Mode Acknowledge
                               *    |||||||___ Sleep Mode Acknowledge
                               *    ||||||____ Wake-up low-pass filter disabled
                               *    |||||_____ Unimplemented
                               *    ||||______ Listen Only Mode disabled
                               *    |||_______ not Loop Back Mode enabled
                               *    ||________ Ext Osc/Xtal as Clock Source
                               *    |_________ MSCAN Module enabled
                               */     
  CAN0BTR0 = 0x43;            /* Synch Jump = 2 Tq clock Cycles 
                               *
                               *  0b01000011
                               *    ||||||||__
                               *    |||||||___\
                               *    ||||||____ |
                               *    |||||_____ |_ CAN Clock Prescaler = 4
                               *    ||||______ |
                               *    |||_______/
                               *    ||________
                               *    |_________>- SJW = 2
                               */     
  CAN0BTR1 = 0x14;	          /* Set Number of samples per bit, TSEG1 and TSEG2 
                               * bit rate=Fclk/pres v/(1+TSEG1+TSEG2)=16M/4/(1+2+5)=500kbps
                               *  0b00010100
                               *    ||||||||__
                               *    |||||||___\
                               *    ||||||____ |- TSEG1 = 5
                               *    |||||_____/
                               *    ||||______
                               *    |||_______\_ TSEG2 = 2
                               *    ||________/
                               *    |_________ One sample per bit
                               */                               
   CAN0IDAC = 0x00;           /* Set two 32-bit Filters 
                               *  
                               *  0b00000000
                               *    ||||||||__ 
                               *    |||||||___\_ Filter Hit Indicator 
                               *    ||||||____/
                               *    |||||_____ Unimplemented 
                               *    ||||______ 
                               *    |||_______>- two 32-bit Acceptance Filters   
                               *    ||________ 
                               *    |_________>- Unimplemented 
                               */                                
  /* note Acceptance Filters neither Acceptance Filter is accorded with,message in receivebuffer will pass  */   
  CAN0IDAR0 = ACC_CAN0_EX_ID_R0;   // |\    32 bit Filter 0
  CAN0IDAR1 = ACC_CAN0_EX_ID_R1;   // | \__ Accepts Extended Data Frame Msg 
  CAN0IDAR2 = ACC_CAN0_EX_ID_R2;   // | /   with ID: CAN0_ID 
  CAN0IDAR3 = ACC_CAN0_EX_ID_R3;   // |/
  CAN0IDAR4 = ACC_CAN0_EX_ID_R0;   // |\    32 bit Filter 0
  CAN0IDAR5 = ACC_CAN0_EX_ID_R1;   // | \__ Accepts Extended Data Frame Msg 
  CAN0IDAR6 = ACC_CAN0_EX_ID_R2;   // | /   with ID: CAN0_ID 
  CAN0IDAR7 = ACC_CAN0_EX_ID_R3;   // |/
  CAN0IDMR0 = 0xFF;
  CAN0IDMR1 = 0xFF;
  CAN0IDMR2 = 0xFF;
  CAN0IDMR3 = 0xFF;
  CAN0IDMR4 = 0xFF;
  CAN0IDMR5 = 0xFF;
  CAN0IDMR6 = 0xFF;
  CAN0IDMR7 = 0xFF;
  /*----------------------------  
  CAN0IDMR0 = MASK_CAN0_EX_ID_R0;
  CAN0IDMR1 = MASK_CAN0_EX_ID_R1;
  CAN0IDMR2 = MASK_CAN0_EX_ID_R2;
  CAN0IDMR3 = MASK_CAN0_EX_ID_R3;
  CAN0IDMR4 = MASK_CAN0_EX_ID_R0;
  CAN0IDMR5 = MASK_CAN0_EX_ID_R1;
  CAN0IDMR6 = MASK_CAN0_EX_ID_R2;
  CAN0IDMR7 = MASK_CAN0_EX_ID_R3;
  ------------------------------*/
  
  CAN0CTL0 = 0x00;                /* Exit Initialization Mode Request */
  while ((CAN0CTL1&0x01)!=0);	    // µÈ´ý£¬Ö±µ½MSCANÕý³£ÔËÐÐ
  while(!(CAN0CTL0&0x10));        // µÈ´ý£¬Ö±µ½MSCANÓëCAN×ÜÏßÍ¬²½¥
  CAN0RFLG = 0xC3;          /* Reset Receiver Flags
                             *  
                             *  0b11000011
                             *    ||||||||__ Receive Buffer Full Flag
                             *    |||||||___ Overrun Interrupt Flag
                             *    ||||||____ 
                             *    |||||_____>- Transmitter Status Bits
                             *    ||||______ 
                             *    |||_______>- Receiver Status Bits
                             *    ||________ CAN Status Change Interrupt Flag
                             *    |_________ Wake-Up Interrupt Flag
                             */   
  CAN0RIER = 0x01;          /* Enable Receive Buffer Full Interrupt
                             *  
                             *  0b00000001
                             *    ||||||||__ Receive Buffer Full Int enabled
                             *    |||||||___ Overrun Int disabled
                             *    ||||||____ 
                             *    |||||_____>- Tx Status Change disabled
                             *    ||||______ 
                             *    |||_______>- Rx Status Change disabled
                             *    ||________ Status Change Int disabled
                             *    |_________ Wake-Up Int disabled
                             */ 
}
// -------------------------------sendframe-----------------------
/*---------------------------------------------------------------
typedef struct{
  dword id;
  byte data[8];
  byte length;
  byte prio;
}CANFrame;
-----------------------------------------------------------------*/
int CAN0SendFrame(CANFrame *fra)
{    
  unsigned char index;                          // number for read message
  unsigned char tbsel = 0;                      // symbol for CAN0TBSEL     
  int cnt=0;  
  
  if (!CAN0TFLG) return IS_ERR;                 /* Is Transmit Buffer full?? */            
  CAN0TBSEL = CAN0TFLG;                         /* Select lowest empty buffer */
  tbsel = CAN0TBSEL;		                        /* Backup selected buffer */       
  /* Load Id to IDR Registers */
  *((unsigned long *) ((unsigned long)(&CAN0TXIDR0)))= fra->id;
  if(fra->length>8) fra->length=8;    
  for (index=0;index<fra->length;index++) 
  {
     *(&CAN0TXDSR0 + index) = (fra->data)[index];   //  Load data to Tx buffer  Data Segment Registers(ONLY 8 BYTES?)                                                   
  }
  CAN0TXDLR  = fra->length;                          /* Set Data Length Code */
  CAN0TXTBPR = fra->prio;                        /* Set Priority */
  CAN0TFLG   = tbsel;	                          /* Start transmission */
  while ((CAN0TFLG & tbsel) != tbsel)
    if(++cnt>10000) return IS_ERR;           //  Wait for Transmission completion
  return NO_ERR;
}

// ----------------------initial CAN4rig--------------------
void InitCAN4(void) 
{
  CAN4CTL0 = 0x01;                                       
  while(CAN4CTL1_INITAK!=1); 	     // Wait for Initialization Mode acknowledge INITRQ bit = 1  
  CAN4CTL1 = 0x80;             
  CAN4BTR0 = 0x43;             
  CAN4BTR1 = 0x14;	                                      
  CAN4IDAC = 0x00;                 // two 32-bit Acceptance Filters                        
  /* note Acceptance Filters neither Acceptance Filter is accorded with,message in receivebuffer will pass  */   
  CAN4IDAR0 = ACC_CAN4_EX_ID_R0;   // |\    32 bit Filter 0
  CAN4IDAR1 = ACC_CAN4_EX_ID_R1;   // | \__ Accepts Extended Data Frame Msg 
  CAN4IDAR2 = ACC_CAN4_EX_ID_R2;   // | /   with ID: CAN0_ID 
  CAN4IDAR3 = ACC_CAN4_EX_ID_R3;   // |/
  CAN4IDAR4 = ACC_CAN4_EX_ID_R0;   // |\    32 bit Filter 0
  CAN4IDAR5 = ACC_CAN4_EX_ID_R1;   // | \__ Accepts Extended Data Frame Msg 
  CAN4IDAR6 = ACC_CAN4_EX_ID_R2;   // | /   with ID: CAN0_ID 
  CAN4IDAR7 = ACC_CAN4_EX_ID_R3;   // |/  
  CAN4IDMR0 = MASK_CAN4_EX_ID_R0;
  CAN4IDMR1 = MASK_CAN4_EX_ID_R1;
  CAN4IDMR2 = MASK_CAN4_EX_ID_R2;
  CAN4IDMR3 = MASK_CAN4_EX_ID_R3;
  CAN4IDMR4 = MASK_CAN4_EX_ID_R0;
  CAN4IDMR5 = MASK_CAN4_EX_ID_R1;
  CAN4IDMR6 = MASK_CAN4_EX_ID_R2;
  CAN4IDMR7 = MASK_CAN4_EX_ID_R3;
    
  CAN4CTL0 = 0x00;                /* Exit Initialization Mode Request */
  while ((CAN4CTL1&0x01)!=0);	    // µÈ´ý£¬Ö±µ½MSCANÕý³£ÔËÐÐ
  while(!(CAN4CTL0&0x10));        // µÈ´ý£¬Ö±µ½MSCANÓëCAN×ÜÏßÍ¬²½
  CAN4RFLG = 0xC3;          
  CAN4RIER = 0x01;          
}
// -------------------------------sendframe-----------------------
/*---------------------------------------------------------------
typedef struct{
  dword id;
  byte data[8];
  byte length;
  byte prio;
}CANFrame;
-----------------------------------------------------------------*/
int CAN4SendFrame(CANFrame *fra)
{    
  unsigned char index;                          // number for read message
  unsigned char tbsel = 0;                      // symbol for CAN4TBSEL 
  int cnt=0;    
  
  if (!CAN4TFLG) return IS_ERR;                 /* Is Transmit Buffer full?? */            
  CAN4TBSEL = CAN4TFLG;                         /* Select lowest empty buffer */
  tbsel = CAN4TBSEL;		                        /* Backup selected buffer */       
  /* Load Id to IDR Registers */
  *((unsigned long *) ((unsigned long)(&CAN4TXIDR0)))= fra->id;//½ÓÊÕ·½
  if(fra->length>8) fra->length=8;    
  for (index=0;index<fra->length;index++) 
  {
     *(&CAN4TXDSR0 + index) = (fra->data)[index];   //  Load data to Tx buffer  Data Segment Registers(ONLY 8 BYTES?)                                                   
  }
  CAN4TXDLR  = fra->length;                          /* Set Data Length Code */
  CAN4TXTBPR = fra->prio;                        /* Set Priority */
  CAN4TFLG   = tbsel;	                          /* Start transmission */
  //printp("\nCAN4TXIDR0~CAN4TXIDR3:%04x,%04x,%04x,%04x",CAN4TXIDR0,CAN4TXIDR1,CAN4TXIDR2,CAN4TXIDR3) ;
  while ((CAN4TFLG & tbsel) != tbsel)
    if(++cnt>10000) return IS_ERR;          //  Wait for Transmission completion
  //putstr("\nCAN4 Sending is successful!");
  return NO_ERR;
}

void CAN0ReadData(CANFrame *fra)
{
   byte index,length;
   length=fra->length=receiveBuffer0[0];
   for(index=0;index<length;++index)
   {
      (fra->data)[index]=receiveBuffer0[index+1];
   } 
   RB0status=READDATA;
}

void CAN4ReadData(CANFrame *fra)
{
   byte index,length;
   length=fra->length=receiveBuffer4[0];
   for(index=0;index<length;++index)
   {
      (fra->data)[index]=receiveBuffer4[index+1];
   }
   RB4status=READDATA; 
}


#pragma CODE_SEG __NEAR_SEG NON_BANKED 
void interrupt VectorNumber_Vcan0rx  CAN0RxISR(void)
{
   byte length,index;
   if(RB0status==NODATA||RB0status==READDATA) 
   {
      RB0status=NEWDATA;
   } 
   else
   {
      RB0status=OVERRUN;
   }
   length=receiveBuffer0[0]=(CAN0RXDLR & 0x0F); //length
   for(index=0;index<length;++index) 
   {
      receiveBuffer0[index+1]=*(&CAN0RXDSR0 + index);
   }
   CAN0RFLG = 0x01;
}
void interrupt VectorNumber_Vcan4rx  CAN4RxISR(void)
{
   byte length,index;
   if(RB4status==NODATA||RB4status==READDATA) 
   {
      RB4status=NEWDATA;
   } 
   else
   {
      RB4status=OVERRUN;
   }
   length=receiveBuffer4[0]=(CAN4RXDLR & 0x0F); //length
   for(index=0;index<length;++index) 
   {
      receiveBuffer4[index+1]=*(&CAN4RXDSR0 + index);
   }
   CAN4RFLG = 0x01;
} 




