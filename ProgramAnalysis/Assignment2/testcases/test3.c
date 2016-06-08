#include<stdlib.h>
void foo(int** p) {

}

int main() {
    int **i, *j, *m;
    i = (int**) malloc(sizeof(int*)*5);  // escapes
    j = (int*) malloc(sizeof(int)*5);    // escapes
    m = (int*) malloc(sizeof(int)*5);    // does not escape
    *i = j;
    foo(i);
    return 0;
}
