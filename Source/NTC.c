/***************************************/
/*   基于STC15W408AS的NTC温度计设计    */
/*       测温范围-40°～110°          */
/*       创建者 ：jmpxwh               */
/*      创建时间：2017/05/05           */
/*      请到https://oimin.com下载       */
/***************************************/
// 4位共阴极数码管段码a-h顺序接P27-P20，位选端接P3高4位
// NTC使用MF52-103/3435;10K±1％精度;B值:3435±1％
// NTC温度输出端子   接P1.7
#include "main.h"

#define ADC_POWER 0x80	// ADC电源控制位
#define ADC_FLAG 0x10	// ADC完成标志位
#define ADC_START 0x08	// ADC启动控制位
#define ADC_SPEED0 0x00 // ADC转换速度，一次转换需要540个时钟
#define ADC_SPEED1 0x20 // ADC转换速度，一次转换需要360个时钟
#define ADC_SPEED2 0x40 // ADC转换速度，一次转换需要180个时钟
#define ADC_SPEED3 0x60 // ADC转换速度，一次转换需要90个时钟

/*************************************
NTC阻值与温度关系表
Vcc---NTC(±1％)---R(10K±1％)---GND
				|
			  ADC端子
NTC使用MF52-103/3435;10K±1％精度;B值:3435±1％
**************************************/
unsigned int code NTCcode[] = {
	/*ADC采样值****温度值*******电阻值**********/
	// 0x033,	// -40.00		190.5562
	// 0x034,	// -39.00		183.4132
	// 0x037,	// -38.00		175.6740
	// 0x039,	// -37.00		167.6467
	// 0x03c,	// -36.00		159.5647
	// 0x03f,	// -35.00		151.5975
	// 0x042,	// -34.00		143.8624
	// 0x045,	// -33.00		136.4361
	// 0x049,	// -32.00		129.3641
	// 0x04d,	// -31.00		122.6678
	// 0x050,	// -30.00		116.3519
	// 0x054,	// -29.00		110.4098
	// 0x059,	// -28.00		104.8272
	// 0x05d,	// -27.00		99.5847
	// 0x061,	// -26.00		94.6608
	// 0x066,	// -25.00		90.0326
	// 0x06a,	// -24.00		85.6778
	// 0x06f,	// -23.00		81.5747
	// 0x074,	// -22.00		77.7031
	// 0x079,	// -21.00		74.0442
	// 0x07e,	// -20.00		70.5811
	// 0x084,	// -19.00		67.2987
	// 0x089,	// -18.00		64.1834
	// 0x08f,	// -17.00		61.2233
	// 0x095,	// -16.00		58.4080
	// 0x09b,	// -15.00		55.7284
	// 0x0a1,	// -14.00		53.1766
	// 0x0a8,	// -13.00		50.7456
	// 0x0af,	// -12.00		48.4294
	// 0x0b5,	// -11.00		46.2224
	// 0x0bd,	// -10.00		44.1201
	// 0x0c4,	// -9.00		42.1180
	// 0x0cb,	// -8.00		40.2121
	// 0x0d3,	// -7.00		38.3988
	// 0x0db,	// -6.00		36.6746
	// 0x0e3,	// -5.00		35.0362
	// 0x0eb,	// -4.00		33.4802
	// 0x0f3,	// -3.00		32.0035
	// 0x0fb,	// -2.00		30.6028
	// 0x0104, // -1.00		29.2750
	0x010d, // 0.00		28.0170
	0x0115, // 1.00		26.8255
	0x011e, // 2.00		25.6972
	0x0127, // 3.00		24.6290
	0x0130, // 4.00		23.6176
	0x0139, // 5.00		22.6597
	0x0142, // 6.00		21.7522
	0x014b, // 7.00		20.8916
	0x0154, // 8.00		20.0749
	0x015d, // 9.00		19.2988
	0x0166, // 10.00		18.5600
	0x0167, // 11.00		18.4818
	0x016b, // 12.00		18.1489
	0x0172, // 13.00		17.6316
	0x017b, // 14.00		16.9917
	0x0185, // 15.00		16.2797
	0x0190, // 16.00		15.5350
	0x019c, // 17.00		14.7867
	0x01a9, // 18.00		14.0551
	0x01b6, // 19.00		13.3536
	0x01c2, // 20.00		12.6900
	0x01cf, // 21.00		12.0684
	0x01dc, // 22.00		11.4900
	0x01e8, // 23.00		10.9539
	0x01f4, // 24.00		10.4582
	0x01ff, // 25.00		10.0000
	0x020a, // 26.00		9.5762
	0x0215, // 27.00		9.1835
	0x021f, // 28.00		8.8186
	0x0229, // 29.00		8.4784
	0x0233, // 30.00		8.1600
	0x023c, // 31.00		7.8608
	0x0245, // 32.00		7.5785
	0x024e, // 33.00		7.3109
	0x0257, // 34.00		7.0564
	0x0260, // 35.00		6.8133
	0x0268, // 36.00		6.5806
	0x0271, // 37.00		6.3570
	0x0279, // 38.00		6.1418
	0x0282, // 39.00		5.9343
	0x028a, // 40.00		5.7340
	0x0292, // 41.00		5.5405
	0x029a, // 42.00		5.3534
	0x02a2, // 43.00		5.1725
	0x02aa, // 44.00		4.9976
	0x02b1, // 45.00		4.8286
	0x02b9, // 46.00		4.6652
	0x02c1, // 47.00		4.5073
	0x02c8, // 48.00		4.3548
	0x02d0, // 49.00		4.2075
	0x02d7, // 50.00		4.0650
	0x02de, // 51.00		3.9271
	0x02e5, // 52.00		3.7936
	0x02ec, // 53.00		3.6639
	0x02f3, // 54.00		3.5377
	0x02fa, // 55.00		3.4146
	0x0301, // 56.00		3.2939
	0x0308, // 57.00		3.1752
	0x030f, // 58.00		3.0579
	0x0316, // 59.00		2.9414
	0x031d, // 60.00		2.8250
	0x0320, // 61.00		2.7762
	0x0324, // 62.00		2.7179
	0x0328, // 63.00		2.6523
	0x032d, // 64.00		2.5817
	0x0331, // 65.00		2.5076
	0x0336, // 66.00		2.4319
	0x033b, // 67.00		2.3557
	0x0341, // 68.00		2.2803
	0x0346, // 69.00		2.2065
	0x034b, // 70.00		2.1350
	0x034f, // 71.00		2.0661
	0x0354, // 72.00		2.0004
	0x0358, // 73.00		1.9378
	0x035d, // 74.00		1.8785
	0x0361, // 75.00		1.8225
	0x0365, // 76.00		1.7696
	0x0368, // 77.00		1.7197
	0x036c, // 78.00		1.6727
	0x036f, // 79.00		1.6282
	0x0372, // 80.00		1.5860
	0x0376, // 81.00		1.5458
	0x0378, // 82.00		1.5075
	0x037b, // 83.00		1.4707
	0x037e, // 84.00		1.4352
	0x0381, // 85.00		1.4006
	0x0383, // 86.00		1.3669
	0x0386, // 87.00		1.3337
	0x0389, // 88.00		1.3009
	0x038b, // 89.00		1.2684
	0x038e, // 90.00		1.2360
	0x0391, // 91.00		1.2037
	0x0393, // 92.00		1.1714
	0x0396, // 93.00		1.1390
	0x0399, // 94.00		1.1067
	0x039b, // 95.00		1.0744
	0x039e, // 96.00		1.0422
	0x03a1, // 97.00		1.0104
	0x03a3, // 98.00		0.9789
	0x03a6, // 99.00		0.9481
	0x03a8, // 100.00		0.9180
	0x03ab, // 101.00		0.8889
	0x03ad, // 102.00		0.8610
	0x03b0, // 103.00		0.8346
	0x03b2, // 104.00		0.8099
	0x03b4, // 105.00		0.7870
	0x03b6, // 106.00		0.7665
	0x03b7, // 107.00		0.7485
	0x03b9, // 108.00		0.7334
	0x03ba, // 109.00		0.7214
	0x03ba	// 110.00		0.7130
};

