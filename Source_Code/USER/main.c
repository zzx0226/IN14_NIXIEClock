#include "stm32f10x.h"
#include "HV57708.h"
#include "ds3231.h"
#include <stdbool.h>
#include <OLED.h>
#include <gps.h>

#define RGB1_1 GPIO_SetBits(GPIOB,GPIO_Pin_13)
#define RGB1_0 GPIO_ResetBits(GPIOB,GPIO_Pin_13)
	
#define RGB2_1 GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define RGB2_0 GPIO_ResetBits(GPIOB,GPIO_Pin_12)
	
#define RGB3_1 GPIO_SetBits(GPIOB,GPIO_Pin_11)
#define RGB3_0 GPIO_ResetBits(GPIOB,GPIO_Pin_11)
	
#define RGB4_1 GPIO_SetBits(GPIOC,GPIO_Pin_4)
#define RGB4_0 GPIO_ResetBits(GPIOC,GPIO_Pin_4)
	
#define RGB5_1 GPIO_SetBits(GPIOC,GPIO_Pin_5)
#define RGB5_0 GPIO_ResetBits(GPIOC,GPIO_Pin_5)
	
#define RGB6_1 GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define RGB6_0 GPIO_ResetBits(GPIOB,GPIO_Pin_10)

#define RGB7_1 GPIO_SetBits(GPIOA,GPIO_Pin_7)
#define RGB7_0 GPIO_ResetBits(GPIOA,GPIO_Pin_7)
	
#define RGB8_1 GPIO_SetBits(GPIOA,GPIO_Pin_5)
#define RGB8_0 GPIO_ResetBits(GPIOA,GPIO_Pin_5)
	
#define RGB9_1 GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define RGB9_0 GPIO_ResetBits(GPIOA,GPIO_Pin_6)

extern GPS_INFO   GPS;

int protect=0,protect_time=0,protect_i=0,protect_flag=0,protect_1=0,protect_2=0,protect_3=0;

int  sec=0,min=0,hour=0,date=0,month=0;

int R=0,G=0,B=0;

int timmer=0;

int RGB_turn=0;

static bool Neon_status = 0;
u8 dis_data[]={0,0,0,0,0,0};
static u8 second_previous = 0;

extern Time_Typedef TimeValue;

u32 voltage=1,last_voltage=2,pre_voltage=3;
u8 vol_buf[4];

u8 voice=0;

#define MAX              1
#define MIN              0 
   
unsigned int random(unsigned int xxx)  
{  
  unsigned int value,iii;  
  for(iii=0;iii<xxx;iii++)  
  {  
    value = rand() % (MAX + 1- MIN) + MIN;  
  }  
  return value;  
} 

unsigned int random10(unsigned int xxx)  
{  
  unsigned int value,iii;  
  for(iii=0;iii<xxx;iii++)  
  {  
    value = rand() % (9 + 1- 0) + 0;  
  }  
  return value;  
} 


void RGB_ALL_ON()
{
	RGB1_0;
	RGB2_0;
	RGB3_0;
	
	RGB4_0;
	RGB5_0;
	RGB6_0;
	
	RGB7_0;
	RGB8_0;
	RGB9_0;

}


	

void RGB_ON()
{
		R=random(voltage);
	  G=random(last_voltage);
	  B=random(pre_voltage);
	
	if(R)
	{
		RGB1_1;
		RGB4_1;
		RGB7_1;
	
	}
	if(!R)
	{
		RGB1_0;
		RGB4_0;
		RGB7_0;

	}
		if(G)
	{
		RGB2_1;
		RGB5_1;
		RGB8_1;
	
	}
	if(!G)
	{
		RGB2_0;
		RGB5_0;
		RGB8_0;

	}
		if(B)
	{
		RGB3_1;
		RGB6_1;
		RGB9_1;
	
	}
	if(!B)
	{
		RGB3_0;
		RGB6_0;
		RGB9_0;

	}
}

