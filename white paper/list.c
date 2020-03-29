#include "list.h"

Node *makeNode(int data){
	Node *pNew = (Node*)malloc(sizeof(Node));

	pNew->destID = data;
	pNew->pNext = NULL;

	return pNew;
}

void insertFront(List *pL, int data){
	Node *pNew = makeNode(data);

	pNew->pNext = pL->pHead;
	pL->pHead = pNew;
	pL->size++;

	return;
}

int getDestId(List *pL, int randM){
	Node *pCur = pL->pHead;
	int i = 0;
	
	while (i != randM){
		pCur = pCur->pNext;
		i++;
	}
	return pCur->destID;
}
void printList(List *pL){
	Node *pCur = pL->pHead;
	if (pL->size > 0){
		printf("%d", pCur->destID);
		if (pCur->pNext) {
			printf(", ");
			pCur = pCur->pNext;
		}
		else{
			return;
		}
		while(pCur){
			printf("%d",pCur->destID);
			if (pCur->pNext) {
				printf(", ");
			}
			pCur = pCur->pNext;
		}
	}
	else{
		printf(" None ");
	}
}
List * makeList(){
	return (List*)calloc(1, sizeof(List));
}
