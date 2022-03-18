/*	File: header.h
 *	Author: Gonçalo Azevedo ist193075
 */ 

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
#define AIRPORT_COMPONENTS_PARSE "%s %s %[a-zA-Z \t]"
#define AIRPORT_IDS_PARSE "%s%c"
#define AIRPORT_PRINT_STR "%s %s %s\n"
/* airport output messages */
#define ADD_AIRPORT_ERR_1 "too many airports\n"
#define ADD_AIRPORT_ERR_2 "invalid airport ID\n"
#define ADD_AIRPORT_ERR_3 "duplicate airport\n"
#define ADD_AIRPORT_SUCCESS "airport %s\n"
#define LIST_AIRPORTS_ERR "%s: no such airport ID\n"

/* flight constants & parse strings */
#define MAX_FLIGHTS 30000
#define FLIGHT_LENGTH_ID 7
#define FLIGHT_COMPONENTS_PARSE "%s %s %s" 
#define NR_PASSENGERS_PARSE "%hd"
#define PRINT_FLIGHT_STR "%s %s %s %hd-%hd-%hd %hd:%hd\n"
/* flight output messages */
#define ADD_FLIGHT_ERR_1 "too many flights\n"
#define ADD_FLIGHT_ERR_2 "invalid flight code\n"
#define ADD_FLIGHT_ERR_3 "flight already exists\n"
#define ADD_FLIGHT_ERR_4N5 "%s: no such airport ID\n"
#define ADD_FLIGHT_ERR_6 "invalid date\n" 
#define ADD_FLIGHT_ERR_7 "invalid duration\n"
#define ADD_FLIGHT_ERR_8 "invalid capacity\n"

/* time and date parse strings */
#define DATE_COMPONENTS_PARSE "%hd-%hd-%hd" 
#define TIME_COMPONENTS_PARSE "%hd:%hd"

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
	date date;
	time time;
	time duration;
} flight;

/* global structure */
typedef struct {
	short nr_airports;
	short nr_flights;
	date date;
	flight flights[MAX_FLIGHTS];
	airport airports[MAX_AIRPORTS];
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

/* airport.c functions */
airport create_airport(char *id, char *country, char *city);
airport *get_airport_by_id(manager *system, char *id);
int add_airport(manager *system, char *id, char *country, char *city);
int is_valid_airport_id(char *id);
int exists_airport_id(manager *system, char *id);
void insert_airport(airport *l, airport new_airport, int size);
void list_airports(manager *system);
void list_airports_by_id(manager *system);
void print_airport(airport airport);

/* flights.c functions */
flight create_flight(char *id, char *origin, char *destination,
		date flight_date, time flight_time, time flight_duration, int nr_passengers);
int add_flight(manager *system, char *id, char *origin, char *destination, 
		date flight_date, time flight_time, time flight_duration, int nr_passengers);
int is_valid_flight_id(char *id);
int is_taken_flight_id(manager *system, char *id, date date);
int exists_flight_id(manager *system, char *id);
void list_all_flights(manager *system);
void print_flight(flight flight);

/* date.c functions */
date create_date(short day, short month, short year);
int is_valid_date(date d1, date d2);
int date_compare(date d1, date d2);
int dates_year_apart(date d1, date d2); 
int same_day(date d1, date d2);
int same_month(date d1, date d2);
int same_year(date d1, date d2);

/* time.c functions */
time create_time(short hour, short minute);
int is_valid_duration(time duration);

#endif
