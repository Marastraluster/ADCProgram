#ifndef __PWM_H__
#define __PWM_H__

void PWM_Init(void);
void PWM_SetFrequency(unsigned int Freq);
void PWM_SetDuty(unsigned char Duty);
void PWM_Enable(void);
void PWM_Disable(void);

extern unsigned char PWM_Enabled;

#endif
