/*	Cpt S 411, Introduction to Parallel Computing
 *	School of Electrical Engineering and Computer Science
 *
 *	Nolen Young, 11517296
 *  Project 3
 * */

#include <stdio.h>
#include <mpi.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

int * SerialMatrix(int n, int seed, int A, int B, int P);
void PMultiplicationSquare(int arr1[][2], int arr2[][2], int res[][2], int P);
void PMultiplicationRect(int mat1[], int mat2[][2], int P, int result[]);

int main(int argc, char *argv[])
{
    // init rank, p, and overall comm stuff
    int rank, p;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
    MPI_Status status;
    
    // init constants, arbitrary primes of certain sizes
    int A = 113;
    int B = 149;
    int P = 10631;
    
    // init other variables
    int n = 20; // number of items to be generated
    int seed = 42; // The answer to life, the universe, and everything
    
    // init matrices
    int m [2][2] = {
            {A, 0},
            {B, 1}
            };
            
    int m0 [2][2] = {
            {1, 0},
            {0, 1}
            };
    
    // find x local
    // assume n is divisible by p, otherwise this won't work
    int * xLocal [n/p];
    int i;
    for (i = 0; i < (n/p); i++) {
        //xLocal[i] = m; // init each value to m
        memcpy(xLocal[i], m, sizeof(m));
    }
    
    // find mLocal
    int mLocal[2][2] = {{},{}};
    memcpy(mLocal, m0, sizeof(m0));
    for (i = 0; i < (n/p); i++) {
        PMultiplicationSquare(mLocal, xLocal[i], mLocal, P);
    }
    
    printf("I MADE IT HERE");
    
    // Begin p parallel prefix
    // init values
    int l[2][2] = {};
    int g[2][2] = {};
    memcpy(l, mLocal, sizeof(mLocal));
    memcpy(g, l, sizeof(l));
    int partner = 0;
    
    int t, x, y, sendTemp, recvTemp = 0;
    int gRemote[2][2] = {};
    for (t = 0; t < log2(n/p); t++){
        partner = rank ^ (1 << t);
        
        // do send and recieve with partner, need to do four seperate sends for each integer, might optimize later
        for(x = 0; x < 2; x++) {
            for (y = 0; y < 2; y++) {
                sendTemp = g[x][y];
                MPI_Sendrecv(&sendTemp, 1, MPI_INT, partner, MPI_ANY_TAG, &recvTemp, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                gRemote[x][y] = recvTemp;
            }
        }
        
        if (partner < rank) {
            // add l and gRemote, store in l
            for(x = 0; x < 2; x++) {
                for (y = 0; y < 2; y++) {
                    l[x][y] = l[x][y] + gRemote[x][y];
                } 
            }
        }
        // set g to g plus remote
        for(x = 0; x < 2; x++) {
            for (y = 0; y < 2; y++) {
                g[x][y] = g[x][y] + gRemote[x][y];
            }
        }
    }
    int mOff[2][2] = {};
    memcpy(mOff, l, sizeof(l));
    
    printf("I MADE IT FURTHER");
    
    
    // serial matrix
    int mNext [2][2] = {};
    memcpy(mNext, mOff, sizeof(mOff));
    
    int x0[2] = {seed, 1};
    int tempResult[2] = {seed, 1};
    for (i = 0; i < (n/p); i++) {
        PMultiplicationRect(x0, mNext, P, tempResult);
        PMultiplicationSquare(mNext, m, mNext, P);
        printf("%d", tempResult[0]); // print results
    }
    MPI_Finalize();
}

void PMultiplicationSquare(int arr1[][2], int arr2[][2], int res[][2], int P) {
    int i, j, k; 
    for (i = 0; i < 2; i++) 
    { 
        for (j = 0; j < 2; j++) 
        { 
            res[i][j] = 0; 
            for (k = 0; k < 2; k++) 
                res[i][j] += (arr1[i][k] *  arr2[k][j]) % P; 
        } 
    }
}

void PMultiplicationRect(int mat1[], int mat2[][2], int P, int result[]) 
{ 
    result[0] = ((mat1[0] * mat2[0][0]) + (mat1[0] * mat2[0][1])) % P;
    result[1] = 1;
} 

/*
int * SerialMatrix(int n, int seed, int A, int B, int P)
{
    int result[n];
    result[0] = seed;
    int m [2][2] = {
            {A, 0},
            {B, 1}
            };
    int mNext [2][2] = {};
    memcpy(mNext, m, sizeof(m));
    
    int i;
    int tempResult[2];
    for (i = 1; i < n; i++) {
        // implement this sections once I understand the array context
    }
}
*/
