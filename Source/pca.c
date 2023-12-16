
#include "main.h"

// PCA 变量函数
void PWM_PCA_Init(void)
{

  P_SW1 |= 0X10; // 配置输出口到P3.5口 P3.5也是黑灵的控制灯口	 手册790
                 // P_SW1和AUXR1是一个寄存器 在手册790页可以看出
  // 黑灵的 SPI总线调到  MOSI-P4^0  MISO-P4^1  SCLK-P4^3	就是利用了AUXR1
  // 而P_SW1.4 P_SW1.5又是调节 PCA输出口所在IO分配的 所以这里一定要注意 是或等于

  CCON = 0;    // PCA控制寄存器				   手册785
  CL = 0;      // PCA 计数器低位 	  配置计数器初始值 	 流程图796页
               // 图中可以看出 PWM只用到了CL做累加计数器 并和CCAPnL进行比较
  CH = 0;      // PCA 计数器高位
  CMOD = 0x02; // 设置PCA时钟源   主频2分频	   手册784

  PCA_PWM0 = 0X00;        // 8位PWM输出   按P_SW1|=0X10设置   输出在P3.5
  CCAP0H = CCAP0L = 0xFE; // 占空比	 CL累加值和CCAP0L比较 CL高于等于CCAP0L输出1  低于CCAP0L输出0
  CCAPM0 = 0x42;          // PCA模块0为8位pwm模式			   手册785

  //  PCA_PWM1=0X40;			 //7位PWM输出    按P_SW1|=0X10设置 输出在P3.6
  //  CCAP1H=CCAP1L=0x20;		 //占空比
  //  CCAPM1=0x42;				 //PCA模块1为7位pwm模式
  //
  //  PCA_PWM2=0X80;			 //8位PWM输出	 按P_SW1|=0X10设置 输出在P3.7
  //  CCAP2H=CCAP2L=0x20;		 //占空比
  //  CCAPM2=0x42;				 //PCA模块2为6位pwm模式

  CR = 1; // 使能PCA
}

void PWM_Out(unsigned char cycle)
{
  U8 t; // 临时变量
  cycle = (float)(2.55 * cycle);
  t = 255 - cycle;     // 使P3.5口的灯亮度发生变化
  CCAP0H = CCAP0L = t; // 将变量赋值到占空比存储空间
  //	CCAP1H=CCAP1L=t;		 //将变量赋值到占空比存储空间
  //	CCAP2H=CCAP2L=t;		 //将变量赋值到占空比存储空间
}

struct NTC ntc;
U8 SW_Flag = 1;
char disp_temp[16];
void Cool_handle(void)
{
  static u8 Sec_Cmp;
  if (ntc.Tick > 50) // 500ms
  {
    ntc.Tick = 0;
    ntc.Now_T = Get_Temp(); // 获取温度值
    sprintf(disp_temp, "%3.1f", ntc.Now_T);
    display_string_8x16(5, 8 * 8, disp_temp);
  }

  if (pid.set != ntc.Set_T)
  {
    pid.set = ntc.Set_T; // 设定温度
    sprintf(disp_temp, "%3.1f", ntc.Set_T);
    display_string_8x16(3, 8 * 8, disp_temp);
  }

  if (Sec_Cmp != ntc.Second) // 1s
  {
    Sec_Cmp = ntc.Second;
    sprintf(disp_temp, "%02d:%02d:%02d", ntc.Hour, ntc.Min, ntc.Second);
    display_string_8x16(1, 8 * 6, disp_temp);

    if (pid.curr != ntc.Now_T)
    {
      pid.curr = ntc.Now_T; // 当前温度
    }

    pid_calc(); // pid??
    if (SW_Flag)
    {
      ntc.Cool_PWM = pid.currpwm;
      PWM_Out(ntc.Cool_PWM); // PWM(0<--->100)
      sprintf(disp_temp, "%3d", ntc.Cool_PWM);
      display_string_8x16(7, 8 * 9, disp_temp);
    }
    else
    {
      PWM_Out(0); // PWM(0<--->100)
      display_string_8x16(7, 8 * 9, "---");
    }
  }
}
