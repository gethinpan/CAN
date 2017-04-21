#include "ADC.h"

void ADC_Init(void)
{
   ATD0CTL1=0x40;   //7:1-�ⲿ����,65:00-8λ����,4:�ŵ�,3210:ch
   ATD0CTL2=0x40;   //��ֹ�ⲿ����, �жϽ�ֹ    
   ATD0CTL3=0xa0;   //�Ҷ����޷���,ÿ��ת��4������, No FIFO, Freezeģʽ�¼���ת    
   ATD0CTL4=0x01;   //765:����ʱ��Ϊ4��ADʱ������,ATDClock=[BusClock*0.5]/[PRS+1]
   ATD0CTL5=0x30;   //6:0����ͨ����ֹ,5:1����ת�� ,4:1��ͨ����������
   ATD0DIEN=0x00;   //��ֹ�������� 
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