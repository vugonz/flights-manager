#ifndef HEADER
#define HEADER


#define AIRPORT_ID_LENGTH 4
#define AIRPORT_COUNTRY_LENGTH 31
#define AIRPORT_CITY_LENGTH 61
#define MAX_AIRPORTS 40 
#define AIRPORT_COMPONENTS_PARSE "%s %s %[a-zA-Z ]"

#define ADD_AIRPORT_ERROR_1 "too many airports" 
#define ADD_AIRPORT_ERROR_2 "invalid airport ID"
#define ADD_AIRPORT_ERROR_3 "duplicate airport"
#define ADD_AIRPORT_SUCCESS "airport %s\n"

#define FLIGHT_ID_LENGTH 7

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
	char id[AIRPORT_ID_LENGTH];
	char country[AIRPORT_COUNTRY_LENGTH];
	char city[AIRPORT_CITY_LENGTH];
} airport;

typedef struct {
	char id[FLIGHT_ID_LENGTH];
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
	flight flights[40];
	airport airports[40];
} manager;

int command_handler(manager *system);
void handle_add_airport(manager *system);
int add_airport(manager *system, char *id, char *country, char *city);
#endif
