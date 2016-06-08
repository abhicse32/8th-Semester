#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
int main(int argc, char* argv[]){
	int X=2;
	//omp_set_num_threads(1);
	#pragma omp parallel
	{
		printf("Thread: %d, the value of the variable x is %d\n",omp_get_thread_num(), X);
		X=0;
	}
	return 0;
}