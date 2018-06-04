//********************************************************************************************************/
#include "gps.h"
#include <string.h>
#include "ds3231.h"

#include <stdio.h>
#include <stdlib.h>

static uchar GetComma(uchar num,char* str);
static int Get_Int_Number(char *s);
static double Get_Double_Number(char *s);
static float Get_Float_Number(char *s);
static void UTC2BTC(DATE_TIME *GPS);
extern Time_Typedef TimeValue;

char str1[20];
char year1[20];
char month1[20];
char day1[20];
char hour1[20];
char min1[20];
char sec1[20];

char lat[20];
char lon[20];
char lat1[20];
char lon1[20];
char dot1[1]=".";

char sea1[20];
char sea2[20];

//====================================================================//
// 函数:int GPS_RMC_Parse(char *line, GPS_INFO *GPS)  
// 功能:解析GPRMC信息
// 参数:存放原始信息数组，解析为可识别结构体
// 返回值:
//			 1: GPRMC解析完毕
//       0: 没有解析，或数据无效
//====================================================================//
int GPS_RMC_Parse(char *line,GPS_INFO *GPS)
{
	
	uchar ch, status, tmp;
	float lati_cent_tmp, lati_second_tmp;
	float long_cent_tmp, long_second_tmp;
	float speed_tmp;
	char *buf = line;
	ch = buf[5];
	
	status = buf[GetComma(2, buf)];
	if (ch == 'C')  //如果第5个字符是C,($GPRMC)
	{
		
		if (status == 'V')
		{
			OLED_P6x8Str(95,0,"GPS");
			OLED_P6x8Str(85,6,"loading");
		}
		if (status == 'A')  //如果数据有效，则分析
		{
			GPS -> NS       = buf[GetComma(4, buf)];
			GPS -> EW       = buf[GetComma(6, buf)];
			GPS -> STA      = buf[GetComma(2, buf)];

			GPS->latitude   = Get_Double_Number(&buf[GetComma(3,buf)]);
			GPS->longitude  = Get_Double_Number(&buf[GetComma(5,buf)]);

      GPS->latitude_Degree  = (int)GPS->latitude / 100;       //分离纬度
			lati_cent_tmp         = (GPS->latitude - GPS->latitude_Degree * 100);
			GPS->latitude_Cent    = (int)lati_cent_tmp;
			lati_second_tmp       = (lati_cent_tmp - GPS->latitude_Cent) * 60;
			GPS->latitude_Second  = (int)lati_second_tmp;

			GPS->longitude_Degree = (int)GPS->longitude / 100;	     //分离经度
			long_cent_tmp         = (GPS->longitude - GPS->longitude_Degree * 100);
			GPS->longitude_Cent   = (int)long_cent_tmp;    
			long_second_tmp       = (long_cent_tmp - GPS->longitude_Cent) * 60;
			GPS->longitude_Second = (int)long_second_tmp;

			speed_tmp      = Get_Float_Number(&buf[GetComma(7, buf)]);    //速度（海里/小时）
			GPS->speed     = speed_tmp * 1.85;                           //1海里=1.85公里
			GPS->direction = Get_Float_Number(&buf[GetComma(8, buf)]); //角度		

			GPS->D.hour    = (buf[7] - '0') * 10 + (buf[8] - '0');		//时间
			GPS->D.minute  = (buf[9] - '0') * 10 + (buf[10] - '0');
			GPS->D.second  = (buf[11] - '0') * 10 + (buf[12] - '0');
			tmp = GetComma(9, buf);
			GPS->D.day     = (buf[tmp + 0] - '0') * 10 + (buf[tmp + 1] - '0'); //日期
			GPS->D.month   = (buf[tmp + 2] - '0') * 10 + (buf[tmp + 3] - '0');
			GPS->D.year    = (buf[tmp + 4] - '0') * 10 + (buf[tmp + 5] - '0')+2000;

			UTC2BTC(&GPS->D);
			

      UART_PutStr(USART1,"Hello,Zhang Zixu\n");
      
			Int_To_Str(Get_Double_Number(&buf[GetComma(3,buf)])/100,lat);
			Int_To_Str(Get_Double_Number(&buf[GetComma(5,buf)])/100,lon);
			Int_To_Str((Get_Double_Number(&buf[GetComma(3,buf)])-(int)Get_Double_Number(&buf[GetComma(3,buf)]))*100,lat1);
			Int_To_Str((Get_Double_Number(&buf[GetComma(5,buf)])-(int)Get_Double_Number(&buf[GetComma(5,buf)]))*100,lon1);
	
				OLED_P6x8Str(95,0,"GPS");
				
				
				strcat(lat, dot1);
				strcat(lat, lat1);
				
				strcat(lon, dot1);
				strcat(lon, lon1);
				
				OLED_P6x8Str(82,2,lat);
				OLED_P6x8Str(118,2,"N");
				OLED_P6x8Str(82,3,lon);
				OLED_P6x8Str(118,3,"E");
				
				OLED_P6x8Str(85,6,"Success");

	TimeValue.year=convert(GPS->D.year);
	TimeValue.month=convert(GPS->D.month);
	//TimeValue.week=0x03;
	TimeValue.date=convert(GPS->D.day);
	TimeValue.hour=convert(GPS->D.hour);
	TimeValue.minute=convert((buf[9] - '0') * 10 + (buf[10] - '0'));
	//TimeValue.minute=GPS->D.minute;
	TimeValue.second=convert((buf[11] - '0') * 10 + (buf[12] - '0'));
	//TimeValue.second=GPS->D.second;
	DS3231_Time_Init(&TimeValue);

			return 1;
		}		
	}
	
	return 0;
}

