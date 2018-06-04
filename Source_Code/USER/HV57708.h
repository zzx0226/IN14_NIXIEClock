#ifndef __HV57708_H
#define __HV57708_H
#include "stm32f10x.h"
/*
 *     HV57708_CLK - CLK
 *     HV57708_LE  - LE
 *     HV57708_POL - POL
 *     HV57708_DI1 - Din1/Dout4(A)
 *     HV57708_DI2 - Din2/Dout3(A)
 *     HV57708_DI3 - Din3/Dout2(A)
 *     HV67708_DI4 - Din4/Dout1(A)
*/
#define HV57708_MODE_NORMAL 1//Ë³Ðò£¬DIR½Ó¸ß
#define HV57708_MODE_INVERTION 0//ÄæÐò£¬DIR½ÓµÍ

#define HV57708_CLK_H    GPIO_SetBits(GPIOC,GPIO_Pin_11)
#define HV57708_LE_H     GPIO_SetBits(GPIOC,GPIO_Pin_12)
#define HV57708_POL_H    GPIO_SetBits(GPIOC,GPIO_Pin_10)
#define HV57708_DI1_H    GPIO_SetBits(GPIOB,GPIO_Pin_7)
#define HV57708_DI2_H    GPIO_SetBits(GPIOB,GPIO_Pin_6)
#define HV57708_DI3_H    GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define HV57708_DI4_H    GPIO_SetBits(GPIOD,GPIO_Pin_2)

#define HV57708_CLK_L    GPIO_ResetBits(GPIOC,GPIO_Pin_11)
#define HV57708_LE_L     GPIO_ResetBits(GPIOC,GPIO_Pin_12)
#define HV57708_POL_L    GPIO_ResetBits(GPIOC,GPIO_Pin_10)
#define HV57708_DI1_L    GPIO_ResetBits(GPIOB,GPIO_Pin_7)
#define HV57708_DI2_L    GPIO_ResetBits(GPIOB,GPIO_Pin_6)
#define HV57708_DI3_L    GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define HV57708_DI4_L    GPIO_ResetBits(GPIOD,GPIO_Pin_2)

void Delay(u32 time);
void HV57708_Init(u8 HV57708_Mode);
void HV57708_SendData(u32 datapart2, u32 datapart1);
void HV57708_OutputData(void);
void SetDisplay(unsigned char data[]);
#endif
