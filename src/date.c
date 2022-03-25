/*  Author: Gonçalo Azevedo 193075
 *  File: date.c
 */
#include "header.h"
#include <stdio.h>

date create_date(short day, short month, short year)
{
	date new_date;

	new_date.day = day;
	new_date.month = month;
	new_date.year = year;

	return new_date;
}

/* returns 1 if d1 is a date set a year or less after d2 */
int is_valid_date(date d1, date d2)
{
	return date_compare(d1, d2) >= 0 && !dates_year_apart(d1, d2); 
}

/* returns negative if d1 is a date set before d2, zero if d1 and d2 are the same date and positive if d1 is set after d2 */
int date_compare(date d1, date d2)
{
	return same_month(d1, d2) ? DIFF_DAYS(d1, d2) :
			!(DIFF_YEARS(d1, d2)) ? DIFF_MONTHS(d1, d2) : DIFF_YEARS(d1, d2); 	
}

/* returns 1 if d1 is a year or least apart from d2 and 0 if not (only used with d1 being a date set after d2) */
int dates_year_apart(date d1, date d2)
{
	if(d1.year != d2.year)
		return d1.month < d2.month ? 0 : d1.month == d2.month ? d1.day - d2.day : 1;

	return 0;
}

/* auxiliary functions */
int same_day(date d1, date d2)
{
	return same_month(d1, d2) && !(DIFF_DAYS(d1, d2)) ? 1 : 0;
}

int same_month(date d1, date d2)
{
	return !(DIFF_YEARS(d1, d2)) && !(DIFF_MONTHS(d1, d2)) ? 1 : 0;
}

void print_date(date d)
{
	printf(PRINT_DATE_STR, d.day, d.month, d.year);
}
