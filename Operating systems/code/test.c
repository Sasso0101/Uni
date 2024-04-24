#include "stdio.h"

int main() {
    int a[4];
    int b[4];
    printf("%p %p\n", &a[4], &b[0]);
    printf("%d", &a[4]==&b[0]);
    return 0;
}