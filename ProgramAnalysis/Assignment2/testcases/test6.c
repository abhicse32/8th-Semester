#include<stdlib.h>
void foo(int*** p) {

}

int main() {
    int ***a, **j, *k, *b;
    a = (int***) malloc(sizeof(int**)*5);    // escapes
    j = (int**) malloc(sizeof(int*)*5);    // escapes
    k = (int*) malloc(sizeof(int)*5);    // escapes
    b = (int*) malloc(sizeof(int)*5);    // does not escape
    *j = k;
    *a = j;
    j = (int**) malloc(sizeof(int*)*5);    // escapes
    foo(a);
    return 0;
}
