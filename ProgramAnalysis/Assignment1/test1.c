#include <stdio.h>
void area(float param){
	float base;
	base = 23.34;
	float area= (base* param)/2;
	printf("area of the triangle is:%f\n",area);
}

void factorial(int param){
	int accumulator=1;
	while(param>1){
		accumulator*= param--;
	}
	printf("factorial of %d = %d\n",param, accumulator);
}
int main(){
	area(1.234);
	factorial(6);
}