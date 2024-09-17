#include <stdio.h>

void print_arr(int* arr, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int maximum(int* arr, int n) {
  int maximum = arr[0];
  for (int i = 1; i < n; i++) {
    if (arr[i] > maximum) {
      maximum = arr[i];
    }
  }
  return maximum;
}

void main() {
  int n = 8;
  int arr[] = {-1, 5, 10, -2, 5, 6, 7, 8};

  print_arr(arr, n);
  printf("Maximum: %d\n", maximum(arr, n));
}