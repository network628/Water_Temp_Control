#ifndef __SH1106_12864_H__
#define __SH1106_12864_H__

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

void Delay1us(unsigned char num);

// 延时
void delay(int i);

void DelayKey(unsigned int Second, unsigned int MS100);

void lcd_address(uchar page, uchar column);

// 全屏清屏
void clear_screen();

// OLED显示模块初始化
void initial_lcd();

// full display test
// void full_display(uchar data1, uchar data2);

// 显示128x64点阵图像
// void display_128x64(uchar *dp);

// 显示32x32点阵图像、汉字、生僻字或32x32点阵的其他图标
// void display_graphic_32x32(uchar page, uchar column, uchar *dp);

// 显示8x16的点阵的字符串，括号里的参数分别为（页,列，字符串指针）
void display_string_8x16(uint page, uint column, uchar *text);
// void Display_String_8x161(uint page,uint column,uchar *text);

// 显示5x8的点阵的字符串，括号里的参数分别为（页,列，字符串指针）
//  void display_string_5x8(uint page,uint column,uchar reverse,uchar *text);

// 写入一组16x16点阵的汉字字符串（字符串表格中需含有此字）
// 括号里的参数：(页，列，汉字字符串）
void display_string_16x16(uchar page, uchar column, uchar *text);

// 显示16x16点阵的汉字或者ASCII码8x16点阵的字符混合字符串
// 括号里的参数：(页，列，字符串）//y x *p
//  void disp_string_8x16_16x16(uchar page,uchar column,uchar *text);

// void display_num_5x8(uint page,uint column,uchar reverse,uint num);

#endif
