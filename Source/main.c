#include "main.h"

void main(void)
{
	ntc.T0_CNT = 0;
#ifdef Debug_EN
	UartInit();
#endif
	Init_ADC();
	PWM_PCA_Init();
	Timer0_Init();
	//	Timer2_Init(); 123
	initial_lcd();	// ��ʼ��
	clear_screen(); // ����
	display_string_16x16(1, 1, "����");
	display_string_8x16(1, 8 * 4 + 3, ":");

	display_string_16x16(3, 1, "�趨");
	display_string_8x16(3, 35, ":");
	display_string_16x16(3, 8 * 14, "��");

	display_string_16x16(5, 1, "��ǰ");
	display_string_8x16(5, 35, ":");
	display_string_16x16(5, 8 * 14, "��");

	display_string_16x16(7, 1, "���");
	display_string_8x16(7, 35, ":");
	display_string_8x16(7, 8 * 14, "%");

	PIDParament_Init();
	ntc.Hour = 0;
	ntc.Min = 0;
	ntc.Second = 0;
	ntc.Tick = 0;

	// printf("Main_Start\r\n");
	 
	while (1)
	{
		key_handle();
		Cool_handle();
	}
}
