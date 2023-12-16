#include "pid.h"
#include "main.h"	

PID pid;
//extern u16 pidcalcms;
void PIDParament_Init()  //
{
  ntc.Set_T =Read_Set_T();
	// pid.set = ntc.Set_T;  //设定温度
	pid.pwm_cycle=100;    ////pwm的周期
	pid.pid_P=5;
	pid.pid_I=4;
	pid.pid_D=3;
//	pid.calc_cycle=50;  //每隔50*10mS运算一次
}
  
void pid_calc(void)  //pid??
{
  //int dk;

	
  short PID_out;
	
//	if(pidcalcms<pid.calc_cycle) return ;

//  pid.Ek=pid.set-pid.curr;//????(????-????)
	pid.Ek=pid.curr-pid.set;//????(????-????)
  PID_out=((pid.pid_P)*((pid.Ek)-(pid.Ek_1)))+ \
          ((pid.pid_I)*(pid.Ek))+ \
          ((pid.pid_D)*((pid.Ek)-(2*(pid.Ek_1))+(pid.Ek_2)));   //??????
  pid.currpwm+=PID_out;
  pid.Ek_2=pid.Ek_1;  //???????
  pid.Ek_1=pid.Ek;    //??????
//  printf("--->本次应该输出的PWM---> %d %d\r\n", pid.currpwm ,PID_out);
  if(pid.currpwm<=0)  //??????0??0??(????)
  {
      pid.currpwm=0;// 
  }  //PID????
  else if(pid.currpwm>pid.pwm_cycle)  //1000    //1s??PWM??
  {
     pid.currpwm=pid.pwm_cycle;   //????????1000???????
  }
   	 
// pidcalcms=0;
//	return pid.currpwm;
} 


//void pid_calc()  //pid??
//{
//  float dk1;float dk2;
//  float t1,t2,t3;
//	
//  //short PID_out;
//	
//	if(pidcalcms<pid.Tsam) return ;  //
//	pid.En=pid.set-pid.curr;  //本次误差
//	dk1=pid.En-pid.En_1;   //本次偏差与上次偏差之差
//	dk2=pid.En-2*pid.En_1+pid.En_2;
//	
//  t1=pid.Kp*dk1;
//	
//	t2=(pid.Kp*pid.Tsam)/pid.Ti;
//	t2=t2*pid.En;
//	
//	t3=(pid.Kp*pid.Td)/pid.Tsam;
//	t3=t3*dk2;
//	
//  pid.Dout=t1+t2+t3;  //本次应该输出的增量
//	
//	pid.currpwm+=pid.Dout;  //本次应该输出的PWM  //当前的pwm宽度
// 	printf("--->本次应该输出的PWM---> %d\r\n", pid.currpwm);
//	if(pid.currpwm>pid.pwm_cycle)
//	{
//	 pid.currpwm=pid.pwm_cycle;
//	}
//	if(pid.currpwm<0)
//	{
//	 
//		pid.currpwm=0;
//	}
//	pid.En_2=pid.En_1;
//	pid.En_1=pid.En;

//  pidcalcms=0;
//}
