#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_DIM 10

void init_arr(int arr[]) {
  for (int i = 0; i < MAX_DIM; i++) {
    arr[i] = (rand() % 10);
  }
}

void print_arr(int arr[], int size) {
  for (int i = 0; i < size; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void recursive(int arr[], int size) {
  if (size == 1) {
    printf("%d\n", arr[0]);
  } else {
    print_arr(arr, size);
    for (int i = 0; i < size - 2; i++) {
      arr[i] = arr[i] + arr[i+1];
    }
    recursive(arr, size-1);
  }
}

int main() {
  int* arr = calloc(10, sizeof(int));
  srand(time(NULL));
  init_arr(arr);
  recursive(arr, MAX_DIM);
}