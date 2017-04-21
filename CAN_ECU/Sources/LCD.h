#include "derivative.h"

extern void LCD_Init(void);
extern void LCD_CLS(void);
extern void LCD_P6x8Str(byte x,byte y,byte ch[],byte cnt);
extern void LCD_P8x16Str(byte x,byte y,byte ch[]);
extern void char2hex(byte *ch,byte *hex,byte cnt);

