/*  Author: Gonçalo Azevedo 93075
 *  File: structures.c
 */
#include "structures.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * Initializes list of reservations
 */
void init_list(list *l)
{
	l->head = NULL;
}


/* 
 * Adds new reservation node given as argument to the end of the list as argument
 */
void add_node(list *l, reservation *new_node)
{
	if(l->head == NULL) {
		l->head = new_node;
		new_node->next = NULL;
	} else {
		new_node->next = l->head;
		l->head = new_node;
	}
}


/*
 * Removes node with given id from list and frees all removed node's associated memory
 * Returns number of passengers of removed node
 * Returns -1 if node doesn't exist
 */
int remove_node(list *l, char *id)
{
	int n;
	reservation *node = l->head;
	reservation *aux = NULL;
	
	/* search for node to be removed */
	while(node != NULL && strcmp(node->id, id)) {
		aux = node;
		node = node->next;
	}

	/* no node with given id */
	if(node == NULL)
		return -1;

	/* if single element list, list becomes empty */
	if(l->head->next == NULL) {
		l->head = NULL;
	} else {
		/* if first element is to be removed */
		if (aux == NULL) { 
			l->head = node->next;
		} else {
			aux->next = node->next;
		}
	}

	/* get nr_passengers from removed node */
	n = node->nr_passengers;
	/* free memory of removed node structure */
	free(node->id);
	free(node);

	return n;
}


/*
 * Prints all reservations in given list
 */
void print_list(list *l)
{
	reservation *node = l->head;

	while(node != NULL) {
		printf(PRINT_RESERVATION_STR, node->id, node->nr_passengers);
		node = node->next;
	}
}


/*
 * Destroys given list by freeing all associated memory
 */
void destroy_list(list *l)
{
	reservation *node = l->head;
	
	while(node != NULL) {
		reservation* aux = node;
		node = aux->next; 

		free(aux->id);
		free(aux);
	}
	free(l);
}


/*
 * Returns pointer to node with given id in given list
 * Returns NULL pointer if node with given id doesn't exist
 */
reservation *find_node_in_list(list *l, char *id)
{
	reservation *node = l->head; 

	while(node != NULL) {
		if(!strcmp(node->id, id))
			break;
		node = node->next;
	}
	
	return node;
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

	/* get right pointer pointing to the middle element of given list */
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


/* 
 * Merges two lists with head pointers given as arguments and returns head pointer to merged list
 */
reservation *merge(reservation *h1, reservation *h2)
{
	reservation *aux;  /* auxiliary pointer to traverse */
	reservation *head; /* head of sorted list */
	
	/* determine head element from given sub lists */
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

