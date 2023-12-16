
#include "main.h"
//#include"SH1106_12864.h"
#include "ASCII_CODE_8X16_5X8_VERTICAL.H"
//#include "chinese_and_graphic.h"
//#include "font.h"
 
/*
	@brief			延迟1us
	@param			无
	@retval			无
 */
void Delay1us(unsigned char num)
{

	while(num--)
	{
		_nop_();
		_nop_();
		_nop_();
	}

}

//延时
void delay(int i)               
{
	unsigned char k;
	int j;
	for(j=0;j<i;j++)
		for(k=0;k<110;k++);
}

#ifdef SPI
//写指令到OLED显示模块
void transfer_command(int data1)   
{
    uchar i;
    lcd_dc= 0;    
	for(i=0;i<8;i++)
	{
        lcd_sclk = 0;    
        if (data1 & 0x80)	lcd_sda = 1;
        else  				lcd_sda = 0;
        lcd_sclk = 1; 
        data1 <<= 1;    
    }
}

//写数据到OLED显示模块
void transfer_data(int data1)
{
    uchar i;
    lcd_dc= 1;    
	for(i=0;i<8;i++)
	{
        lcd_sclk = 0;    
        if (data1 & 0x80)	lcd_sda = 1;
        else  				lcd_sda = 0;
        lcd_sclk = 1; 
        data1 <<= 1;    
    }
} 
#else

void start()
{
     SCL=1;	 Delay1us(1);
     SDA=1;	 Delay1us(1);
     SDA=0;	 Delay1us(1);
     SCL=0;	 Delay1us(1);
}

void stop()
{
     SCL=0;	 Delay1us(1);
     SDA=0;	 Delay1us(1);
     SDA=1;	 Delay1us(1);
     SCL=1;	 Delay1us(1);
}

void write_w(unsigned char dat)
{
     unsigned char m,da;
     unsigned char j;
     da=dat;
     for(j=0;j<8;j++)
     {
	     m=da;
	     SCL=0;	 Delay1us(1);
	     m=m&0x80;
	     if(m==0x80)
		 {
	      SDA=1; Delay1us(1);
		 }
	     else
		 {
	      SDA=0;  Delay1us(1);
		 }
	     da=da<<1;Delay1us(1);
	     SCL=1;	 Delay1us(1);
	 }
     SCL=0;	  Delay1us(1);
     //_nop_();  
     SCL=1;	  Delay1us(1);
}
 
//写指令到OLED显示模块
void transfer_command(int data1)   
{
     start();
     write_w(0x78);
     write_w(0x00);
     write_w(data1);
     stop();
}

//写数据到OLED显示模块
void transfer_data(int data1)
{
     start();
     write_w(0x78);
     write_w(0x40);
     write_w(data1);
     stop();
}


#endif   

					  //y			x
void lcd_address(uchar page,uchar column)
{
	column=column;  							//我们平常所说的第1列，在LCD驱动IC里是第0列。所以在这里减去1.
	page=page-1;
	transfer_command(0xb0+page);   			//设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，在LCD驱动IC里是第0页，所以在这里减去1
	transfer_command(((column>>4)&0x0f)+0x10);	//设置列地址的高4位
	transfer_command(column&0x0f);				//设置列地址的低4位
}
  
//全屏清屏
void clear_screen()
{
	unsigned char i,j;
 	for(j=0;j<8;j++)
	{
		lcd_address(1+j,0);
		for(i=0;i<128;i++)
		{
			transfer_data(0x00);
		}
	}
}

//OLED显示模块初始化
void initial_lcd()
{
  
	       
	delay(500);//这里的延时很重要,一般50ms即可,保险起见，可以长一些
 
	transfer_command(0xAE);	//关闭显示

	transfer_command(0x00);	//设置低列地址
	transfer_command(0x10);	//设置高列地址
	transfer_command(0x40);	//设置起始行地址
	transfer_command(0xB0);	//设置页地址

	transfer_command(0x81); 	// 对比度设置，可设置亮度
	transfer_command(0xFF);	//  265  

	transfer_command(0xA1);	//设置段（SEG）的起始映射地址；column的127地址是SEG0的地址
	transfer_command(0xA6);	//正常显示；0xa7逆显示

	transfer_command(0xA8);	//设置驱动路数（16~64）
	transfer_command(0x3F);	//64duty
	
	transfer_command(0xC8);	//重映射模式，COM[N-1]~COM0扫描

	transfer_command(0xD3);	//设置显示偏移
	transfer_command(0x00);	//无偏移
	
	transfer_command(0xD5);	//设置震荡器分频
	transfer_command(0x80);	//使用默认值
	
	transfer_command(0xD9);	//设置 Pre-Charge Period
	transfer_command(0xF1);	//使用官方推荐值
	
	transfer_command(0xDA);	//设置 com pin configuartion
	transfer_command(0x12);	//使用默认值
	
	transfer_command(0xDB);	//设置 Vcomh，可调节亮度（默认）
	transfer_command(0x40);	////使用官方推荐值
	
	transfer_command(0x8D);	//设置OLED电荷泵
	transfer_command(0x14);	//开显示
	
	transfer_command(0xAF);	//开启OLED面板显示
	clear_screen();  //初始清屏
	lcd_address(0,0); 
}
  
//显示8x16的点阵的字符串，括号里的参数分别为（页,列，字符串指针）
void display_string_8x16(uint page,uint column,uchar *text)
{
	uint i=0,j,k,n;
	if(column>123)
	{
		column=1;
		page+=2;
	}
	while(text[i]>0x00)
	{	
		if((text[i]>=0x20)&&(text[i]<=0x7e))
		{
			j=text[i]-0x20;
			for(n=0;n<2;n++)
			{			
				lcd_address(page+n,column);
				for(k=0;k<8;k++)
				{					
					transfer_data(ascii_table_8x16[j][k+8*n]);	//写数据到LCD,每写完1字节的数据后列地址自动加1
				}
			}
			i++;
			column+=8;
		}
		else
		i++;
	}
}

//写入一组16x16点阵的汉字字符串（字符串表格中需含有此字）
//括号里的参数：(页，列，汉字字符串）
void display_string_16x16(uchar page,uchar column,uchar *text)
{
    uchar i,j,k;
    uint address; 
    
    j = 0;
    while(text[j] != '\0')
    {
        i = 0;
        address = 1;
        while(Chinese_text_16x16[i] > 0x7e)	  // >0x7f即说明不是ASCII码字符
        {
            if(Chinese_text_16x16[i] == text[j])
            {
                if(Chinese_text_16x16[i + 1] == text[j + 1])
                {
                    address = i * 16;
                    break;
                }
            }
            i += 2;            
        }
        
        if(column > 113)
        {
            column = 0;
            page += 2;
        }
        
        if(address != 1)// 显示汉字                   
        {
            
			for(k=0;k<2;k++)
			{
				lcd_address(page+k,column);
	            for(i = 0; i < 16; i++)               
	            {
	                transfer_data(Chinese_code_16x16[address]);   
	                address++;
	            }
   			}
            j += 2;        
        }
        else              //显示空白字符            
        {
			for(k=0;k<2;k++)
			{
				lcd_address(page+k,column);
	            for(i = 0; i < 16; i++)               
	            {
	                transfer_data(0x00);   
	            }
   			}
            
            j++;
        }
        
        column+=16;
    }
}
 