#include "pgRnk.h"

COINFACE tossWeightedCoin(double dampingVal){
	double coinflip = (double)rand() / (double)((unsigned)RAND_MAX + 1);
	return (coinflip <= dampingVal) ? HEADS : TAILS;
}

void printGraph(Graph *g){
	int i = 0;
	for (i = 0; i < g->numNodes; i++){
		printf("( %d : ", g->nodeArray[i].id + 1);
		printList(&(g->nodeArray[i]));
		printf(" )\n");
	}
}

void readGraph(Graph *graphObject, char *filename){
	FILE *infile = NULL;
	char buf[200];
	int u, v;
	int n = 0;

	infile = fopen(filename, "r");

	if (!infile){
		printf("ERROR: file open failed.\n");
		exit(1);
	}

	graphObject->nodeArray = (List *) malloc(sizeof(List));
	graphObject->numNodes = 1;
	graphObject->nodeArray[0].id = 0;
	graphObject->nodeArray[0].size = 0;
	graphObject->nodeArray[0].pHead = NULL;
    
	while (!feof(infile)){
		if (!fgets (buf, sizeof(buf), infile)) { break; }//if null just end
		if (buf[0] == '#') continue;
		sscanf(buf, "%d %d", &u, &v);
		if (u >= v){
			if (n-1 < u) n = u+1;
		}
		else{
			if (n-1 < v) n = v+1;
		}
		if (n > graphObject->numNodes){
			graphObject->nodeArray = (List *) realloc(graphObject->nodeArray, sizeof(List) * (n));
			memset(&graphObject->nodeArray[graphObject->numNodes], 0, sizeof(List)*((n)-graphObject->numNodes));
			graphObject->numNodes = n;
		}
		insertFront(&graphObject->nodeArray[u], v);
		graphObject->nodeArray[u].id = u;

	}
	fclose(infile);
	for (v = 1; v < n; v++) {
		if (graphObject->nodeArray[v].id == 0) {
			graphObject->nodeArray[v].id = v;
		}
	}
}

void printUsage(){
	printf("Usage - ");
	printf("pagerank {K: length of random walk} {D: damping ratio} {F: graph file} {P: #threads}\n");
}

void findMax5(int *arr, int size){
	int i;
	int top[5] = {-1,-1,-1,-1,-1};
	for (i = 0; i < size; i++){
		if (arr[top[0]] <= arr[i]){
			top[4] = top[3];
			top[3] = top[2];
			top[2] = top[1];
			top[1] = top[0];
			top[0] = i;
		}
	}
	printf("Top 5 ( [nodeId]: #visited ): \t[%d]: %d\t[%d]: %d\t[%d]: %d\t[%d]: %d\t[%d]: %d\n", 
			top[0], arr[top[0]],
			top[1], arr[top[1]],
			top[2], arr[top[2]],
			top[3], arr[top[3]],
			top[4], arr[top[4]]);
}
