#include "header.h"

date create_date(short day, short month, short year)
{
	date new_date = {0};

	new_date.day = day;
	new_date.month = month;
	new_date.year = year;

	return new_date;
}

/* returns true if d2 is a date set a year or less after d1 */
int is_valid_date(date d1, date d2)
{
	return date_compare(d1, d2) >= 0 && !dates_year_apart(d1, d2); 
}

/* returns negative if d2 is a date set before d1, zero if d2 and d1 are the same date and positive if d2 is set after d1, similar to strcmp */
int date_compare(date d1, date d2)
{
	return same_day(d1, d2) ? 0 : same_month(d1, d2) ? d2.day - d1.day : same_year(d1, d2) ? d2.month - d1.month : d2.year - d1.year; 	
}

/* returns 1 if d2 is a year or least apart from d1 and 0 if not */
int dates_year_apart(date d1, date d2)
{
	if(d2.year - d1.year > 1) {
		return 1;
	} else if(d2.year - d1.year == 1) {
		if(d2.month > d1.month) { 
			return 1;
		} else if(d2.month == d1.month)
			if(d2.day > d1.day)
				return 1;
	}
	
	return 0;
}

/* auxiliary functions */
int same_day(date d1, date d2)
{
	return d1.day == d2.day && same_month(d1, d2) && same_year(d1, d2) ? 1 : 0;
}

int same_month(date d1, date d2)
{
	return d1.month == d2.month && same_year(d1, d2) ? 1 : 0;
}

int same_year(date d1, date d2)
{
	return d1.year == d2.year ? 1 : 0;
}