void RGB_OFF()
{
  RGB1_1;
	RGB2_1;
	RGB3_1;
	
	RGB4_1;
	RGB5_1;
	RGB6_1;
	
	RGB7_1;
	RGB8_1;
	RGB9_1;
	

}

void Adc_Multi_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE );
 


	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);  

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode =DISABLE; 
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	
	ADC_InitStructure.ADC_NbrOfChannel = 1;	
	ADC_Init(ADC1, &ADC_InitStructure);	
	
	
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_55Cycles5 );
	
	ADC_Cmd(ADC1, ENABLE);
	
	ADC_ResetCalibration(ADC1); 
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	
	
	ADC_StartCalibration(ADC1);
 
	while(ADC_GetCalibrationStatus(ADC1));
 
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		
}		


u16 Get_Adc(u8 ch)   
{
	/*u16 tempADC = 0; 
	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	tempADC = ADC_GetConversionValue(ADC1);

	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );			     
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
*/
	u32 ResultVolt=0;
	u8 i;
	for(i=0;i<8;i++)
	{
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
		while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));
	  ResultVolt+= ADC_GetConversionValue(ADC1);
	
	}
	ResultVolt=ResultVolt>>3;
	
	ResultVolt=(ResultVolt*3300)>>12;
	
	return ResultVolt;	
}

void NVIC_Configuration1(void)
{
   NVIC_InitTypeDef NVIC_InitStructure;
  
   /* Set the Vector Table base location at 0x08000000 */
   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
  
   /* Configure the NVIC Preemption Priority Bits */  
   NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
   /* Enable the USART1 Interrupt */
   NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;      //通道设置为串口1中断
   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	   	//中断优先级0
   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		   	//使能中断
   NVIC_Init(&NVIC_InitStructure); 						   					//写入寄存器
}

void USART1_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 打开GPIO和USART1时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	/* 设置USART1发射管脚为推挽模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 设置USART1接收管脚为浮空模式 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 设置传输参数波特率等 */
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);

  /* 接收寄存器满，则产生中断 */
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	/* 使能USART1功能 */
	USART_Cmd(USART1, ENABLE);
}


void Neon_Gpio_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;														//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														//速度50MHZ
	GPIO_Init(GPIOC,&GPIO_InitStructure);


}

void Neon_Init(void)
{
    Neon_status = 0;
    GPIO_ResetBits(GPIOC, GPIO_Pin_1); // DOT1
    GPIO_ResetBits(GPIOC, GPIO_Pin_0); // DOT2
    GPIO_ResetBits(GPIOC, GPIO_Pin_3); // DOT3
    GPIO_ResetBits(GPIOC, GPIO_Pin_2); // DOT4
}

void Neon_Flip(void) 
{
    Neon_status = !Neon_status;
    if(Neon_status == 1) {
        GPIO_ResetBits(GPIOC, GPIO_Pin_1); // DOT1
        GPIO_ResetBits(GPIOC, GPIO_Pin_0); // DOT2
        GPIO_ResetBits(GPIOC, GPIO_Pin_3); // DOT3
        GPIO_ResetBits(GPIOC, GPIO_Pin_2); // DOT4
    } else {
        GPIO_SetBits(GPIOC, GPIO_Pin_1); // DOT1
        GPIO_SetBits(GPIOC, GPIO_Pin_0); // DOT2
        GPIO_SetBits(GPIOC, GPIO_Pin_3); // DOT3
        GPIO_SetBits(GPIOC, GPIO_Pin_2); // DOT4
    }
}

void RGB_gpio_init()
{
	GPIO_InitTypeDef	GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);  
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA,DISABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;												
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;													
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB,DISABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_9;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;														
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;													
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC,DISABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;														
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;													
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);  //禁止使用JTAG
	


}

int convert(int a) 

