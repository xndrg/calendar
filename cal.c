#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define YEAR_LEN 4

const char header[] = "Su Mo Tu We Th Fr Sa";
#define HEADER_LEN (sizeof(header) / sizeof(char) - 1)

typedef struct {
    char  *contents;
    size_t len;
} String;

typedef struct {
    String name;
    int    days;
} Month;

typedef struct {
    int left_gap;
    int right_gap;
} Gaps;

typedef struct {
    int   day;
    Month month;
    int   year;
    int   day_of_week; // [0, 6]
} Date;

bool is_leap_year(int year)
{
    if((year % 4 == 0 && year % 100 != 0 ) || (year % 400 == 0))
        return true;
    else
        return false;
}

Month get_month(int year, int month)
{
    switch (month) {
    case 0:  return (Month) {(String) {"January",   7}, 31};
    case 1: {
        int days;

        if (is_leap_year(year)) days = 29; else days = 28;

        return (Month) {(String) {"February",  8}, days};
    }
    case 2:  return (Month) {(String) {"March",     5}, 31};
    case 3:  return (Month) {(String) {"April",     5}, 30};
    case 4:  return (Month) {(String) {"May",       3}, 31};
    case 5:  return (Month) {(String) {"June",      4}, 30};
    case 6:  return (Month) {(String) {"July",      4}, 31};
    case 7:  return (Month) {(String) {"August",    6}, 31};
    case 8:  return (Month) {(String) {"September", 9}, 30};
    case 9:  return (Month) {(String) {"October",   7}, 31};
    case 10: return (Month) {(String) {"November",  8}, 30};
    case 11: return (Month) {(String) {"December",  8}, 31};
    default: return (Month) {(String) {"",          0}, 0 };
    }
}

Gaps get_gaps(String month) {
    int gap = HEADER_LEN - (month.len + YEAR_LEN + 1);
    int left_gap, right_gap;

    if (gap % 2 == 0) {
        left_gap = right_gap = gap / 2;
    } else {
        right_gap = gap / 2;
        left_gap = right_gap + 1;
    }

    return (Gaps) {left_gap, right_gap};
}

Date get_current_date()
{
    time_t t = time(NULL);
    struct tm *date_time = localtime(&t);

    int   year        = date_time->tm_year + 1900;
    Month month       = get_month(year, date_time->tm_mon);
    int   day         = date_time->tm_mday;
    int   day_of_week = date_time->tm_wday;

    return (Date) {day, month, year, day_of_week};
}

void print_calendar(Date date)
{
    // Gaps at the first line where we print month and year
    Gaps gaps = get_gaps(date.month.name);

    printf("%0*c%s %d%0*c\n",
            gaps.left_gap,  ' ',
            date.month.name.contents,
            date.year,
            gaps.right_gap, ' '); 

    printf("%s\n", header);

    // Maybe a bit strange way to find day of week of first day of given month
    // We can use for loop for doing this, probably it will me more clear, but idc
    int day_of_month = (date.day_of_week - date.day) + (date.day / 7) * 7 + 1;

    for (int i = 0; i < day_of_month; ++i)
        printf("%2c ", ' ');

    for (int i = 1; i <= date.month.days; ++i) {
        if (i == date.day) {
            // Using ANSI escape codes to change background color of current day
            printf("\033[30;107m%2d\033[0m ", i);
        } else
            printf("%2d ", i);

        if (day_of_month == 6) {
            day_of_month = 0;
            printf("\n");
        } else
            day_of_month++;
    }

    printf("\n");
}

int main(void)
{
    Date current_date = get_current_date();
    print_calendar(current_date);

    return 0;
}
