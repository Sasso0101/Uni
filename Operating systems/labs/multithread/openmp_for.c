#include <stdio.h>

int main() {
    #pragma omp parallel for
    for(int n=0; n<100; ++n) {
        printf(" %d", n);
    }
}