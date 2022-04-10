/*  Author: Gonçalo Azevedo 193075
 *  File: header.h
 */
#include "structures.h"

#ifndef _HEADER_
#define _HEADER_

/* global init date config */
#define DAY_0 1
#define MONTH_0 1
#define YEAR_0 2022

/* global structure output messages */
#define FORWARD_DATE_ERR "invalid date\n"
#define MAX_COMMAND_SIZE 65535

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
#define PRINT_DATE_STR "%02hd-%02hd-%hd"
#define PRINT_TIME_STR "%02hd:%02hd"
/* time and date constants */
#define MINUTES_IN_HOUR 60
#define MINUTES_IN_DAY 1440
#define HOURS_IN_DAY 24
#define DAYS_IN_YEAR 365

/* reservation output messages */
#define ADD_RESERVATION_ERR1 "invalid reservation code\n"
#define ADD_RESERVATION_ERR2 "%s: flight does not exist\n"
#define ADD_RESERVATION_ERR3 "%s: flight reservation already used\n"
#define ADD_RESERVATION_ERR4 "too many reservations\n"
#define ADD_RESERVATION_ERR5 "invalid date\n"
#define ADD_RESERVATION_ERR6 "invalid passager number\n"
#define ADD_RESERVATION_SUCCESS "%s %d\n"

/* eliminate command error messages */
#define ELIMINATE_ERR "not found\n"

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
	int nr_flights;
} airport;

typedef struct {
	char id[FLIGHT_LENGTH_ID];
	int capacity;
	char destination[AIRPORT_LENGTH_ID];
	char origin[AIRPORT_LENGTH_ID];
	date date;
	time time;
	time duration;
	int departure_schedule;
	int arrival_schedule;
	list *reservations;     /* pointer to list structure with flight's reservations addresses */
	int nr_passengers;
	int nr_reservations;
} flight;


/* global structure */
typedef struct {
	int nr_airports;
	int nr_flights;
	int nr_reservations;
	date date;
	airport airports[MAX_AIRPORTS];               /* list of system's airports alphabetically sorted */
	flight flights[MAX_FLIGHTS];                  /* list of system's flights sorted by creation date */
} manager;

/* proj1.c functions */

/* handle functions manage input, buffers and output feedback such as error or success messages */
int command_handler(manager *system);

void handle_add_airport(manager *system);

void handle_list_airports(manager *system);

void handle_flights(manager *system);

void handle_add_flight(manager *system);

void handle_list_flight_by_airport(manager *system, char command);  /* handles both 'c' and 'p' commands */

void handle_forward_date(manager *system);

void handle_reservations(manager *system); 

void handle_add_reservation(manager *system, char *buffer, char *flight_id, date *d);

void handle_eliminate(manager *system);

/* initializes global structure that stores all of current session's useful information */
void initialize(manager *system);

void free_all_memory(manager *system);

void terminate_program(manager *system);

int forward_date(manager *system, date *d);

int eliminate(manager *system, char *id, int size);

void bubblesort(manager *system, int *index_list, int size, int(*cmp_func)(manager *system, int a, int b));


/* airports.c functions */
int add_airport(manager *system, char *id, char *country, char *city);

void list_airports(manager *system);

void list_airports_by_id(manager *system);

int cmp_airports(manager *system, int a, int b);

airport *get_airport_by_id(manager *system, char *id);

/* auxiliary functions */
void create_airport(manager *system, char *id, char *country, char *city);

int is_valid_airport_id(char *id);

int exists_airport_id(manager *system, char *id);

void print_airport(airport *a);


/* flights.c functions */
int add_flight(manager *system, char *id, char *origin, char *destination,
		date *d, time *t, time *dur, int nr_passengers);

void list_flights(manager *system);

int list_flights_by_airport(manager *system, char *ariport, char command);

void list_airport_flights_by_departure(manager *system, char *airport);

void list_airport_flights_by_arrival(manager *system, char *airport);  

int eliminate_flight(manager *system, char *id);

 /* auxiliary functions */
void create_flight(manager *system, char *id, char *origin, char *destination,
		date *d, time *t, time *dur, int nr_passengers);

int is_valid_flight_id(char *id);

int exists_flight_id(manager *system, char *id);

void print_flight(flight flight);

void print_departing_flight(manager *system, int index);

void print_arriving_flight(manager *system, int index);

flight *get_flight_by_id_and_date(manager *system, char *id, date *d);

/* compare functions used by sorting insertion algorithm */
int compare_flight_departure(manager *system, int a, int b);

int compare_flight_arrival(manager *system, int a, int b);


/* schedule.c functions */
int convert_time_to_int(const time *);

int convert_date_to_int(const date *);

int convert_date_time_to_int(const date *, const time *);

int is_valid_date(const date *current, const date *date);

int is_valid_duration(const time *);

date convert_int_to_date(int n);

time convert_int_to_time(int n);

void print_date(const date *);

void print_time(const time *);


/* reservation.c functions */
void add_reservation(manager *system, flight *f, char *reservation_id, int nr_passengers); 

void list_reservations(manager *system, char *flight_id, date *d);

int eliminate_reservation(manager *system, char *id);

reservation *get_reservation_by_id(manager *system, char *reservation_id);

/* auxiliary functions */
int validate_reservation(manager *system, char *buffer, char *flight, date *d, int nr_passengers);

int evaluate_reservation_id(char *buffer);

void read_date_and_flight_id(char **buffer, char *flight_id, date *d);

int read_reservation(char *buffer, char **reservation_id, int *nr_passengers);

void ignore_whitespaces(char **buffer);


/* utils */
int evaluate_string(char *buffer);

#endif
