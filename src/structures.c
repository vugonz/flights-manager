/*
 *  Author: Gonçalo Azevedo 93075
 *  File: structures.h
 */
#include "header.h"
#include <stdlib.h>

void init_list(res_l *l)
{
	l->tail = l->head = NULL;
};

void add_reservation(res_l *l)
{
	reservation *new_node = malloc(sizeof *new_node);
	
	/* no memory */
	if(new_node == NULL)
		return -1;
	
	/* add to end of list */
	new_node->prev = l->tail;
	new_node->next = NULL;
	l->tail = new_node;

	/* if first element */
	if(l->head == NULL)
		l->head == new_node;

	return 0;
};

void remove_reservation(res_l *l, reservation *node)
{
	if(node->prev == NULL) 
		l->head = node->next;
	else
		node->prev->next = node->next;

	node->next->prev = node->prev;

	if(node->next == NULL) {
		l->tail = node->prev;
	else {
		node->next->prev = node->prev;

	node->prev->next = node->next;

	free(node);
};

void free_all_memory(manager *system)
{
	int i;

	for(i = 0; i < system->nr_flights; ++i)
		destroy_list(&system->flights[i].res_l)
		
}

void destroy_list(res_l *l)
{
	reservation *p;
	
	p = l->head;

	while(p != NULL) {
		p = p->next;
		free(p->prev);
}

