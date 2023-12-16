#ifndef _pid_
#define _pid_
 

typedef struct
{
	float curr;  //当前温度
	float set;   //设定温度
	
	unsigned int calc_cycle;
	short currpwm;  //当前的pwm宽度
	unsigned int pwm_cycle;//pwm的周期
	
	float pid_P;       //P
	float pid_I;       //I
	float pid_D;       //D
	
	short Ek;//
	short Ek_1;//
	short Ek_2;//
	int PID_OUT;//
	
}PID;


extern unsigned char STATUS;
extern PID pid;
void PIDParament_Init(void);  //
void pid_calc(void);  //pid??

#endif

