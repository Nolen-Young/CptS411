#ifndef LIST_H
#define LIST_H
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	int destID;
	struct node * pNext;
} Node;

typedef struct list {
	int size;
	int id;
	Node *pHead;
} List;

typedef struct graph{
	List *nodeArray;
	int numNodes;
} Graph;

Node * makeNode(int data);

void insertFront(List *pList, int data);
void printList(List *pL);
int getDestId(List *pL, int randM);

#endif