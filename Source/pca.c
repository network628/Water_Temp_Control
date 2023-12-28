
#include "main.h"

// PCA ��������
void PWM_PCA_Init(void)
{

  P_SW1 |= 0X10; // ��������ڵ�P3.5�� P3.5Ҳ�Ǻ���Ŀ��Ƶƿ�	 �ֲ�790
                 // P_SW1��AUXR1��һ���Ĵ��� ���ֲ�790ҳ���Կ���
  // ����� SPI���ߵ���  MOSI-P4^0  MISO-P4^1  SCLK-P4^3	����������AUXR1
  // ��P_SW1.4 P_SW1.5���ǵ��� PCA���������IO����� ��������һ��Ҫע�� �ǻ����

  CCON = 0;    // PCA���ƼĴ���				   �ֲ�785
  CL = 0;      // PCA ��������λ 	  ���ü�������ʼֵ 	 ����ͼ796ҳ
               // ͼ�п��Կ��� PWMֻ�õ���CL���ۼӼ����� ����CCAPnL���бȽ�
  CH = 0;      // PCA ��������λ
  CMOD = 0x02; // ����PCAʱ��Դ   ��Ƶ2��Ƶ	   �ֲ�784

  PCA_PWM0 = 0X00;        // 8λPWM���   ��P_SW1|=0X10����   �����P3.5
  CCAP0H = CCAP0L = 0xFE; // ռ�ձ�	 CL�ۼ�ֵ��CCAP0L�Ƚ� CL���ڵ���CCAP0L���1  ����CCAP0L���0
  CCAPM0 = 0x42;          // PCAģ��0Ϊ8λpwmģʽ			   �ֲ�785

  //  PCA_PWM1=0X40;			 //7λPWM���    ��P_SW1|=0X10���� �����P3.6
  //  CCAP1H=CCAP1L=0x20;		 //ռ�ձ�
  //  CCAPM1=0x42;				 //PCAģ��1Ϊ7λpwmģʽ
  //
  //  PCA_PWM2=0X80;			 //8λPWM���	 ��P_SW1|=0X10���� �����P3.7
  //  CCAP2H=CCAP2L=0x20;		 //ռ�ձ�
  //  CCAPM2=0x42;				 //PCAģ��2Ϊ6λpwmģʽ

  CR = 1; // ʹ��PCA
}

void PWM_Out(unsigned char cycle)
{
  U8 t; // ��ʱ����
  cycle = (float)(2.55 * cycle);
  t = 255 - cycle;     // ʹP3.5�ڵĵ����ȷ����仯
  CCAP0H = CCAP0L = t; // ��������ֵ��ռ�ձȴ洢�ռ�
  //	CCAP1H=CCAP1L=t;		 //��������ֵ��ռ�ձȴ洢�ռ�
  //	CCAP2H=CCAP2L=t;		 //��������ֵ��ռ�ձȴ洢�ռ�
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
    ntc.Now_T = Get_Temp(); // ��ȡ�¶�ֵ
    sprintf(disp_temp, "%3.1f", ntc.Now_T);
    display_string_8x16(5, 8 * 8, disp_temp);
  }

  if (pid.set != ntc.Set_T)
  {
    pid.set = ntc.Set_T; // �趨�¶�
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
      pid.curr = ntc.Now_T; // ��ǰ�¶�
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