{ 
    int i = 0, ret = 0; 
    int t; 
    while(a) 
    { 

        t = a%10; 

        ret |= t<<(i*4); 

        a/=10; 

        i ++; 

        if(i > 8) break; 

    } 

    return ret; 

}

void key_init()
{
	GPIO_InitTypeDef	GPIO_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO,ENABLE);  
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA,DISABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;													
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB,DISABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_8;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;														
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;														
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE); 

}

int main(void)
{

	HV57708_Init(HV57708_MODE_NORMAL);
	IIC_GPIO_Config(); 
	
	Adc_Multi_Init();
	Neon_Gpio_Init();
  Neon_Init();
	key_init();
	NVIC_Configuration1();
  USART1_Configuration();
	
	
  TimeValue.year=DS3231_YEARDATA;
	TimeValue.month=0x12;
	TimeValue.week=0x00;
	TimeValue.date=0x06;
	TimeValue.hour=0x10;
	TimeValue.minute=0x19;
	TimeValue.second=0x00;
	
	//DS3231_Time_Init(&TimeValue);//DS3231赋初值
	
	Time_Handle();
  second_previous = Display_Time[7] - 0x30;


	RGB_gpio_init();
	OLED_gpio_init();
	OLED_Init();
	Draw_BMP();	
  RGB_ON();
	while(1) 
		
	
	{
		
        Time_Handle();
        dis_data[0] = Display_Time[0] - 0x30;
        dis_data[1] = Display_Time[1] - 0x30;
        dis_data[2] = Display_Time[3] - 0x30;
        dis_data[3] = Display_Time[4] - 0x30;
        dis_data[4] = Display_Time[6] - 0x30;
        dis_data[5] = Display_Time[7] - 0x30;
				
		    hour=dis_data[0]*10+dis_data[1];
		
				if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)==0)
				{
					if(hour>12)
					{
						hour-=12;
						dis_data[0]=(int)(hour/10);
						dis_data[1]=hour-10*dis_data[0];
					}
			
				}
				
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==0)
				{
					USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
					while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)==0);
					USART_ITConfig(USART1,USART_IT_RXNE,DISABLE); 
				}
				else
					USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
				
				
				if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)==0)
		{
		
							voltage=Get_Adc(voice);
			
							if(voltage<3270)
							 timmer=200;
							
							else
								timmer--;
							if(timmer<=0)
								timmer=0;
							
							if(timmer!=0)
							{
								GPIO_ResetBits(GPIOB,GPIO_Pin_9);
								if(RGB_turn)RGB_ON();
								RGB_turn=0;
							}
							else
							{
								GPIO_SetBits(GPIOB,GPIO_Pin_9);
								RGB_OFF();
								RGB_turn=1;
							
							}
							
		}
		else
		{
			GPIO_ResetBits(GPIOB,GPIO_Pin_9);
			RGB_ALL_ON();
		
		}
		
		
        if(dis_data[5] != second_previous) {
            second_previous = dis_data[5];
            Neon_Flip();
        }
				

				
				if(protect)
				{
					GPIO_ResetBits(GPIOB,GPIO_Pin_9);
					RGB_ALL_ON();

					for(protect_i=100;protect_i>=0;protect_i--)
					{
						for(protect_time=9;protect_time>=0;protect_time--)
						{
						dis_data[0] = protect_time;
					  dis_data[1] = protect_time;
					  dis_data[2] = protect_time;
					  dis_data[3] = protect_time;
					  dis_data[4] = protect_time;
					  dis_data[5] = protect_time;
						SetDisplay(dis_data);
            Delay(500);
						}
					
					}
					protect=0;
					RGB_OFF();
				
				}
				
				if(dis_data[2]==3&&dis_data[3]==0&&dis_data[4]==3&&dis_data[5]==0)
					protect=1;
					
				SetDisplay(dis_data);
        Delay(500);
				
				pre_voltage=last_voltage;
				last_voltage=voltage;
				SetDisplay(dis_data);
				
	}
}

