/*  Author: Gonçalo Azevedo 193075
 *  File: schedule.c
 */
#include "header.h"

#include <stdio.h>

int convert_time_to_int(time t)
{
	return t.minute + t.hour * MINUTES_IN_HOUR;
}

int convert_date_to_int(date d)
{
	int acc_days[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

	return d.day + acc_days[d.month - 1] + (d.year - YEAR_0)*DAYS_IN_YEAR - 1;
}

int convert_date_time_to_int(date d, time t)
{
	return convert_date_to_int(d) * MINUTES_IN_DAY + convert_time_to_int(t);
}

/*
 * Returns 1 if given date is set a year or less after current system's date
 * Returns 0 elsewise
 */
int is_valid_date(date curr_date, date d)
{
	int n_date;
	date next_year_date = curr_date;
	next_year_date.year++;

	n_date = convert_date_to_int(d);
	
	return !(n_date < convert_date_to_int(curr_date) 
			|| n_date > convert_date_to_int(next_year_date)); 
}

/*
 * Returns 1 if given time is a duration of 12 or less hours
 * Returns 0 elsewise
 */
int is_valid_duration(time t)
{
	return !(t.hour > 12 || (t.hour == 12 && t.minute > 0));
}

date convert_int_to_date(int n)
{
	int i = 0;
	int acc_days[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
	date d;

	n = (n - (n % MINUTES_IN_DAY)) / MINUTES_IN_DAY;
	d.year = (n / DAYS_IN_YEAR) + YEAR_0;
	n = n - ((d. year - YEAR_0) * DAYS_IN_YEAR);

	while(i <= 11 && n >= acc_days[i]) 
		i++;
	d.month = i;
	d.day = n - acc_days[i - 1] + 1;

	return d;
}

time convert_int_to_time(int n)
{
	time t;

	t.minute = n % MINUTES_IN_HOUR;
	t.hour = ((n - t.minute) / (MINUTES_IN_HOUR) % HOURS_IN_DAY); 

	return t;
}

void print_date(date d)
{
	printf(PRINT_DATE_STR, d.day, d.month, d.year);
}

void print_time(time t)
{
	printf(PRINT_TIME_STR, t.hour, t.minute);
}
