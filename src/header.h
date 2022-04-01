/*  Author: Gonçalo Azevedo 193075
 *  File: header.h
 */
#ifndef _HEADER_
#define _HEADER_

/* global init date config */
#define DAY_0 1
#define MONTH_0 1
#define YEAR_0 2022
/* global structure output messages */
#define FORWARD_DATE_ERR "invalid date\n"

/* airport constants & parse strings */
#define MAX_AIRPORTS 40
#define AIRPORT_LENGTH_ID 4
#define AIRPORT_LENGTH_COUNTRY 31
#define AIRPORT_LENGTH_CITY 61
#define AIRPORT_MEMBERS_PARSE "%s %s %[a-zA-Z- \t]"
/* airport output messages */
#define AIRPORT_PRINT_STR "%s %s %s %d\n"
#define ADD_AIRPORT_ERR_1 "invalid airport ID\n"
#define ADD_AIRPORT_ERR_2 "too many airports\n"
#define ADD_AIRPORT_ERR_3 "duplicate airport\n"
#define ADD_AIRPORT_SUCCESS "airport %s\n"
#define LIST_AIRPORTS_ERR "%s: no such airport ID\n"

/* flight constants & parse strings */
#define DEPARTURE 'p'
#define MAX_FLIGHTS 30000
#define FLIGHT_LENGTH_ID 7
#define FLIGHT_MAX_NUM_ID 10000
#define FLIGHT_MAX_HOUR_DURATION 12
#define MIN_FLIGHT_CAPACITY 10
#define MAX_FLIGHT_CAPACITY 100
#define FLIGHT_MEMBERS_PARSE "%s %s %s" 
#define PRINT_FLIGHT_STR "%s %s %s %02hd-%02hd-%hd %02hd:%02hd\n"
#define PRINT_FLIGHT_IN_AIRPORT_STR "%s %s %02hd-%02hd-%hd %02hd:%02hd\n"
/* flight output messages */
#define ADD_FLIGHT_ERR_1 "invalid flight code\n"
#define ADD_FLIGHT_ERR_2 "flight already exists\n"
#define ADD_FLIGHT_ERR_3N4 "%s: no such airport ID\n"
#define ADD_FLIGHT_ERR_5 "too many flights\n"
#define ADD_FLIGHT_ERR_6 "invalid date\n" 
#define ADD_FLIGHT_ERR_7 "invalid duration\n"
#define ADD_FLIGHT_ERR_8 "invalid capacity\n"
#define LIST_FLIGHTS_BY_AIRPORT_ID_ERR "%s\n"

/* time and date parse strings */
#define DATE_MEMBERS_PARSE "%hd-%hd-%hd" 
#define TIME_MEMBERS_PARSE "%hd:%hd"
#define PRINT_DATE_STR "%02hd-%02hd-%hd\n"

/* date.c constants and macros */
#define MAX_MONTHS 12
#define DIFF_DAYS(a, b) a.day - b.day
#define DIFF_MONTHS(a, b) a.month - b.month
#define DIFF_YEARS(a, b) a.year - b.year
#define DAYS_IN_MONTH(n)\
	(n == 2 ? 28 : (n == 4 || n == 6 || n == 9 || n == 11) ? 30 : 31)

/* time.c constants and macros */
#define MAX_HOURS 24
#define MAX_MINUTES 60
#define SUM_MINUTES(a, b) a.minute + b.minute
#define SUM_HOURS(a, b) a.hour + b.hour
#define DIFF_MINUTES(a, b) a.minute - b.minute
#define DIFF_HOURS(a, b) a.hour - b.hour

/* structures */
typedef struct {
	short hour;
	short minute;
} time;

typedef struct {
	short day;
	short month;
	short year;
} date;

typedef struct {
	time time;
	date date;
} schedule;

typedef struct {
	char id[AIRPORT_LENGTH_ID];
	char country[AIRPORT_LENGTH_COUNTRY];
	char city[AIRPORT_LENGTH_CITY];
	int nr_flights;
} airport;

typedef struct {
	char id[FLIGHT_LENGTH_ID];
	int nr_passengers;
	char destination[AIRPORT_LENGTH_ID];
	char origin[AIRPORT_LENGTH_ID];
	schedule departure;
	schedule arrival;
} flight;

/* global structure */
typedef struct {
	int nr_airports;
	int nr_flights;
	date date;
	airport airports[MAX_AIRPORTS];               /* list of system's airports alphabetically sorted */
	flight flights[MAX_FLIGHTS];                  /* list of system's flights sorted by creation time */
	flight sorted_departure_flights[MAX_FLIGHTS]; /* list of system's flights sorted by departure time */
	flight sorted_arrival_flights[MAX_FLIGHTS];   /* list of system's flights sorted by arrival time */
} manager;

/* proj1.c functions */

/* handle functions manage input, buffers and output feedback such as error or success messages */
int command_handler(manager *system);
void handle_add_airport(manager *system);
void handle_list_airports(manager *system);
void handle_v_command(manager *system);
void handle_add_flight(manager *system);
void handle_list_flight_by_airport(manager *system, char command);  /* handles both 'c' and 'p' commands */
void handle_forward_date(manager *system);
/* initializes global structure that stores all of current session's useful information */
manager *initialize();
int forward_date(manager *system, date);


/* airports.c functions */

int add_airport(manager *system, char *id, char *country, char *city);
void insert_airport(airport *l, airport new_airport, int size);
void list_airports(manager *system);
void list_airports_by_id(manager *system);
/* auxiliary functions */
airport create_airport(char *id, char *country, char *city);
airport *get_airport_by_id(manager *system, char *id);
int is_valid_airport_id(char *id);
int exists_airport_id(manager *system, char *id);
void print_airport(airport airport);


/* flights.c functions */

int add_flight(manager *system, char *id, char *origin, char *destination, 
		schedule departure, time duration, int nr_passengers);
void insert_flight(manager *system, flight new_flight);
void insert_sorted_flight(flight *l, flight new_flight, int size, int (*cmp_fn) (flight f1, flight f2)); 
void list_flights(manager *system);
int list_flights_by_airport(manager *system, char *ariport, char command);
void list_airport_flights_by_departure(flight *l, char *airport, int size);  
void list_airport_flights_by_arrival(flight *l, char *airport, int size);  
 /* auxiliary functions */
flight create_flight(char *id, char *origin, char *destination,
		schedule departure, time duration, int nr_passengers);
int is_valid_flight_id(char *id);
int is_taken_flight_id(manager *system, char *id, date date);
int exists_flight_id(manager *system, char *id);
void print_flight(flight flight);
void print_flight_in_airport(char *id, char *airport, schedule s);
/* cmp functions used by sorted insertion algorithm */
int compare_flight_departure(flight f1, flight f2);
int compare_flight_arrival(flight f1, flight f2);


/* schedule.c functions */

schedule create_schedule(time t, date d);
schedule calculate_arrival(schedule s, time duration);
int schedule_cmp(schedule s1, schedule s2);


/* date.c functions */

date create_date(short day, short month, short year);
int is_valid_date(date d1, date d2);
int date_cmp(date d1, date d2);
int dates_year_apart(date d1, date d2); 
/* auxiliary funcions */
int same_day(date d1, date d2);
int same_month(date d1, date d2);
void print_date(date);


/* time.c functions */

time create_time(short hour, short minute);
int is_valid_duration(time duration);
int time_cmp(time t1, time t2);
/* auxiliary functions */
int same_minute(time t1, time t2);
int same_hour(time t1, time t2);

#endif
