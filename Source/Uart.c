
#include "main.h"

#ifdef Debug_EN
 
#define FOSC 11059200L          //系统频率
#define BAUD 9600             //串口波特率 

#define NONE_PARITY     0       //无校验
#define ODD_PARITY      1       //奇校验
#define EVEN_PARITY     2       //偶校验
#define MARK_PARITY     3       //标记校验
#define SPACE_PARITY    4       //空白校验

#define PARITYBIT NONE_PARITY   //定义校验位

#define S1_S0 0x40 // P_SW1.6
#define S1_S1 0x80 // P_SW1.7

void UartInit(void)
{ 
    ACC = P_SW1;
    ACC &= ~(S1_S0 | S1_S1); // S1_S0=0 S1_S1=0
    P_SW1 = ACC;             //(P3.0/RxD, P3.1/TxD)
  
#if (PARITYBIT == NONE_PARITY)
    SCON = 0x50; // 8位可变波特率
#elif (PARITYBIT == ODD_PARITY) || (PARITYBIT == EVEN_PARITY) || (PARITYBIT == MARK_PARITY)
    SCON = 0xda; // 9位可变波特率,校验位初始为1
#elif (PARITYBIT == SPACE_PARITY)
    SCON = 0xd2; // 9位可变波特率,校验位初始为0
#endif

    T2L = (65536 - (FOSC / 4 / BAUD)); // 设置波特率重装值
    T2H = (65536 - (FOSC / 4 / BAUD)) >> 8;
    AUXR |= 0x14;  // T2为1T模式, 并启动定时器2
    AUXR |= 0x01; // 选择定时器2为串口1的波特率发生器
    // ES = 1;       // 使能串口1中断
    // EA = 1;
}

// void UART_SendByte(unsigned char byte)
// {
//     SBUF = byte;
//     while (!TI)
//         ;   // 等待发送完成
//     TI = 0; // 清除发送完成标志
// }

// void UART_SendString(char *str)
// {
//     while (*str)
//     {
//         UART_SendByte(*str++);
//     }
// }

char putchar(char chr)
{
    SBUF = chr;
    while (!TI)
        ;   // 等待发送完成
    TI = 0; // 清除发送完成标志
    return (chr);
}
 
#endif
