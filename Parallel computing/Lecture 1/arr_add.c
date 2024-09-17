#include <stdio.h>

void print_arr(int* arr, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void arr_add(int* arr1, int* arr2, int* sum, int n) {
  for (int i = 0; i < n; i++) {
    sum[i] = arr1[i] + arr2[i];
  }
}

void main() {
  int n = 8;
  int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8};
  int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8};
  int sum[n];

  print_arr(arr1, n);
  print_arr(arr2, n);
  arr_add(arr1, arr2, sum, n);
  print_arr(sum, n);
}