
#include "main.h"
//#include"SH1106_12864.h"
#include "ASCII_CODE_8X16_5X8_VERTICAL.H"
//#include "chinese_and_graphic.h"
//#include "font.h"
 
/*
	@brief			�ӳ�1us
	@param			��
	@retval			��
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

//��ʱ
void delay(int i)               
{
	unsigned char k;
	int j;
	for(j=0;j<i;j++)
		for(k=0;k<110;k++);
}

#ifdef SPI
//дָ�OLED��ʾģ��
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

//д���ݵ�OLED��ʾģ��
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
 
//дָ�OLED��ʾģ��
void transfer_command(int data1)   
{
     start();
     write_w(0x78);
     write_w(0x00);
     write_w(data1);
     stop();
}

//д���ݵ�OLED��ʾģ��
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
	column=column;  							//����ƽ����˵�ĵ�1�У���LCD����IC���ǵ�0�С������������ȥ1.
	page=page-1;
	transfer_command(0xb0+page);   			//����ҳ��ַ��ÿҳ��8�С�һ�������64�б��ֳ�8��ҳ������ƽ����˵�ĵ�1ҳ����LCD����IC���ǵ�0ҳ�������������ȥ1
	transfer_command(((column>>4)&0x0f)+0x10);	//�����е�ַ�ĸ�4λ
	transfer_command(column&0x0f);				//�����е�ַ�ĵ�4λ
}
  
//ȫ������
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

//OLED��ʾģ���ʼ��
void initial_lcd()
{
  
	       
	delay(500);//�������ʱ����Ҫ,һ��50ms����,������������Գ�һЩ
 
	transfer_command(0xAE);	//�ر���ʾ

	transfer_command(0x00);	//���õ��е�ַ
	transfer_command(0x10);	//���ø��е�ַ
	transfer_command(0x40);	//������ʼ�е�ַ
	transfer_command(0xB0);	//����ҳ��ַ

	transfer_command(0x81); 	// �Աȶ����ã�����������
	transfer_command(0xFF);	//  265  

	transfer_command(0xA1);	//���öΣ�SEG������ʼӳ���ַ��column��127��ַ��SEG0�ĵ�ַ
	transfer_command(0xA6);	//������ʾ��0xa7����ʾ

	transfer_command(0xA8);	//��������·����16~64��
	transfer_command(0x3F);	//64duty
	
	transfer_command(0xC8);	//��ӳ��ģʽ��COM[N-1]~COM0ɨ��

	transfer_command(0xD3);	//������ʾƫ��
	transfer_command(0x00);	//��ƫ��
	
	transfer_command(0xD5);	//����������Ƶ
	transfer_command(0x80);	//ʹ��Ĭ��ֵ
	
	transfer_command(0xD9);	//���� Pre-Charge Period
	transfer_command(0xF1);	//ʹ�ùٷ��Ƽ�ֵ
	
	transfer_command(0xDA);	//���� com pin configuartion
	transfer_command(0x12);	//ʹ��Ĭ��ֵ
	
	transfer_command(0xDB);	//���� Vcomh���ɵ������ȣ�Ĭ�ϣ�
	transfer_command(0x40);	////ʹ�ùٷ��Ƽ�ֵ
	
	transfer_command(0x8D);	//����OLED��ɱ�
	transfer_command(0x14);	//����ʾ
	
	transfer_command(0xAF);	//����OLED�����ʾ
	clear_screen();  //��ʼ����
	lcd_address(0,0); 
}
  
//��ʾ8x16�ĵ�����ַ�����������Ĳ����ֱ�Ϊ��ҳ,�У��ַ���ָ�룩
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
					transfer_data(ascii_table_8x16[j][k+8*n]);	//д���ݵ�LCD,ÿд��1�ֽڵ����ݺ��е�ַ�Զ���1
				}
			}
			i++;
			column+=8;
		}
		else
		i++;
	}
}

//д��һ��16x16����ĺ����ַ������ַ���������躬�д��֣�
//������Ĳ�����(ҳ���У������ַ�����
void display_string_16x16(uchar page,uchar column,uchar *text)
{
    uchar i,j,k;
    uint address; 
    
    j = 0;
    while(text[j] != '\0')
    {
        i = 0;
        address = 1;
        while(Chinese_text_16x16[i] > 0x7e)	  // >0x7f��˵������ASCII���ַ�
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
        
        if(address != 1)// ��ʾ����                   
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
        else              //��ʾ�հ��ַ�            
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
 