//====================================================================//
// 函数:int GPS_GGA_Parse(char *line, GPS_INFO *GPS)  
// 功能:解析GPGGA信息
// 参数:存放原始信息数组，解析为可识别结构体
// 返回值:
//			 1: GPGGA解析完毕
//       0: 没有进行解析，或数据无效
//====================================================================//
int GPS_GGA_Parse(char *line,GPS_INFO *GPS)
{
	uchar ch, status;
	char *buf = line;
	ch = buf[4];
	status = buf[GetComma(2, buf)];

	if (ch == 'G')  //$GPGGA
	{
		if (status != ',')
		{
			GPS->height_sea = Get_Float_Number(&buf[GetComma(9, buf)]);
			GPS->height_ground = Get_Float_Number(&buf[GetComma(11, buf)]);
			
			Int_To_Str( Get_Float_Number(&buf[GetComma(9, buf)]),sea1);
			OLED_P6x8Str(95,5,sea1);

			return 1;
		}
	}
	
	return 0;
}

//====================================================================//
// 函数:int GPS_GSV_Parse(char *line, GPS_INFO *GPS)  
// 功能:解析GPGSV信息
// 参数:存放原始信息数组，解析为可识别结构体
// 返回值:
//			    1: GPGSV解析完毕
//          0: 没有进行解析，或数据无效
//====================================================================//
int GPS_GSV_Parse(char *line,GPS_INFO *GPS)
{
	uchar ch, status;
	char *buf = line;
	ch = buf[5];
	status = buf[GetComma(2, buf)];

	if (ch == 'V')  //$GPGSV
	{
		GPS->satellite = Get_Int_Number(&buf[GetComma(3, buf)]);
		return 1;
	}
	
	return 0;
}

//====================================================================//
// 函数: static int Str_To_Int(char *buf)
// 功能: 把一个字符串转换为整数
// 参数: 字符串
// 返回值:转换后的整数
//====================================================================//
static int Str_To_Int(char *buf)
{
	int rev = 0;
	int dat;
	char *str = buf;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
		}

		rev = rev * 10 + dat;
		str ++;
	}

	return rev;
}

//====================================================================//
// 函数: static int Get_Int_Number(char *s)
// 功能:把给定字符串第一个逗号之前的字符转换为整型
// 参数:字符串
// 返回值:转换后的整型
//====================================================================//
static int Get_Int_Number(char *s)
{
	char buf[10];
	uchar i;
	int rev;
	i=GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev=Str_To_Int(buf);
	return rev;	
}

//====================================================================//
// 函数: static float Str_To_Float(char *buf)
// 功能: 把一个字符串转换成浮点数????????????
// 参数: 字符串
// 返回值:转化后的单精度值
//====================================================================//
static float Str_To_Float(char *buf)
{
	float rev = 0;
	float dat;
	int integer = 1;
	char *str = buf;
	int i;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str ++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str ++;
	}
	return rev;

}
												
//====================================================================//
// 函数: static float Get_Float_Number(char *s)
// 功能: 把给定字符串第一个逗号之后的字符串转换成单精度
// 参数: 字符串
// 返回值:转换后的单精度
//====================================================================//
static float Get_Float_Number(char *s)
{
	char buf[10];
	uchar i;
	float rev;
	i=GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev=Str_To_Float(buf);
	return rev;	
}

