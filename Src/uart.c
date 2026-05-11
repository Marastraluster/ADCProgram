#include <REG52.H>
#include "Inc/uart.h"

unsigned char ReceiveBuf[UART_BUF_SIZE];
static unsigned char ReceiveIndex = 0;
unsigned char ReceiveFlag = 0;

/**
  * @brief  串口初始化，9600bps@11.0592MHz
  * @param  无
  * @retval 无
  */
void UART_Init(void)
{
	SCON = 0x50;
	PCON &= 0x7F;
	TMOD &= 0x0F;
	TMOD |= 0x20;
	TH1 = 0xFD;
	TL1 = 0xFD;
	ET1 = 0;
	TR1 = 1;
	ES = 1;
	EA = 1;
}

/**
  * @brief  串口发送一个字节数据
  * @param  Byte 要发送的一个字节数据
  * @retval 无
  */
void UART_SendByte(unsigned char Byte)
{
	SBUF = Byte;
	while (TI == 0);
	TI = 0;
}

/**
  * @brief  串口发送字符串
  * @param  Str 要发送的字符串指针
  * @retval 无
  */
void UART_SendString(unsigned char *Str)
{
	while (*Str)
	{
		UART_SendByte(*Str++);
	}
}

/**
  * @brief  串口中断服务程序，接收数据到ReceiveBuf
  * @param  无
  * @retval 无
  */
void UART_Routine(void) interrupt 4
{
	unsigned char ch;
	if (RI)
	{
		RI = 0;
		ch = SBUF;
		if (ReceiveFlag == 0)
		{
			if (ch == '\n')
			{
				ReceiveBuf[ReceiveIndex] = '\0';
				ReceiveFlag = 1;
				ReceiveIndex = 0;
			}
			else
			{
				if (ReceiveIndex < UART_BUF_SIZE - 1)
				{
					ReceiveBuf[ReceiveIndex++] = ch;
				}
			}
		}
	}
}
