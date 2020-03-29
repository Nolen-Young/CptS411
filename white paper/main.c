#include "list.h"
#include "pgRnk.h"

int main(int argc, char *argv[]){
	int K = 0;
	double D =  0.0;
	char *F;
	int i = 0, step = 0;
	Graph g;
	int *visited = NULL;
	int p = 1;

	unsigned seed = 1;

	if (argc != 5){
		printUsage();
		exit(1);
	}

	K = atoi(argv[1]);
	D = atof(argv[2]);
	F = strdup(argv[3]);
	p = atoi(argv[4]);
	
    // get time
	double t_input = omp_get_wtime();
	readGraph(&g, F);
	t_input = omp_get_wtime() - t_input;
    
	visited = (int*)calloc(g.numNodes, sizeof(int));

	omp_lock_t visitedLock;
	omp_init_lock(&visitedLock);
	double t = omp_get_wtime();
	int temp;
	int rank = -1;
	#pragma omp parallel for num_threads(p) private(step) schedule(guided, 5)
	for (i = 0; i < g.numNodes; i++) {
		if (omp_get_thread_num() > rank){
			temp = omp_get_thread_num();
			#pragma omp critical
			rank = temp;
		}

		List *pCur = &g.nodeArray[i]; //curr <- node i
		for (step = 1; step <= K; step++){
			#pragma omp atomic
			visited[pCur->id]++;
			
			//toss a coin
			if (tossWeightedCoin(D) == HEADS){
				pCur = &g.nodeArray[(rand_r(&seed)%g.numNodes)];
			}
			else {
				if (pCur->size){
					pCur = &g.nodeArray[getDestId(pCur, rand_r(&seed)%pCur->size)];
				}
				else{
					pCur = &g.nodeArray[(rand_r(&seed)%g.numNodes)];
				}
			}
		}
	}
	omp_destroy_lock(&visitedLock);
	findMax5(visited, g.numNodes);
	putchar('\n');
	t = omp_get_wtime() - t;
}


