#ifndef __UART_H__
#define __UART_H__

#define UART_BUF_SIZE 16

extern unsigned char ReceiveBuf[UART_BUF_SIZE];
extern unsigned char ReceiveFlag;

void UART_Init(void);
void UART_SendByte(unsigned char Byte);
void UART_SendString(unsigned char *Str);

#endif
