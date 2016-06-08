#include<stdlib.h>
void foo(int** p) {

}

int main() {
    int **a, *i, *k;
    a = (int**) malloc(sizeof(int*)*5);    // escapes
    k = (int*) malloc(sizeof(int)*5);    // escapes
    *a = k;
    foo(a);
    i = (int*) malloc(sizeof(int)*5);    // does not escape
    a = (int**) malloc(sizeof(int*)*5);    // escapes
    return 0;
}
