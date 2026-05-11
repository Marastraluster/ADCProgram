#include <REG52.H>
#include "Inc/PWM.h"

sbit PWM_OUT = P1^0;

static unsigned int TotalCount;
static unsigned int HighReload;
static unsigned int LowReload;
static unsigned char CurrentDuty;
unsigned char PWM_Enabled = 0;

/**
  * @brief  PWM初始化，使用定时器0模式1
  * @param  无
  * @retval 无
  */
void PWM_Init(void)
{
	TMOD &= 0xF0;
	TMOD |= 0x01;
	ET0 = 1;
	TR0 = 0;
	PWM_OUT = 0;
	PWM_Enabled = 0;
	CurrentDuty = 50;
	PWM_SetFrequency(1000);
	PWM_SetDuty(50);
}

/**
  * @brief  设置PWM频率
  * @param  Freq 频率值(Hz)，范围100-5000
  * @retval 无
  */
void PWM_SetFrequency(unsigned int Freq)
{
	unsigned int HighCount;
	unsigned int LowCount;
	if (Freq < 100) Freq = 100;
	if (Freq > 5000) Freq = 5000;
	EA = 0;
	TotalCount = 921600UL / Freq;
	HighCount = (unsigned int)((unsigned long)TotalCount * CurrentDuty / 100);
	LowCount = TotalCount - HighCount;
	if (HighCount == 0) HighCount = 1;
	if (LowCount == 0) LowCount = 1;
	HighReload = 65536UL - HighCount;
	LowReload = 65536UL - LowCount;
	EA = 1;
}

/**
  * @brief  设置PWM占空比
  * @param  Duty 占空比(%)，范围0-100
  * @retval 无
  */
void PWM_SetDuty(unsigned char Duty)
{
	unsigned int HighCount;
	unsigned int LowCount;
	if (Duty > 100) Duty = 100;
	EA = 0;
	CurrentDuty = Duty;
	HighCount = (unsigned int)((unsigned long)TotalCount * Duty / 100);
	LowCount = TotalCount - HighCount;
	if (HighCount == 0) HighCount = 1;
	if (LowCount == 0) LowCount = 1;
	HighReload = 65536UL - HighCount;
	LowReload = 65536UL - LowCount;
	EA = 1;
}

/**
  * @brief  使能PWM输出
  * @param  无
  * @retval 无
  */
void PWM_Enable(void)
{
	PWM_Enabled = 1;
	PWM_OUT = 1;
	TH0 = HighReload >> 8;
	TL0 = HighReload & 0xFF;
	TR0 = 1;
	EA = 1;
}

/**
  * @brief  禁用PWM输出
  * @param  无
  * @retval 无
  */
void PWM_Disable(void)
{
	TR0 = 0;
	PWM_OUT = 0;
	PWM_Enabled = 0;
}

/**
  * @brief  定时器0中断服务程序，PWM波形生成
  * @param  无
  * @retval 无
  */
void Timer0_Routine(void) interrupt 1
{
	if (PWM_OUT)
	{
		PWM_OUT = 0;
		TH0 = LowReload >> 8;
		TL0 = LowReload & 0xFF;
	}
	else
	{
		PWM_OUT = 1;
		TH0 = HighReload >> 8;
		TL0 = HighReload & 0xFF;
	}
}
