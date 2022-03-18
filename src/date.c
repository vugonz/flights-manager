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
	return same_day(d1, d2) ? 0 : same_month(d1, d2) ? DIFF_DAYS(d2, d1) : same_year(d1, d2) ? DIFF_MONTHS(d2, d1) : DIFF_YEARS(d2, d1); 	
}

/* returns 1 if d2 is a year or least apart from d1 and 0 if not (only used with d2 being a date set after d1) */
int dates_year_apart(date d1, date d2)
{
	if(DIFF_YEARS(d2, d1) > 1) {
		return 1;
	} else if(DIFF_YEARS(d2,d1) == 1) {
		if(DIFF_MONTHS(d2, d1) { 
			return 1;
		} else if(DIFF_MONTHS(d2, d1) == 0)
			if(DIFF_DAYS(d2, d1) > 0)
				return 1;
	}
	
	return 0;
}

/* auxiliary functions */
int same_day(date d1, date d2)
{
	return !(DIFF_DAYS(d1,d2)) && same_month(d1, d2) && same_year(d1, d2) ? 1 : 0;
}

int same_month(date d1, date d2)
{
	return !(DIFF_MONTHS(d1,d2)) && same_year(d1, d2) ? 1 : 0;
}

int same_year(date d1, date d2)
{
	return !(DIFF_YEARS(d1, d2)) ? 1 : 0;
}

