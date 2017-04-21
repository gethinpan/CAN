#ifndef _CAN_H
#define _CAN_H

#include "derivative.h"

#ifndef NO_ERR
  #define NO_ERR 0x00
  #define IS_ERR 0x01
#endif

typedef struct{
  dword id;
  byte data[9];
  byte length;
  byte prio;
}CANFrame;

/*define status*/
enum status
{ NODATA,      /* 0 */
  NEWDATA,
  READDATA,
  OVERRUN
};

extern byte receiveBuffer0[9];   //include length and data
extern byte receiveBuffer4[9];
extern volatile byte RB0status;  //receive buffer 0 status
extern volatile byte RB4status;

extern unsigned long CAN0_ID;
extern unsigned long CAN4_ID;


extern unsigned long  ConvertID2MSCAN_ID(unsigned long OID);
extern int  CAN0SendFrame(CANFrame*);
extern int  CAN4SendFrame(CANFrame*);
extern void InitCAN0(void);
extern void InitCAN4(void);
extern void CAN0ReadData(CANFrame*);
extern void CAN4ReadData(CANFrame*);

#endif