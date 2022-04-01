/*  Author: Gonçalo Azevedo 193075
 *  File: date.c
 */
#include "header.h"
#include <stdio.h>

/*
 * Returns a date structure with given arguments as memmbers 
 */
date create_date(short day, short month, short year)
{
	date new_date;

	new_date.day = day;
	new_date.month = month;
	new_date.year = year;

	return new_date;
}

/*
 * Returns 1 if d1 is a date set a year or less after d2
 * Returns 0 elsewise 
 */
int is_valid_date(date d1, date d2)
{
	return date_cmp(d1, d2) >= 0 && !dates_year_apart(d1, d2); 
}

/*
 * Returns negative if d1 is a date set before d2
 * Returns 0 if d1 and d2 are the same date
 * Returns positive if d1 is a date set after d2
 */
int date_cmp(date d1, date d2)
{
	return same_month(d1, d2) ? DIFF_DAYS(d1, d2) :
			!(DIFF_YEARS(d1, d2)) ? DIFF_MONTHS(d1, d2) : DIFF_YEARS(d1, d2); 	
}

/* 
 * Returns 1 if d1 is a year or less apart from d2
 * Returns 0 elsewise
 */
int dates_year_apart(date d1, date d2)
{
	if(d1.year != d2.year)
		return d1.month < d2.month ? 0 : 
			d1.month == d2.month ? d1.day - d2.day : 1;

	return 0;
}

/*
 * Returns 1 if given dates are the same
 * Returns 0 elsewise
 * -- More efficient than date_cmp for simple check in flights.c members validation
 */ 
int same_day(date d1, date d2)
{
	return same_month(d1, d2) && !(DIFF_DAYS(d1, d2)) ? 1 : 0;
}

/*
 * Returns 1 if given dates share the same year and month
 * Returns 0 elsewise
 */
int same_month(date d1, date d2)
{
	return !(DIFF_YEARS(d1, d2)) && !(DIFF_MONTHS(d1, d2)) ? 1 : 0;
}

/*
 * Formatted print of date structure
 */
void print_date(date d)
{
	printf(PRINT_DATE_STR, d.day, d.month, d.year);
}
