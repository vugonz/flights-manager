/*
 * Author: Gonçalo Azevedo 93075
 * File: structures.h
 *
 * This header contains the essential new features added to the second part of the project.
 * It facilitates the evaluation of the second part since the base wasn't too altered.
 */
#ifndef _STRUCTURES_
#define _STRUCTURES_

#define PRINT_RESERVATION_STR "%s %d\n"

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

void init_list(list *l);

void add_node(list *l, reservation *new_node);

int remove_node(list *l, char *id);

void print_list(list *l);

void destroy_list(list *l);

reservation *find_node_in_list(list *l, char *id);

/* merge sort functions */
reservation *sort_list(reservation *head, int size);

reservation *merge(reservation *left, reservation *right);

#endif
