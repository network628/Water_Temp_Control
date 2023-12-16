//==============================================================================
//	Include Header Files
//==============================================================================
#include "main.h"

bit Key_Scan_Cnt; // 定时器变量10ms执行一次
KeyValue_ENUM key;
// 按键扫描
KeyValue_ENUM KeyScan(void)
{
	uint8_t io_value;
	static uint8_t key_Press; // 这个是要返回的键值
	static uint8_t key_Old = 0x00;
	static uint8_t DownCnt = 0; // 按下计数
	static uint8_t CntLongPress = 0;
	static struct _sta
	{
		uint8_t up : 1; // 弹起
		uint8_t dn : 1; // 按下
		uint8_t rep : 1;
	} Status; // 按键状态标记
	uint8_t ret;

	io_value = 0x00;
	ret = KEY_OFF;

	// 获取当前按下的按键
	if (!KEY_SW)
		io_value |= KEY_S1;
	if (!KEY_Up)
		io_value |= KEY_S2;
	if (!KEY_Dowm)
		io_value |= KEY_S3;

	// if (!Key4_ReadSta())io_value |= KEY_S4;

	if (io_value) // 如果按键被按下
	{
		if (key_Old == io_value) // 判断与上次扫描到的是否为同一个按键
		{
			DownCnt++; // 计数

			if (DownCnt < KEY_DOWN_CNT) // 当前计数小于长按的时间
			{
				if (Status.rep == 0) // 不是重复
				{
					Status.dn = 1;
					key_Press = io_value; // 记下按键
				}
			}

			if (DownCnt >= KEY_DOWN_CNT) // 当前计数大于长按的时间
			{
				Status.rep = 1; // 标记重复

				if (CntLongPress++ == KEY_REP_3S) // 按下超过3秒
				{
					ret = io_value | KEY_LONG_FLAG; // 加长按标记
				}
				else
				{
					ret = io_value | KEY_REP_FLAG; // 重复
				}

				DownCnt = KEY_SPEED_CNT; // 重复起始值
			}
		}
		key_Old = io_value; // 记录键值
	}
	else // 按键松开
	{
		if (Status.dn && !Status.rep) // 返回松开前的键值
		{
			ret = key_Press;
		}

		// 清除标记
		Status.dn = 0;
		Status.rep = 0;
		DownCnt = 0;
		CntLongPress = 0;
		key_Old = KEY_NO;
	}

	return (KeyValue_ENUM)ret; // 返回键值
}
//--------------END OF FILE---------------

//==============================================================================
//	Function: key_handle
//		Input: None
//		Output: None
//==============================================================================
void key_handle(void)
{
	if (Key_Scan_Cnt == 0)
		return;
	Key_Scan_Cnt = 0;

	key = KeyScan(); // 获取按下的按键值
	switch (key)
	{
	case KEY_Switch: //
		if (SW_Flag == 1)
			SW_Flag = 0;
		else if (SW_Flag == 0)
			SW_Flag = 1;
		break;
	case KEY_UUp: //
		ntc.Set_T += 1;
		if (ntc.Set_T > 50)
		{
			ntc.Set_T = 50;
		}
		Write_Set_T(ntc.Set_T); // 写入EEP_ROM
		break;
	case KEY_DDown: //
		ntc.Set_T -= 1;
		if (ntc.Set_T < 1)
		{
			ntc.Set_T = 1;
		}
		Write_Set_T(ntc.Set_T); // 写入EEP_ROM
		break;
	default:
		break;
	}
}