unsigned char ADCcount; // 数码管显示计数变量，ADC采样次数变量
/******************************
函数说明：初始化ADC寄存器，设置P1.7为ADC输入功能
入口参数：无
出口参数：无
******************************/
void Init_ADC(void)
{
	P1ASF = 0x01; // 选择P1.0口的哪一口 这里的口和ch要对应才能达到选择该口
	//    P1ASF = 0x80;		//打开P1.7口的ADC功能
	ADC_RES = 0;
	ADC_RESL = 0;						// 清掉ADC转换结果寄存器
	ADC_CONTR = ADC_POWER | ADC_SPEED3; // 使能A/D供电，设置转换速度90T
	ADCcount = 0;						// ADC采样次数
}

/******************************
函数说明：查询方式读取ADC转换结果
入口参数：ch  ADC采样通道
出口参数：int ADC_RES ADC转换结果
******************************/
unsigned int Get_ADC_Result(unsigned char ch)
{
	unsigned int result;
	ADC_RES = 0;
	ADC_RESL = 0;										 // 清掉ADC转换结果寄存器
	ADC_CONTR = ADC_POWER | ADC_SPEED3 | ch | ADC_START; // 配置ADC，设置转换通道，启动转换
	_nop_();
	_nop_();
	_nop_();
	_nop_(); // 等待设置ADC_POWER完毕
	while (!(ADC_CONTR & ADC_FLAG))
		;							  // 读取转换完毕标志位ADC_FLAG
	ADC_CONTR &= ~ADC_FLAG;			  // 清除ADC_FLAG标志位
	result = ADC_RES << 2 | ADC_RESL; // 读取10位转换结果保存到result
	return result;					  // 返回ADC转换结果10位
}

