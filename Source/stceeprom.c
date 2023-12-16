 
#include "main.h"
#include <intrins.H>
 
//===================================================================================
//                   用户接口函数
//
// 由于STC12C5A60S2 EEPROM区的特殊性, 只能一整扇区(512字节)写,不能单字节更改.
// 特加入以下两个函数, 以支持单字节更改
// 用户无需关心扇区擦除等操作 但额外需要一扇区作为缓存以减少RAM占用(时间换空间^_^)
// STC12C5A60S2 EEPROM区为1024字节, 共2个扇区 每扇区512字节
// 去除用作缓存的第二扇区,用户可用数据为512个 地址范围:0-511
//===================================================================================
#define MaxEepromNum    512

// 读取STC EEPROM
U8 STC_EE_Read(U16 Addr, U8 *Buf, U16 Num)
{
    U16 i;
    // 参数检测
    if((Addr+Num) > MaxEepromNum)return FALSE;

    for(i=0;i<Num;i++)
    {
        Buf[i] = Byte_Read(Addr+i);
    }
    return TRUE;
}


// 写入STC EEPROM
U8 STC_EE_Write(U16 Addr, U8 *Buf, U16 Num)
{
    U16 i;
    U8  tmp;

    // 参数检测
    if((Addr+Num) > MaxEepromNum)return FALSE;

    // 擦除扇区2 
    Sector_Erase(MaxEepromNum);                    // 擦除第2扇区
    // 复制第1扇区数据到第2扇区备份
    for(i=0;i<MaxEepromNum;i++)
    {
        tmp = Byte_Read(i);                 // 读EEPROM的值
        Byte_Program(i+MaxEepromNum, tmp);         // 写入 EEPROM
    }
    // 擦除扇区1 
    Sector_Erase(0x00);                     // 擦除第1扇区
    // 恢复第2扇区数据到第1扇区
    for(i=0;i<Addr;i++)
    {
        tmp = Byte_Read(i+MaxEepromNum);           // 读EEPROM的值
        Byte_Program(i, tmp);               // 写入 EEPROM
    }
    // 写入要保存的数据
    for(i=Addr;i<Addr+Num;i++)
    {
        Byte_Program(i, Buf[i-Addr]);       // 写入 EEPROM
    }
    // 继续恢复第2扇区数据到第1扇区
    for(i=Addr+Num;i<MaxEepromNum;i++)
    {
        tmp = Byte_Read(i+MaxEepromNum);           // 读EEPROM的值
        Byte_Program(i, tmp);               // 写入 EEPROM
    }
    return TRUE;
}





//===================================================================================
//    基础函数 来自宏晶网站
//===================================================================================


//定义Flash 操作等待时间及允许IAP/ISP/EEPROM 操作的常数
//#define ENABLE_ISP 0x80 //系统工作时钟<30MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x81 //系统工作时钟<24MHz 时，对IAP_CONTR 寄存器设置此值
#define ENABLE_ISP 0x82 //系统工作时钟<20MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x83 //系统工作时钟<12MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x84 //系统工作时钟<6MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x85 //系统工作时钟<3MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x86 //系统工作时钟<2MHz 时，对IAP_CONTR 寄存器设置此值
//#define ENABLE_ISP 0x87 //系统工作时钟<1MHz 时，对IAP_CONTR 寄存器设置此值

//#define DEBUG_DATA               0x5A  //本测试程序最终存储在 EEPROM 单元的数值
//#define DATA_FLASH_START_ADDRESS 0x0400  //STC5Axx 系列 EEPROM 测试起始地址

union union_temp16
{
    U16 un_temp16;
    U8  un_temp8[2];
}my_unTemp16;


//读一字节，调用前需打开IAP 功能，入口:DPTR = 字节地址，返回:A = 读出字节
U8 Byte_Read(U16 add)
{
    IAP_DATA = 0x00;
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x01;                 //IAP/ISP/EEPROM 字节读命令

    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    //EA = 0;
    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    return (IAP_DATA);
}