//====================================================================//
// 函数: static double Str_To_Double(char *buf)
// 功能: 把一个字符串转换成双精度浮点数
// 参数: 字符串
// 返回值:转换后的双精度浮点数
//====================================================================//
static double Str_To_Double(char *buf)
{
	double rev = 0;
	double dat;
	int integer = 1;
	char *str = buf;
	int i;
	while(*str != '\0')
	{
		switch(*str)
		{
			case '0':
				dat = 0;
				break;
			case '1':
				dat = 1;
				break;
			case '2':
				dat = 2;
				break;		
			case '3':
				dat = 3;
				break;
			case '4':
				dat = 4;
				break;
			case '5':
				dat = 5;
				break;
			case '6':
				dat = 6;
				break;
			case '7':
				dat = 7;
				break;
			case '8':
				dat = 8;
				break;
			case '9':
				dat = 9;
				break;
			case '.':
				dat = '.';
				break;
		}
		if(dat == '.')
		{
			integer = 0;
			i = 1;
			str ++;
			continue;
		}
		if( integer == 1 )
		{
			rev = rev * 10 + dat;
		}
		else
		{
			rev = rev + dat / (10 * i);
			i = i * 10 ;
		}
		str ++;
	}
	return rev;
}
												
//====================================================================//
// 函数: static double Get_Double_Number(char *s)
// 功能:把给定字符串第一个逗号之后的字符串转换成双精度
// 参数:字符串
// 返回值:转换后的双精度
//====================================================================//
static double Get_Double_Number(char *s)
{
	char buf[10];
	uchar i;
	double rev;
	i=GetComma(1, s);
	i = i - 1;
	strncpy(buf, s, i);
	buf[i] = 0;
	rev=Str_To_Double(buf);
	return rev;	
}

//====================================================================//
// 函数:static uchar GetComma(uchar num,char *str)
// 功能:计算字串中各个逗号的位置
// 参数:查找的逗号是第几个的个数，查找的字符串
// 返回值:0
//====================================================================//
static uchar GetComma(uchar num,char *str)
{
	uchar i,j = 0;
	int len=strlen(str);

	for(i = 0;i < len;i ++)
	{
		if(str[i] == ',')
			j++;
		if(j == num)
			return i + 1;	
	}

	return 0;	
}

//====================================================================//
// 函数:void UTC2BTC(DATE_TIME *GPS)
// 功能:转换时间为北京时区时间
// 参数:存放时间的结构体
// 返回值:无
//====================================================================//
static void UTC2BTC(DATE_TIME *GPS)
{
	GPS->second ++;  
	if(GPS->second > 59)
	{
		GPS->second = 0;
		GPS->minute ++;
		if(GPS->minute > 59)
		{
			GPS->minute = 0;
			GPS->hour ++;
		}
	}	

    GPS->hour = GPS->hour + 8;
	if(GPS->hour > 23)
	{
		GPS->hour -= 24;
		GPS->day += 1;
		if(GPS->month == 2 ||
		   		GPS->month == 4 ||
		   		GPS->month == 6 ||
		   		GPS->month == 9 ||
		   		GPS->month == 11 )
		{
			if(GPS->day > 30)
			{
		   		GPS->day = 1;
				GPS->month++;
			}
		}
		else
		{
			if(GPS->day > 31)
			{	
		   		GPS->day = 1;
				GPS->month ++;
			}
		}
		if(GPS->year % 4 == 0 )
		{
	   		if(GPS->day > 29 && GPS->month == 2)
			{		
	   			GPS->day = 1;
				GPS->month ++;
			}
		}
		else
		{
	   		if(GPS->day > 28 &&GPS->month == 2)
			{
	   			GPS->day = 1;
				GPS->month ++;
			}
		}
		if(GPS->month > 12)
		{
			GPS->month -= 12;
			GPS->year ++;
		}		
	}
}
//====================================================================//
//	函数:	Int_To_Str(int x,char *Str)
//	功能:	将整型转为字符串
//	参数:		x: 转换的整数
//				Str: 转换后的字符串
//	返回值:	无
//====================================================================//
void Int_To_Str(int x,char *Str)
{
	int t;
	char *Ptr,Buf[5];
	int i = 0;
	Ptr = Str;
	if(x < 10)		// 当整数小于10，转换为0x格式
	{
		*Ptr ++ = '0';
		*Ptr ++ = x+0x30;
	}
	else
	{
		while(x > 0)
		{
			t = x % 10;
			x = x / 10;
			Buf[i++] = t+0x30;	// 通过计算把数字编成ASCII码形式
		}
		i -- ;
		for(;i >= 0;i --) 		// 将得到的字符串倒序
		{
			*(Ptr++) = Buf[i];
		}
	}
	*Ptr = '\0';
}
