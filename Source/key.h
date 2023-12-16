#ifndef __KEY_H__
#define __KEY_H__

#include "main.h"
//************************************************************************
// key.c
/*--------按键标记 ------*/
#define KEY_NO 0x00 // 无按键
#define KEY_S1 0x01 // 开关
#define KEY_S2 0x02 // ++
#define KEY_S3 0x04 // --
// #define KEY_S4	0x08
#define KEY_REP_FLAG 0x40  // 重复按下
#define KEY_LONG_FLAG 0x80 // 长按3秒标记
// 键值
typedef enum _key
{
    KEY_OFF = KEY_NO,    // 没有任何键按下
    KEY_Switch = KEY_S1, // 开关
    KEY_UUp = KEY_S2,    // ++
    KEY_DDown = KEY_S3,  // --

    // KEY_LEFT			= KEY_S4,  //向左键
    // KEY_L_UP			=(KEY_LONG_FLAG|KEY_UP), //左键+上键
    // KEY_L_DN			=(KEY_LONG_FLAG|KEY_DN),
    // KEY_L_RIGHT			=(KEY_LONG_FLAG|KEY_RIGHT),
    // KEY_L_LEFT			=(KEY_LONG_FLAG|KEY_LEFT),
    // KEY_R_UP			=(KEY_REP_FLAG|KEY_UP),
    // KEY_R_DN			=(KEY_REP_FLAG|KEY_DN),
    // KEY_R_RIGHT			=(KEY_REP_FLAG|KEY_RIGHT),
    // KEY_R_LEFT			=(KEY_REP_FLAG|KEY_LEFT),
    // KEY_L_UP_DN			=(KEY_LONG_FLAG|KEY_UP|KEY_DN), //+ － 长按
    // KEY_L_LEFT_RIGHT	=(KEY_LONG_FLAG|KEY_RIGHT|KEY_LEFT), //OK ESC 长按
    // KEY_L_LEFT_DN		=(KEY_LONG_FLAG|KEY_DN|KEY_LEFT), //DN ESC 长按
} KeyValue_ENUM;

extern bit Key_Scan_Cnt; // 定时器变量10ms执行一次
/*----------按键计时-----------*/
#define KEY_DOWN_CNT 60      // 按下时长
#define KEY_SPEED_CNT 50     // 重复按下重载时间
#define KEY_REP_3S 2         // 重复速度
KeyValue_ENUM KeyScan(void); // 此函数为按键扫描函数 
void key_handle(void);

#endif
