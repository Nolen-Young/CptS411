/*	Cpt S 411, Introduction to Parallel Computing
 *	School of Electrical Engineering and Computer Science
 *
 *	Example code
 *	Send receive test:
 *   	rank 1 sends to rank 0 (all other ranks sit idle)
 *   	For timing use of C gettimeofday() is recommended.
 * */


#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	// init mpi variables and comm stuff
	int rank, p;
	struct timeval t1, t2;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);

	//printf("my rank=%d\n", rank);
	//printf("Rank=%d: number of processes =%d\n", rank, p);

	assert(p >= 2);

	// initialize variables we will be using
	int baseSize = 2;
	//char * message = (char*)malloc(baseSize); // the actual message being sent, starts at 1 byte then doubles over time
	//char message [4] = {'t','e','s','t'};
	int size = 0;
	float blockingAverages [21];
	float nonblockingAverages [21];
	
	
	
	//!!!!!!BLOCKING TEST!!!!!!
	for (int i = 0; i < 21; i++) { // loop for each size of message
		size = pow(baseSize, i);
		//message = (char*) realloc(message, size); //realloc the memory size
		char message [size/4];
		double recvResults [10]; // stores the results of each test for a given message size
		for (int j = 0; j < 10; j++) { // loop for ten tests
			if (rank == 1) {
				int dest = 0;
				//gettimeofday(&t1, NULL);
				MPI_Send(&message, size/4, MPI_CHAR, dest, 0, MPI_COMM_WORLD); // send message
				//gettimeofday(&t2, NULL);
				//int tSend = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
				//printf("Sent: Rank=%d, Size=%d, Send time=%d microsec\n", rank, size, tSend);

			}
			else if (rank == 0) {
				char y [size/4];
				MPI_Status status;

				gettimeofday(&t1, NULL);
				MPI_Recv(&y, size/4, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
				gettimeofday(&t2, NULL);

				int tRecv = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
				//printf("Recieved: Rank=%d, Size=%d, Recv time=%d microsec\n", rank, size, tRecv);
				recvResults[j] = (double) tRecv; // add time to results array
			}
		}
		// average the reult and store in array
		if (rank == 0) {
			double sum = 0.0; 
			for (int k = 0; k < 10; k++) {
				sum = sum + recvResults[k];
			}
			blockingAverages[i] = (double) (sum / 10.0);
		}
	}
	
	//print results for blocking stuff
	if (rank == 0) {
		printf("Blocking:\n");
		for (int l = 0; l < 21; l++) {
			printf("%d: %.2f\n", l, blockingAverages[l]); 
		}
	}
	

	//!!!!!!NON BLOCKING TEST!!!!!!
	for (int i = 0; i < 21; i++) { // loop for each size of message
		size = pow(baseSize, i);
		//message = (char*) realloc(message, size); //realloc the memory size
		char message[size/4];
		double recvResults [10]; // stores the results of each test for a given message size
		for (int j = 0; j < 10; j++) { // loop for ten tests
			if (rank == 1) {
				int dest = 0;
				//gettimeofday(&t1, NULL);
				MPI_Send(&message, size/4, MPI_CHAR, dest, 0, MPI_COMM_WORLD); // send message
				//gettimeofday(&t2, NULL);
				//int tSend = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
				//printf("Sent: Rank=%d, Size=%d, Send time=%d microsec\n", rank, size, tSend);

			}
			else if (rank == 0) {
				char y [size/4];
				int flag = 0;
				MPI_Request request;
				
				gettimeofday(&t1, NULL);
				MPI_Irecv(&y, size/4, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &request);
				MPI_Test(&request, &flag, MPI_STATUS_IGNORE);
				while(!flag) {
					MPI_Test(&request, &flag, MPI_STATUS_IGNORE);
				}
				gettimeofday(&t2, NULL);

				int tRecv = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
				//printf("Recieved: Rank=%d, Size=%d, Recv time=%d microsec\n", rank, size, tRecv);
				recvResults[j] = (double) tRecv; // add time to results array
			}
		}
		// average the reult and store in array
		if (rank == 0) {
			double sum = 0.0; 
			for (int k = 0; k < 10; k++) {
				sum = sum + recvResults[k];
			}
			nonblockingAverages[i] = (double) (sum / 10.0);
		}
	}
	// print out results for non blocking
	if (rank == 0) {
		printf("\n\nNon-Blocking:\n");
		for (int l = 0; l < 21; l++) {
			printf("%d: %.2f\n", l, nonblockingAverages[l]); 
		}
	}
	
	//free(message);
	MPI_Finalize();
}












