/*
 *  Author: Gonçalo Azevedo 93075
 *  File: structures.h
 */

struct node {
	struct node* prev, next;
};

typedef struct list {
	node* tail, head;	
} list;

typedef struct node node;
