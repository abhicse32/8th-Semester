#include<stdlib.h>

void foo(int* p) {

}

int main() {
    int *i, *j;
    i = (int*) malloc(sizeof(int)*5);    // escapes
    j = (int*) malloc(sizeof(int)*5);    // does not escape
    foo(i);
    return 0;
}
