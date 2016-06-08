#include <stdio.h>
int sumArray(int arr[], int len){
	int i, sum=0;
	for(i=0; i<len ; sum+=arr[i],i++);

	return sum;
}

int main(int argc, char** argv){
	int arr[]={4,-1,5,6,-9};
	printf("sum: %d\n", sumArray(arr,5));
}