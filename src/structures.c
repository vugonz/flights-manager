/*
 *  Author: Gonçalo Azevedo 93075
 *  File: structures.h
 */
#include "header.h"
#include <stdlib.h>
#include <string.h>

list *init_list(manager *system, list *l)
{
	l = (list *)malloc(sizeof(list));

	if(l == NULL)
		terminate_program(system);

	l->tail = NULL;
	l->head = NULL;

	return l;
}

void add_reservation_to_list(list *l, reservation *new_node)
{
	/* add to end of list */
	new_node->next = NULL;
	
	if(l->tail != NULL)
		l->tail->next = new_node;

	l->tail = new_node;

	/* if first element */
	if(l->head == NULL)
		l->head = new_node;
}
/*
void remove_reservation(list *l, reservation *node)
{
	if(node->prev == NULL) 
		l->head = node->next;
	else
		node->prev->next = node->next;

	node->next->prev = node->prev;

	if(node->next == NULL)
		l->tail = node->prev;
	else 
		node->next->prev = node->prev;

	node->prev->next = node->next;

	free(&(node->id));
	free(node);
}
*/

void destroy_list(list *l)
{
	reservation *p = l->head;
	
	while(p != NULL) {
		reservation* aux = p;
		p = aux->next; 

		free(aux->id);
		free(aux);
	}

	free(l);
}

reservation *find_node_in_list(list *l, char *id)
{
	reservation *p = l->head; 

	while(p != NULL) {
		if(!strcmp(p->id, id))
			break;
		p = p->next;
	}
	
	return p;
}

