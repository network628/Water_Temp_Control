#ifndef __SH1106_12864_H__
#define __SH1106_12864_H__

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long

void Delay1us(unsigned char num);

// ��ʱ
void delay(int i);

void DelayKey(unsigned int Second, unsigned int MS100);

void lcd_address(uchar page, uchar column);

// ȫ������
void clear_screen();

// OLED��ʾģ���ʼ��
void initial_lcd();

// full display test
// void full_display(uchar data1, uchar data2);

// ��ʾ128x64����ͼ��
// void display_128x64(uchar *dp);

// ��ʾ32x32����ͼ�񡢺��֡���Ƨ�ֻ�32x32���������ͼ��
// void display_graphic_32x32(uchar page, uchar column, uchar *dp);

// ��ʾ8x16�ĵ�����ַ�����������Ĳ����ֱ�Ϊ��ҳ,�У��ַ���ָ�룩
void display_string_8x16(uint page, uint column, uchar *text);
// void Display_String_8x161(uint page,uint column,uchar *text);

// ��ʾ5x8�ĵ�����ַ�����������Ĳ����ֱ�Ϊ��ҳ,�У��ַ���ָ�룩
//  void display_string_5x8(uint page,uint column,uchar reverse,uchar *text);

// д��һ��16x16����ĺ����ַ������ַ���������躬�д��֣�
// ������Ĳ�����(ҳ���У������ַ�����
void display_string_16x16(uchar page, uchar column, uchar *text);

// ��ʾ16x16����ĺ��ֻ���ASCII��8x16������ַ�����ַ���
// ������Ĳ�����(ҳ���У��ַ�����//y x *p
//  void disp_string_8x16_16x16(uchar page,uchar column,uchar *text);

// void display_num_5x8(uint page,uint column,uchar reverse,uint num);

#endif
