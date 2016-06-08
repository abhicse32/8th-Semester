#include <omp.h>
#include <stdio.h>
	
#define N 10000
int A[N][N];
int B[N][N];
int C[N][N];

int main(){
	int i,j;
	srand(time(NULL));
	for(i = 0; i < N; i++)
		for(j = 0; j< N; j++){
			A[i][j]= rand()%100;
			B[i][j]= rand()%100;
		}

	#pragma omp parallel num_threads(10)
	{	
		int i,j;
		for(i = 0; i < N; i++)
			for(j = 0; j < N; j++)
				C[i][j] = A[i][j]+ B[i][j];
	}

	return 0;
}