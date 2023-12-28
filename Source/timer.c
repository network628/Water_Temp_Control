
#include "main.h"

//-----------------------------------------------

#define FOSC 11059200L
#define T0MS (65536 - FOSC / 1015) // 1Tģʽ   +��Щ  -��Щ	 1005
// #define T2MS (65536 - FOSC / 1000) // 1Tģʽ

#define TIME_BASE_10MS 10
u8 time_base_10ms = 10;
//-----------------------------------------------

/* Timer0 interrupt routine */
void tm0_isr() interrupt 1
{

	--time_base_10ms;
	if (time_base_10ms == 0)
	{
		time_base_10ms = TIME_BASE_10MS;

		Key_Scan_Cnt = 1; // ��ʱ������10msִ��һ��
		ntc.Tick++;
		// if (ntc_cnt > 0) //
		// 	--ntc_cnt;
	}

	ntc.T0_CNT++;
	if (ntc.T0_CNT > 1000)
	{
		ntc.T0_CNT = 0;

		ntc.Second++;

		led = ~led;
		if (ntc.Second > 60)
		{
			ntc.Second = 0;
			ntc.Min++;
			if (ntc.Min > 60)
			{
				ntc.Min = 0;
				ntc.Hour++;
				if (ntc.Hour > 24)
				{
					ntc.Hour = 0;
				}
			}
		}
	}
}

////-----------------------------------------------
////�жϷ������
// void t2int() interrupt 12           //�ж����
//{
////    P10 = !P10;                     //�����Կ�ȡ��
////    delay(1000);
////  IE2 &= ~0x04;                   //����Ҫ�ֶ�����жϱ�־,���ȹر��ж�,��ʱϵͳ���Զ�����ڲ����жϱ�־
////  IE2 |= 0x04;                    //Ȼ���ٿ��жϼ���
//}

void Timer0_Init(void)
{
	AUXR |= 0x80; // ��ʱ��0Ϊ1Tģʽ
	//  AUXR &= 0x7f;                   //��ʱ��0Ϊ12Tģʽ

	TMOD = 0x00; // ���ö�ʱ��Ϊģʽ0(16λ�Զ���װ��)
	TL0 = T0MS;	 // ��ʼ����ʱֵ
	TH0 = T0MS >> 8;
	//	TL0 = 0x3c;                     //��ʼ����ʱֵ
	//    TH0 = 0xb0;
	TR0 = 1; // ��ʱ��0��ʼ��ʱ
	ET0 = 1; // ʹ�ܶ�ʱ��0�ж�
	EA = 1;
}

// void Timer2_Init(void)
//{
//     AUXR |= 0x04;                   //��ʱ��2Ϊ1Tģʽ
////  AUXR &= ~0x04;                  //��ʱ��2Ϊ12Tģʽ
//    T2L = T2MS;                     //��ʼ����ʱֵ
//    T2H = T2MS >> 8;
//    AUXR |= 0x10;                   //��ʱ��2��ʼ��ʱ
//
//    IE2 |= 0x04;                    //����ʱ��2�ж�
//    EA = 1;
//}
