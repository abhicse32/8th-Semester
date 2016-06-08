#include<stdlib.h>
void foo(int* p) {

}

int main() {
    int **i, *j, *k;
    i = (int**) malloc(sizeof(int*)*5);    // does not escape
    j = (int*) malloc(sizeof(int)*5);      // escapes
    *i = j;
    k = *i;
    foo(k);
    return 0;
}
