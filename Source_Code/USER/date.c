extern int date,month;
extern int year;
#define isPrime(year) ((year%4==0&&year%100!=0)||(year%400==0))

int CEEflag=0,flag=0;;
struct date
{
	int year;
	int month;
	int day;
};
int datecounter(void)
{	
	//	int isPrime(int year);
	int dateDiff(struct date mindate,struct date maxdate);
	struct date mindate,maxdate;
	int days;
	
	//scanf("%i-%i-%i",&mindate.year,&mindate.month,&mindate.day);
	if(month==01)
		month=1;
		if(month==02)
		month=2;
			if(month==03)
		month=3;
				if(month==04)
		month=4;
					if(month==05)
		month=5;
						if(month==06)
		month=6;
							if(month==07)
		month=7;

	if(date==01)
		date=1;
		if(month==02)
		date=2;
			if(date==03)
		date=3;
				if(date==04)
		date=4;
					if(date==05)
		date=5;
						if(date==06)
		date=6;
							if(date==07)
		date=7;

	mindate.year=year;
	mindate.month=month;
	mindate.day=date;

	maxdate.year=2018;
	maxdate.month=6;
	maxdate.day=7;
	days=dateDiff(mindate,maxdate);
							
  if(year==2018&&month==6&&(date==7||date==8))
		flag=1;
	else 
		flag=0;
	
	if(CEEflag==0&&flag==0)
	{		
	OLED_P6x8Str(0,7,"To 2018 CEE : ");
	OLED_Write_Num_three(80,7,dateDiff(mindate,maxdate));
	OLED_P6x8Str(100,7,"Days");
	}
	if(CEEflag==1&&flag==0)
	{
	OLED_P6x8Str(0,7,"New journey is coming");
	
	} 
    
	if(flag==1)
	{
		OLED_P6x8Str(18,6,"Believe yourself");
		OLED_P6x8Str(33,7,"do the best");
	
	}
		
}

int dateDiff(struct date mindate,struct date maxdate)
{
	int days=0,j,flag;
	const int primeMonth[][12]={{31,28,31,30,31,30,31,31,30,31,30,31},{31,29,31,30,31,30,31,31,30,31,30,31}};	
	/************************************************************************/
	/*        交换两个日期函数,将小的日期给mindate,将大的日期给maxdate     */
    /************************************************************************/
	struct date tmp;
	if ((mindate.year>maxdate.year)|| (mindate.year==maxdate.year&&mindate.month>maxdate.month)||(mindate.year==maxdate.year&&mindate.month==maxdate.month&&mindate.day>maxdate.day))
	{
		tmp=mindate;
		mindate=maxdate;
		maxdate=tmp;
		CEEflag=1;
	} 
    /************************************************************************/
    /*  从mindate.year开始累加到maxdate.year                                */
    /************************************************************************/
	for(j=mindate.year;j<maxdate.year;++j)
		days+=isPrime(j)?366:365;
	
	//如果maxdate.year是闰年,则flag=1,后面调用primeMonth[1][12]
	flag=isPrime(maxdate.year);
	//加上maxdate.month到1月的天数
	for (j=1;j<maxdate.month;j++)
		days+=primeMonth[flag][j-1];

	//减去mindate.month到1月的天数

        flag=isPrime(maxdate.year);
        for (j=1;j<mindate.month;j++)
          days-=primeMonth[flag][j-1];
        days=days+maxdate.day-mindate.day;
        return days;
} 


