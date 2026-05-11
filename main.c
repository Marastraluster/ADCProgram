#include <REG52.H>
#include "Inc/XPT2046.h"
#include "Inc/uart.h"
#include "Inc/PWM.h"
#include "Inc/strutils.h"

/**
  * @brief  发送一个float数据（小端序）
  * @param  val 要发送的float值
  * @retval 无
  */
static void UART_SendFloat(float val)
{
	unsigned char *p = (unsigned char *)&val;
	UART_SendByte(p[0]);
	UART_SendByte(p[1]);
	UART_SendByte(p[2]);
	UART_SendByte(p[3]);
}

/**
  * @brief  以JustFloat格式发送四通道ADC数据
  * @param  ch0-ch3 四个通道的AD值(0-255)
  * @retval 无
  *
  * JustFloat帧格式: [f0][f1][f2][f3][0x00][0x00][0x80][0x7F]
  * 每个float占4字节，小端序，帧尾固定为 00 00 80 7F
  */
static void SendADCData(unsigned char ch0, unsigned char ch1,
                        unsigned char ch2, unsigned char ch3)
{
	UART_SendFloat((float)ch0);
	UART_SendFloat((float)ch1);
	UART_SendFloat((float)ch2);
	UART_SendFloat((float)ch3);
	UART_SendByte(0x00);
	UART_SendByte(0x00);
	UART_SendByte(0x80);
	UART_SendByte(0x7F);
}

/**
  * @brief  解析接收到的串口命令
  * @param  无
  * @retval 无
  */
static void ParseCommand(void)
{
	if (ReceiveBuf[0] == 'F' && ReceiveBuf[1] == '=')
	{
		unsigned int Freq = Str_GetNumber(&ReceiveBuf[2]);
		PWM_SetFrequency(Freq);
	}
	else if (ReceiveBuf[0] == 'D' && ReceiveBuf[1] == '=')
	{
		unsigned char Duty = (unsigned char)Str_GetNumber(&ReceiveBuf[2]);
		PWM_SetDuty(Duty);
	}
	ReceiveFlag = 0;
}

void main(void)
{
	unsigned char adc_ch0, adc_ch1, adc_ch2, adc_ch3;

	UART_Init();
	PWM_Init();
	PWM_Enable();

	while (1)
	{
		adc_ch0 = (unsigned char)XPT2046_ReadAD(XPT2046_VBAT);
		adc_ch1 = (unsigned char)XPT2046_ReadAD(XPT2046_AUX);
		adc_ch2 = (unsigned char)XPT2046_ReadAD(XPT2046_XP);
		adc_ch3 = (unsigned char)XPT2046_ReadAD(XPT2046_YP);

		SendADCData(adc_ch0, adc_ch1, adc_ch2, adc_ch3);

		if (ReceiveFlag)
		{
			ParseCommand();
		}
	}
}
