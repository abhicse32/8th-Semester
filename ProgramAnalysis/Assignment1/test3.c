#include <stdio.h>
#include <string.h>
int main(){
	int a=12;
	int* p= &a;
	int b= *p+12;
	*p++;
	p= &b;
	*p= *p+23;
	b = *p + a;
	printf("a:%d, b:%d\n",a,b);
}