#include<stdlib.h>
void foo(int*** p) {

}

int main() {
    int ****a, ***i, **j, *k;
    a = (int****) malloc(sizeof(int***)*5);    // does not escape
    i = (int***) malloc(sizeof(int**)*5);    // escapes
    j = (int**) malloc(sizeof(int*)*5);    // escapes
    k = (int*) malloc(sizeof(int)*5);    // escapes
    *a = i;
    *i = j;
    *j = k;
    foo(i);
    return 0;
}
