#include "CAN.h"
#include "SCI.h"
#include "data.h"

#define ID_GW 0
#define ID_ECU_1 1
#define ID_CAN0 0x888e
#define ID_CAN4 0x887e
byte receiveBuffer0[9];   //include length and data
byte receiveBuffer4[9];
byte SCI0RxBuffer[8];   //SCI receive buffer
byte SCI0TxBuffer[9];   
volatile byte RB0status=NODATA;  //receive buffer 0 status
volatile byte RB4status=NODATA;
volatile byte SCI0status=EMPTY;  //SCI status
