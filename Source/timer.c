
#include "main.h"

//-----------------------------------------------

#define FOSC 11059200L
#define T0MS (65536 - FOSC / 1015) // 1T模式   +快些  -慢些	 1005
// #define T2MS (65536 - FOSC / 1000) // 1T模式

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

		Key_Scan_Cnt = 1; // 定时器变量10ms执行一次
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
////中断服务程序
// void t2int() interrupt 12           //中断入口
//{
////    P10 = !P10;                     //将测试口取反
////    delay(1000);
////  IE2 &= ~0x04;                   //若需要手动清除中断标志,可先关闭中断,此时系统会自动清除内部的中断标志
////  IE2 |= 0x04;                    //然后再开中断即可
//}

void Timer0_Init(void)
{
	AUXR |= 0x80; // 定时器0为1T模式
	//  AUXR &= 0x7f;                   //定时器0为12T模式

	TMOD = 0x00; // 设置定时器为模式0(16位自动重装载)
	TL0 = T0MS;	 // 初始化计时值
	TH0 = T0MS >> 8;
	//	TL0 = 0x3c;                     //初始化计时值
	//    TH0 = 0xb0;
	TR0 = 1; // 定时器0开始计时
	ET0 = 1; // 使能定时器0中断
	EA = 1;
}

// void Timer2_Init(void)
//{
//     AUXR |= 0x04;                   //定时器2为1T模式
////  AUXR &= ~0x04;                  //定时器2为12T模式
//    T2L = T2MS;                     //初始化计时值
//    T2H = T2MS >> 8;
//    AUXR |= 0x10;                   //定时器2开始计时
//
//    IE2 |= 0x04;                    //开定时器2中断
//    EA = 1;
//}
