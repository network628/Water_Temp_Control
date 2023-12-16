/******************************************************************************
                                智信电子科技
******************************************************************************/

// SQUIRREL_STC12C5A60S2开发板例程
// 片内EEPROM驱动头文件
// R207SC@SQUIRREL_STC12C5A60S2





#ifndef     __STCEEPROM_H__
#define     __STCEEPROM_H__


//关闭IAP 功能, 清相关的特殊功能寄存器,使CPU 处于安全状态,
void IAP_Disable(void);
//读一字节，调用前需打开IAP 功能，入口:DPTR = 字节地址，返回:A = 读出字节
unsigned char Byte_Read(unsigned int add);
// //字节编程，调用前需打开IAP 功能，入口:DPTR = 字节地址, A= 须编程字节的数据
void Byte_Program(unsigned int add, unsigned char ch);
// //擦除扇区, 入口:DPTR = 扇区地址
void Sector_Erase(unsigned int add);



// // 读取STC EEPROM
// unsigned char STC_EE_Read(unsigned int Addr, unsigned char *Buf, unsigned int Num);
// // 写入STC EEPROM
// unsigned char STC_EE_Write(unsigned int Addr, unsigned char *Buf, unsigned int Num);

void Write_Set_T(float dat);
float Read_Set_T(void);

//void Write_ON_OFF_Time(unsigned int dat);
//unsigned int Read_ON_OFF_Time(void);


#endif // end of __STC_EEPROM_H__














