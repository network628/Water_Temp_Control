#ifndef     __MAIN_H__
#define     __MAIN_H__

#include"STC15W.h"
#include <intrins.h>
#include <string.h>
#include <stdio.h>
#include"SH1106_12864.h"
#include "pca.h"
#include "NTC.h"
#include "timer.h"
#include "Uart.h"
#include "stceeprom.h"
#include "pid.h"
#include "key.h"

#ifndef TURE
 #define TRUE              1
#endif

#ifndef FALSE
 #define FALSE             0
#endif


typedef unsigned char   U8;
typedef signed   char   S8;
typedef unsigned int    U16;
typedef signed   int    S16;
typedef unsigned long   U32;
typedef signed   long   S32;

#define uint8_t unsigned char
#define u8 unsigned char
#define u16 unsigned int
#define uint16_t unsigned int
#define u32 unsigned int
 
sbit SCL=P3^7;  	//接口定义:SCL就是LCD的SCL   //SCL接到“D0”脚
sbit SDA=P3^6;   	//接口定义:SDA就是LCD的SDA	 //SDA接到“D1”脚

sbit led = P5^5; 

sbit KEY_SW = P1^6;	//MENU P1.6
sbit KEY_Up	= P1^7; //KEY_Up P1.7
sbit KEY_Dowm = P5^4;	//KEY_Dowm P5.4
 
extern U8 SW_Flag;
extern char disp_temp[16];
extern struct NTC{

 	U16 Hour,Min,Second;
    U16 T0_CNT;		//计数器
	float Set_T;    //设置温度
	float Now_T;	//当前温度
 	short Cool_PWM;
	U16 Tick;
}ntc;


#endif





