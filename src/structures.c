/*
 *  Author: Gonçalo Azevedo 93075
 *  File: structures.c
 */
#include "header.h"
#include "structures.h"

#include <stdlib.h>
#include <string.h>

void init_list(list *l)
{
	l->tail = NULL;
	l->head = NULL;
}

void add_node(list *l, reservation *new_node)
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

int remove_node(list *l, char *id)
{
	int n;
	reservation *node = l->head;
	reservation *aux = NULL;
	
	while(node != NULL && strcmp(node->id, id)) {
		aux = node;
		node = node->next;
	}

	/* no node with given id */
	if(node == NULL)
		return -1;

	/* single element list */
	if(l->head == l->tail) {
		l->head = l->tail = NULL;
	} else {
		/* if first element is to be removed */
		if (aux == NULL) 
			l->head = node->next;
		/* if last element is to be removed */
		else if (node->next == NULL)
			l->tail = aux;

		aux->next = node->next;
	}

	n = node->nr_passengers;
	free(node->id);
	free(node);

	return n;
}

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

/*
 * Sorts given list of reservations in alphabetical order of reservation's id using mergesort
 */
reservation *sort_list(reservation *head, int size)
{
	int i;
	reservation *left = head;
	reservation *right = head;
	reservation *aux;
	
	/* empty list case or base case for recursive calls */
	if(size == 0 || size == 1)
		return head;

	/* get right pointing to middle list's element */
	for(i = 0; i < size - size/2 - 1; ++i)
		right = right->next;
	/* divide list */
	aux = right->next;
	right->next = NULL;
	right = aux;

	/* recursive division */
	left = sort_list(left, size - size/2);
	right = sort_list(right, size/2);
	/* return pointer of sorted sub division to previous stack */
	return merge(left, right);
}

reservation *merge(reservation *h1, reservation *h2)
{
	reservation *aux;  /* auxiliary pointer to traverse */
	reservation *head; /* head of sorted list */
	
	/* get first element in auxiliary pointer */
	aux = strcmp(h1->id, h2->id) < 0 ? h1 : h2;
	if(aux == h1)
		h1 = h1->next;
	else
		h2 = h2->next;
	
	/* save head pointer */
	head = aux;

	/* link elements in order until a sub list runs out of elements */
	while(h1 != NULL && h2 != NULL) {
		if(strcmp(h1->id, h2->id) < 0) {
			aux->next = h1;
			h1 = h1->next;
		} else {
			aux->next = h2;
			h2 = h2->next;
		}
		aux = aux->next;
	}

	/* link remaining elements */
	if(h1 != NULL) {
		while(h1 != NULL) {
			aux->next = h1;
			h1 = h1->next;
			aux = aux->next;
		}
	} else {
		while(h2 != NULL) {
			aux->next = h2;
			h2 = h2->next;
			aux = aux->next;
		}
	}
	return head;
}