/****************************************************************************/
// 过程噪声和测量噪声的方差
float KQ = 0.01;							  // 过程噪声方差
float KR = 0.1;								  // 测量噪声方差
float NTC_Kalman_Filtering(uint16_t newValue) // 692byte 4096
{
	// 卡尔曼滤波器的预测步骤
	// 定义卡尔曼滤波器的状态变量和协方差
	static float Kx_hat = 0; // 估计的状态值
	static float KP1 = 1;	 // 估计误差的协方差
	float KK1;
	short ntc_value;
	// 控制向量 u 影响状态的演变
	// 在这个示例中，我们假设控制向量表示外部输入，例如速度
	// 预测下一个状态值，考虑控制向量
	// x_hat = x_hat + u;
	// 更新估计误差的协方差
	KP1 = KP1 + KQ;

	// 计算卡尔曼增益
	KK1 = KP1 / (KP1 + KR);
	// 更新状态估计
	ntc_value = (newValue - Kx_hat);

	Kx_hat = Kx_hat + KK1 * ntc_value;
	// 更新估计误差的协方差
	KP1 = (1 - KK1) * KP1;

	return Kx_hat;
}

/******************************
函数说明：获取温度值
入口参数：无
出口参数：无
******************************/
// #define N 16	//ADC采样使用递推平均滤波算法，采样次数
// unsigned int  Temp_Buf[N+1];// 采样数组
float Get_Temp(void)
{
	float tempvalue;
	unsigned char xx;

	// unsigned int sum;
	// sum = tempvalue =0;
	// Temp_Buf[N]=Get_ADC_Result(0);	//将ADC转换结果放数组最高位
	// if( ++ADCcount < 16)		//采样初期不使用滤波算法
	// {
	// 	for(xx=0;xx<N;xx++)	//准备滤波算法的数据
	// 	{
	// 		Temp_Buf[xx]=Temp_Buf[xx+1];//所有数据循环左移
	// 	}
	// 	tempvalue=Temp_Buf[N];//采样初期使用当前采样值
	// }
	// else 	//只有采样次数大于8次以后才使用滤波算法
	// {
	// 	ADCcount=16;	//采样次数超过8次后，固定设置为8
	// 	for(xx=0;xx<N;xx++)	//滤波算法
	// 	{
	// 		Temp_Buf[xx]=Temp_Buf[xx+1];//所有数据循环左移
	// 		sum+=Temp_Buf[xx];	//求和
	// 	}
	// 	tempvalue=sum/N;		//求平均值
	// }

	// 读取ADC值
	uint16_t NTC_adcValue = Get_ADC_Result(0); // 将ADC转换结果放数组最高位
	tempvalue = NTC_Kalman_Filtering(NTC_adcValue);

	// printf("T: %d", tempvalue);
	//*********************************************************
	xx = 0;
	while (tempvalue > NTCcode[xx]) // 将当前温度值与温度表对比，得到所在位置
	{
		xx++;
	}

	tempvalue = xx * 10 + (tempvalue * 10 - NTCcode[xx - 1] * 10) / (NTCcode[xx] - NTCcode[xx - 1]);
	// 插值法计算温度，将数据全部扩大10倍，使小数变为整数
	// tempvalue=tempvalue-30;//修正温度，比正常高3.0度(扩大10倍为30)

	// if (tempvalue > 400) // 正温度(大于0度)
	// {
	// 	tempvalue = tempvalue - 400; // 取得0°以上温度
	// }
	// else // 负温度(小于0度)
	// {
	// 	tempvalue = 410 - tempvalue; // 取得0°以下温度
	// }
	tempvalue /= 10;
	return tempvalue;
}
