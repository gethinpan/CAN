#include "ADC.h"

void ADC_Init(void)
{
   ATD0CTL1=0x40;   //7:1-外部触发,65:00-8位精度,4:放电,3210:ch
   ATD0CTL2=0x40;   //禁止外部触发, 中断禁止    
   ATD0CTL3=0xa0;   //右对齐无符号,每次转换4个序列, No FIFO, Freeze模式下继续转    
   ATD0CTL4=0x01;   //765:采样时间为4个AD时钟周期,ATDClock=[BusClock*0.5]/[PRS+1]
   ATD0CTL5=0x30;   //6:0特殊通道禁止,5:1连续转换 ,4:1多通道轮流采样
   ATD0DIEN=0x00;   //禁止数字输入 
}

byte ReadATD(byte ch)
{
  byte ad=0;
  while(!ATD0STAT0_SCF);
  switch(ch)
  {
    default:
    case 0:
      ad= ATD0DR0L;
    break;
    case 1:
      ad= ATD0DR1L;
    break;
    case 2:
      ad= ATD0DR2L;
    break;
    case 3:
      ad= ATD0DR3L;
    break;
    case 4:
      ad= ATD0DR4L;
    break;
    case 5:
      ad= ATD0DR5L;
    break;
    case 6:
      ad= ATD0DR6L;
    break;
    case 7:
      ad= ATD0DR7L;
    break;   
  }
  return ad;
}