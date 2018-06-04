
 /**********************************************************************************************
 
    打印接口的配置初始化文件


 
 
 
 
 **********************************************************************************************/



#ifndef __Sprint_Config_H
#define __Sprint_Config_H 1


#include "stm32f10x.h"//加入头文件



#define  LED0_ON()   GPIO_ResetBits(GPIOC, GPIO_Pin_13) 

#define  LED0_OFF()  GPIO_SetBits(GPIOC, GPIO_Pin_13) //LED高电平灭


//#define  LED1_ON()   GPIO_ResetBits(GPIOD, GPIO_Pin_2) 

//#define  LED1_OFF()  GPIO_SetBits(GPIOD, GPIO_Pin_2) //LED高电平灭


void SysTick_init(void);

void delay_ms(unsigned int nTime);

void TimingDelay_Decrement(void);


void GPIO_Configuration(void);// 系统使用的IO口初始化 

void RCC_Configuration(void); // 时钟分配初始化

void NVIC_Configuration(void);//中断优先级分配初始化

void USART_Configuration(void); //串口初始化函数

void TIM2_Config(void);			//定时器初始化

void SPI1_Init(void);           //硬件SPI1口初始化

void SPI2_Init(void);           //硬件SPI2口初始化

unsigned char SPI1_ReadWriteByte(unsigned char TxData);//SPI1接收或者发送一个数据

unsigned char SPI2_ReadWriteByte(unsigned char TxData);//SPI2接收或者发送一个数据

#endif
