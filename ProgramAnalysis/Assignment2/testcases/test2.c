#include<stdlib.h>
void foo(int* p) {

}

int main() {
    int *i, *j, *k;
    i = (int*) malloc(sizeof(int)*5);    // escapes
    j = (int*) malloc(sizeof(int)*5);    // does not escape
    k = i;
    foo(k);
    return 0;
}
