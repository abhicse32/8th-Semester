#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define N 10000
int A[N][N];
int B[N][N];
int C[N][N];

void *calc_sum(void * p){
	int x=(int)p;
	int i,j;
	for(i=x-1000; i< x; i++)
		for(j=0;j<N;j++)
			C[i][j]=A[i][j]+B[i][j];
}

int main(){
	pthread_t threads[10];	
	int return_ids[10];
	int i,j;
	srand(time(NULL));
	for(i=0;i<N;i++){
		for(j=0;j<N;j++){
			 A[i][j]= rand()%100;
			 B[i][j]= rand()%100;
		}
	}
	j=1000;
	for(i=0;i<10;i++){
		return_ids[i]= pthread_create(&threads[i], NULL,calc_sum, (void*)j);
		j+=1000;
	}

	for(i=0;i<10;i++)
		pthread_join(threads[i],NULL);
	exit(EXIT_SUCCESS);
}