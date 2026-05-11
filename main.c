#include <REG52.H>
#include "Inc/XPT2046.h"
#include "Inc/uart.h"
#include "Inc/PWM.h"
#include "Inc/strutils.h"

/**
  * @brief  将unsigned char转为ASCII字符串写入缓冲区
  * @param  buf 目标缓冲区
  * @param  val 要转换的值(0-255)
  * @retval 写入的字符数
  */
static unsigned char ByteToAscii(unsigned char *buf, unsigned char val)
{
	unsigned char len = 0;
	if (val >= 100)
	{
		buf[len++] = '0' + val / 100;
		val %= 100;
		buf[len++] = '0' + val / 10;
		val %= 10;
		buf[len++] = '0' + val;
	}
	else if (val >= 10)
	{
		buf[len++] = '0' + val / 10;
		val %= 10;
		buf[len++] = '0' + val;
	}
	else
	{
		buf[len++] = '0' + val;
	}
	return len;
}

/**
  * @brief  发送四通道ADC数据到上位机
  * @param  ch0-ch3 四个通道的AD值
  * @retval 无
  */
static void SendADCData(unsigned char ch0, unsigned char ch1,
                        unsigned char ch2, unsigned char ch3)
{
	unsigned char buf[16];
	unsigned char pos = 0;
	unsigned char i;

	pos += ByteToAscii(&buf[pos], ch0);
	buf[pos++] = ',';
	pos += ByteToAscii(&buf[pos], ch1);
	buf[pos++] = ',';
	pos += ByteToAscii(&buf[pos], ch2);
	buf[pos++] = ',';
	pos += ByteToAscii(&buf[pos], ch3);
	buf[pos++] = '\n';

	for (i = 0; i < pos; i++)
	{
		UART_SendByte(buf[i]);
	}
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
