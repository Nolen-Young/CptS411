#ifndef PGRNK_H
#define PGRNK_H
#define _CRT_SECURE_NO_WARNINGS
#include <omp.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef enum coinface {
	TAILS, HEADS
}COINFACE;

COINFACE tossWeightedCoin(double dampingVal);
void printGraph(Graph *g);
void readGraph(Graph *graphObject, char *filename);
void printUsage();
void findMax5(int *arr, int size);

#endif
