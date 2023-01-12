#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

int days_in_month[]={0,31,28,31,30,31,30,31,31,30,31,30,31};

struct date {
	int date_year;
	int date_month;
	int date_day;
};

struct date getCurrentDate(void);
int detDayCode(int);
int detLeapYear(int);
void calendar(int year, int daycode, int monthnum);
void SetColorAndBackground(int, int);

char *months[]=
{
	" ",
	"January",
	"February",
	"March",
	"April",
	"May",
	"June",
	"July",
	"August",
	"September",
	"October",
	"November",
	"December"
};

int main(int argc, char *argv[])
{
	int year, daycode, leapyear, month;
	
	printf("\n");
	if (argc != 1)
	{
		year = atoi(argv[1]);
		month = atoi(argv[2]);
	}	
	else
	{
		struct date curdate;
		curdate = getCurrentDate();
		year = curdate.date_year;
		month = curdate.date_month;
	}
	daycode = detDayCode(year);
	detLeapYear(year);
	calendar(year, daycode, month);
	printf("\n\n");
}

int detDayCode(int year)
{
	int daycode;
	int d1, d2, d3;
	
	d1 = (year - 1.) / 4.0;
	d2 = (year - 1.) / 100.;
	d3 = (year - 1.) / 400.;
	daycode = (year + d1 - d2 + d3) % 7 - 1;
	if (daycode == -1)
		daycode = 6;
	return daycode;
}

int detLeapYear(int year)
{
	if(year % 4 == FALSE && year % 100 != FALSE || year % 400 == FALSE)
	{
		days_in_month[2] = 29;
		return TRUE;
	}
	else
	{
		days_in_month[2] = 28;
		return FALSE;
	}
}

void calendar(int year, int daycode, int monthnum)
{
	int month = monthnum, day;
	struct date DateCurrent = getCurrentDate();
	
	int highlight;
	if(DateCurrent.date_year == year && DateCurrent.date_month == month)
		highlight = TRUE;
	else
		highlight = FALSE;

	printf("%s %d", months[month], year);
	printf("\n\nMo Tu We Th Fr Sa Su\n");
		
	for(int k = 1; k < month; k++)
		daycode = (daycode + days_in_month[k]) % 7;
	for (day = 1; day <= daycode * 3; day++)
		printf(" ");
		
	for (day = 1; day <= days_in_month[month]; day++)
	{
		if (highlight == TRUE && DateCurrent.date_day == day)
		{
			SetColorAndBackground(0, 7);
			printf("%2d", day);
			SetColorAndBackground(6, 0);
		}
		else
			printf("%2d", day);
			
		if ((day + daycode) % 7 > 0)
			printf(" ");
		else
			printf("\n");
	}
		
}

struct date getCurrentDate(void)
{
	struct date CurrentDate;
	time_t now;

	time(&now);

	struct tm *local = localtime(&now);

	CurrentDate.date_day = local->tm_mday;
	CurrentDate.date_month = local->tm_mon + 1;
	CurrentDate.date_year = local->tm_year + 1900;
	
	return CurrentDate;
}

void SetColorAndBackground(int ForgC, int BackC)
{
     WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);;
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
     return;
}
