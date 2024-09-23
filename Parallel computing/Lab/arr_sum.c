#include <stdio.h>

void print_arr(int* arr, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int sum(int* arr, int n) {
  int sum = 0;
  for (int i = 0; i < n; i++) {
    sum += arr[i];
  }
  return sum;
}

void main() {
  int n = 8;
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};

  print_arr(arr, n);
  printf("Sum: %d\n", sum(arr, n));
}