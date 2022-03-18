/*	File: header.h Author: Gonçalo Azevedo ist193075 */ 

#ifndef HEADER
#define HEADER

/* global init date config macros */
#define DAY_0 1
#define MONTH_0 1
#define YEAR_0 2022
/* global output messages */
#define FORWARD_DATE_ERR "invalid date"

/* airport constants & parse strings */
#define MAX_AIRPORTS 40
#define AIRPORT_LENGTH_ID 4
#define AIRPORT_LENGTH_COUNTRY 31
#define AIRPORT_LENGTH_CITY 61
#define MIN_FLIGHT_CAPACITY 10
#define MAX_FLIGHT_CAPACITY 100
#define AIRPORT_COMPONENTS_PARSE "%s %s %[a-zA-Z \t]"
#define AIRPORT_IDS_PARSE "%s%c"
/* airport output messages */
#define AIRPORT_PRINT_STR "%s %s %s\n"
#define ADD_AIRPORT_ERR_1 "too many airports\n"
#define ADD_AIRPORT_ERR_2 "invalid airport ID\n"
#define ADD_AIRPORT_ERR_3 "duplicate airport\n"
#define ADD_AIRPORT_SUCCESS "airport %s\n"
#define LIST_AIRPORTS_ERR "%s: no such airport ID\n"

/* flight constants & parse strings */
#define MAX_FLIGHTS 30000
#define FLIGHT_LENGTH_ID 7
#define FLIGHT_MAX_HOUR_DURATION 12
#define FLIGHT_COMPONENTS_PARSE "%s %s %s" 
#define NR_PASSENGERS_PARSE "%hd"
#define PRINT_FLIGHT_STR "%s %s %s %02d-%02d-%d %02hd:%02hd\n"
/* flight output messages */
#define ADD_FLIGHT_ERR_1 "too many flights\n"
#define ADD_FLIGHT_ERR_2 "invalid flight code\n"
#define ADD_FLIGHT_ERR_3 "flight already exists\n"
#define ADD_FLIGHT_ERR_4N5 "%s: no such airport ID\n"
#define ADD_FLIGHT_ERR_6 "invalid date\n" 
#define ADD_FLIGHT_ERR_7 "invalid duration\n"
#define ADD_FLIGHT_ERR_8 "invalid capacity\n"
#define LIST_FLIGHTS_BY_AIRPORT_ID_ERR "%s\n"

/* time and date parse strings */
#define DATE_COMPONENTS_PARSE "%hd-%hd-%hd" 
#define TIME_COMPONENTS_PARSE "%hd:%hd"

/* date.c constants and macros */
#define MAX_MONTHS 12
#define SUM_DAYS(a, b) a.day + b.day
#define SUM_MONTHS(a, b) a.month + b.month
#define SUM_YEARS(a, b) a.year + b.year
#define DIFF_DAYS(a, b) a.day - b.day
#define DIFF_MONTHS(a, b) a.month - b.month
#define DIFF_YEARS(a, b) a.year - b.year
#define DAYS_IN_MONTH(n) n == 2 ? 28 : (n == 4 || n == 6 || n = 9 || n = 11) ? 30 : 31;
#define INC(a) a + 1

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
	char id[AIRPORT_LENGTH_ID];
	char country[AIRPORT_LENGTH_COUNTRY];
	char city[AIRPORT_LENGTH_CITY];
} airport;

typedef struct {
	char id[FLIGHT_LENGTH_ID];
	short nr_passengers;
	char destination[AIRPORT_LENGTH_ID];
	char origin[AIRPORT_LENGTH_ID];
	date date_departure;
	date date_arrival;
	time time_departure;
	time time_arrival;
} flight;

/* global structure */
typedef struct {
	short nr_airports;
	short nr_flights;
	date date;
	airport airports[MAX_AIRPORTS];
	flight flights[MAX_FLIGHTS];					/* list of system's flights, also the list of flights sorted by creation */
	flight sorted_departure_flights[MAX_FLIGHTS];	/* list of system's flights sorted by departure time */
	flight sorted_arrival_flights[MAX_FLIGHTS];		/* list of system's flights sorted by arrival time */
} manager;

/* proj1.c functions */
/* handle functions serve the purpose of handling input & buffers and displaying output feedback such as error or success messages */
int command_handler(manager *system);
void handle_add_airport(manager *system);
void handle_list_airports(manager *system);
void handle_v_command(manager *system);
void handle_add_flight(manager *system);
void handle_forward_date(manager *system);
/* initializes global structure that stores all of current session's useful information */
manager *initialize();
/* forwards global date to date given as argument */
int forward_date(manager *system, date);

/*
 * airport.c functions
 */
int add_airport(manager *system, char *id, char *country, char *city);
int is_valid_airport_id(char *id);
int exists_airport_id(manager *system, char *id);
void insert_airport(airport *l, airport new_airport, int size);
void list_airports(manager *system);
void list_airports_by_id(manager *system);
void print_airport(airport airport);
airport create_airport(char *id, char *country, char *city);
airport *get_airport_by_id(manager *system, char *id);

/*
 * flights.c functions
 */
flight create_flight(char *id, char *origin, char *destination,
		date flight_date, time flight_time, time flight_duration, int nr_passengers);
int add_flight(manager *system, char *id, char *origin, char *destination, 
		date flight_date, time flight_time, time flight_duration, int nr_passengers);
int is_valid_flight_id(char *id);
int is_taken_flight_id(manager *system, char *id, date date);
int exists_flight_id(manager *system, char *id);
int compare_flight_departure(flight f1, flight f2);
void insert_flight(flight *l, flight *sorted_l, flight new_flight, int size);
void list_all_flights(manager *system);
void print_flight(flight flight);

/*
 * date.c functions
 */
date create_date(short day, short month, short year);
int is_valid_date(date d1, date d2);
int date_compare(date d1, date d2);
int dates_year_apart(date d1, date d2); 
/* auxiliary functions */
int same_day(date d1, date d2);
int same_month(date d1, date d2);
int same_year(date d1, date d2);

/*
 * time.c functions
 */
time create_time(short hour, short minute);
int time_compare(time t1, time t2);
int is_valid_duration(time duration);
void add_time(date *d, time *t, time t_inc);
/* auxiliary functions */
int same_time(time t1, time t2);
int same_hour(time t1, time t2);

#endif