//字节编程，调用前需打开IAP 功能，入口:DPTR = 字节地址, A= 须编程字节的数据
void Byte_Program(U16 add, U8 ch)
{
    IAP_CONTR = ENABLE_ISP;         //打开 IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x02;                 //IAP/ISP/EEPROM 字节编程命令

    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    IAP_DATA = ch;                  //要编程的数据先送进IAP_DATA 寄存器
    //EA = 0;
    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

//擦除扇区, 入口:DPTR = 扇区地址
void Sector_Erase(U16 add)
{
    IAP_CONTR = ENABLE_ISP;         //打开IAP 功能, 设置Flash 操作等待时间
    IAP_CMD = 0x03;                 //IAP/ISP/EEPROM 扇区擦除命令

    my_unTemp16.un_temp16 = add;
    IAP_ADDRH = my_unTemp16.un_temp8[0];    //设置目标单元地址的高8 位地址
    IAP_ADDRL = my_unTemp16.un_temp8[1];    //设置目标单元地址的低8 位地址

    //EA = 0;
    IAP_TRIG = 0x5A;   //先送 5Ah,再送A5h 到ISP/IAP 触发寄存器,每次都需如此
    IAP_TRIG = 0xA5;   //送完A5h 后，ISP/IAP 命令立即被触发起动
    _nop_();
    //EA = 1;
    IAP_Disable();  //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
                    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
}

void IAP_Disable()
{
    //关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
    //一次连续的IAP 操作完成之后建议关闭IAP 功能,不需要每次都关
    IAP_CONTR = 0;      //关闭IAP 功能
    IAP_CMD   = 0;      //清命令寄存器,使命令寄存器无命令,此句可不用
    IAP_TRIG  = 0;      //清命令触发寄存器,使命令触发寄存器无触发,此句可不用
    IAP_ADDRH = 0;
    IAP_ADDRL = 0;
}

void Write_Set_T(float dat) //写入当前的亮度值
{
    U8  Buf[2];
	dat *= 10;
	Buf[0] =  ((int)dat>>8)&0X00FF;	 //闁跨喐鏋婚幏锟?8娴ｏ拷
	Buf[1] =  (int)dat&0X00FF;		 //闁跨喐鏋婚幏锟?8娴ｏ拷
	STC_EE_Write(0x00,Buf,2);  //byte 00  01
}

float Read_Set_T(void)  //读出关灯前的亮度值
{
    U16 dat_H,dat_L;
	float DAT;
	U8  Buf[2];
 	STC_EE_Read(0x00,Buf,2);  //byte 00  01
	dat_H = (Buf[0]<<8)&0Xff00;	  //闁跨喐鏋婚幏锟?8娴ｏ拷
	dat_L =  Buf[1]&0X00ff;		  //闁跨喐鏋婚幏锟?8娴ｏ拷
	DAT = dat_H |  dat_L;
	DAT /= 10;
	return DAT;
}

//void Write_ON_OFF_Time(U16 dat) //写入开关机次数
//{
//    U8  Buf[2];
//	Buf[0] =  (dat>>8)&0X00FF;	 //闁跨喐鏋婚幏锟?8娴ｏ拷
//	Buf[1] =   dat&0X00FF;		 //闁跨喐鏋婚幏锟?8娴ｏ拷
//	STC_EE_Write(0x02,Buf,2);  //byte 02  03
//}
//
//U16 Read_ON_OFF_Time(void)  //读出开关机次数
//{
//    U16 dat_H,dat_L,DAT;
//	U8  Buf[2];
// 	STC_EE_Read(0x02,Buf,2);  //byte 02  03
//	dat_H = (Buf[0]<<8)&0Xff00;	  //闁跨喐鏋婚幏锟?8娴ｏ拷
//	dat_L =  Buf[1]&0X00ff;		  //闁跨喐鏋婚幏锟?8娴ｏ拷
//	DAT = dat_H |  dat_L;
//	return DAT;
//}





