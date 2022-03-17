/*  File: header.h
 *	Author: Gonçalo Azevedo ist193075
 */ 

#ifndef HEADER
#define HEADER

#define MAX_AIRPORTS 40
#define AIRPORT_LENGTH_ID 4
#define AIRPORT_LENGTH_COUNTRY 31
#define AIRPORT_LENGTH_CITY 61
#define AIRPORT_COMPONENTS_PARSE "%s %s %[a-zA-Z \t]"
#define AIRPORT_IDS_PARSE "%s%c"

#define ADD_AIRPORT_ERR_1 "too many airports\n"
#define ADD_AIRPORT_ERR_2 "invalid airport ID\n"
#define ADD_AIRPORT_ERR_3 "duplicate airport\n"
#define ADD_AIRPORT_SUCCESS "airport %s\n"
#define LIST_AIRPORTS_ERR "%s: no such airport ID\n"

#define PRINT_AIRPORT "%s %s %s\n"

#define MAX_FLIGHTS 30000
#define FLIGHT_LENGTH_ID 7

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
	airport destination;
	airport origin;
	date date;
	time time;
} flight;

typedef struct {
	short nr_airports;
	short nr_flights;
	date date;
	flight flights[MAX_FLIGHTS];
	airport airports[MAX_AIRPORTS];
} manager;


/* proj1.c functions */
int command_handler(manager *system);
void handle_add_airport(manager *system);
void handle_list_airports(manager *system);

/* airport.c functions */
int add_airport(manager *system, char *id, char *country, char *city);
void insert_airport(airport *l, airport new_airport, int size);
void print_airport(airport airport);
void list_airports(manager *system);
void list_airports_by_id(manager *system);
int is_valid_airport_id(manager *system, char *id);
airport *get_airport_by_id(manager *system, char *id);

#endif
