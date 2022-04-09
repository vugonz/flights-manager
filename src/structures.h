/*
 * Author: Gonçalo Azevedo 93075
 * File: structures.h
 *
 * This header contains the essential new features added to the second part of the project.
 * It facilitates the evaluation of the second part since the base wasn't too altered.
 */

/* reservation self referenced structure */
struct reservation {
	int nr_passengers;
	char *id;
	struct reservation *next;
};

/* reservation's list structure */
struct list {
	struct reservation *head;
	struct reservation *tail;
};

typedef struct reservation reservation;
typedef struct list list;

/* structure.c functions */
list *init_list(manager *system, list *l);

void add_reservation_to_list(list *l, reservation *new_node);

void remove_reservation(list *l, reservation *node);

void destroy_list(list *l);


/* reservation.c functions */
void add_reservation(manager *system, flight *f, char *reservation_id, int nr_passengers); 

void list_reservations(manager *system, char *flight_id, date *d);

reservation *get_reservation_by_id(manager *system, char *reservation_id);

reservation *find_node_in_list(list *l, char *id);

/* auxiliary functions */
int validate_reservation(manager *system, char *buffer, char *flight, date *d, int nr_passengers);

int evaluate_reservation_id(char *buffer);

void read_date_and_flight_id(char **buffer, char *flight_id, date *d);

void ignore_whitespaces(char **buffer);

/* merge sort functions */
reservation *sort_list(reservation *head, int size);

reservation *merge(reservation *left, reservation *right);



