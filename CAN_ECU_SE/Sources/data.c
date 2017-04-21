#include "CAN.h"
#include "SCI.h"

byte receiveBuffer0[9];   //include length and data
byte receiveBuffer4[9];
byte SCI0Buffer[10];   //SCI buffer
volatile byte RB0status=NODATA;  //receive buffer 0 status
volatile byte RB4status=NODATA;
volatile byte SCI0status=EMPTY;  //SCI status