

#ifndef _OLED_H_
#define _OLED_H_



#define LED_IMAGE_WHITE       0
#define LED_IMAGE_BLACK       1

#define X_WIDTH 128
#define Y_WIDTH 64

#define OLED_SCL_1  (GPIO_SetBits(GPIOA,GPIO_Pin_1))
#define OLED_SCL_0  (GPIO_ResetBits(GPIOA,GPIO_Pin_1))
#define OLED_SDA_1  (GPIO_SetBits(GPIOA,GPIO_Pin_0))
#define OLED_SDA_0  (GPIO_ResetBits(GPIOA,GPIO_Pin_0))
#define OLED_RST_1  (GPIO_SetBits(GPIOA,GPIO_Pin_4))
#define OLED_RST_0  (GPIO_ResetBits(GPIOA,GPIO_Pin_4))
#define OLED_DC_1   (GPIO_SetBits(GPIOB,GPIO_Pin_14))
#define OLED_DC_0   (GPIO_ResetBits(GPIOB,GPIO_Pin_14))

//#define BitGet(Number,pos) ((Number) >> (pos)&1)
void OLED_gpio_init(void);

void OLED_WrDat(unsigned f,int data);

void OLED_WrCmd(int cmd);

void OLED_Set_Pos(int x,int y);

void OLED_Fill(int bmp_data);

void OLED_CLS(void);

void OLED_DLY_ms(int ms);

void OLED_Init(void);

void OLED_PutPixel(unsigned int x,unsigned int y);

void OLED_Rectangle(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2,unsigned int gif);

void OLED_P6x8(unsigned int x,unsigned int y,char ch[]);

void OLED_P6x8Str(u8 x,u8 y,u8 ch[]);

void Draw_BMP();

void OLED_Write_Num(int x,int y,int num);

void Draw_img_black(u8 *pucTable);

void Image_Show(u8 age[60][80]);

void OLED_Write_Num_two(int x,int y,int num);










#endif

