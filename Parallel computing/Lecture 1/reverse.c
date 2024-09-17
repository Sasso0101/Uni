#include <stdio.h>

void print_arr(int* arr, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void reverse(int* arr1, int n) {
  int temp = 0;
  for (int i = 0; i < n/2; i++) {
    temp = arr1[n-i-1];
    arr1[n-i-1] = arr1[i];
    arr1[i] = temp;
  }
}

void main() {
  int n = 8;
  int arr1[] = {1, 2, 3, 4, 5, 6, 7, 8};

  print_arr(arr1, n);
  reverse(arr1, n);
  print_arr(arr1, n);
}