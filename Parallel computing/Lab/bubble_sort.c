#include <stdio.h>
#include <stdbool.h>

void print_arr(int* arr, int n) {
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void swap(int* arr, int i) {
  int temp = 0;
  temp = arr[i+1];
  arr[i+1] = arr[i];
  arr[i] = temp;
}

void sort(int* arr, int n) {
  bool sorted = false;
  while (!sorted) {
    sorted = true;
    for (int i = 0; i < n - 1; i++) {
      if (arr[i] > arr[i+1]) {
        swap(arr, i);
        sorted = false;
      }
    }
  }
}

void main() {
  int n = 8;
  int arr1[] = {1, 2, 5, 3, 2, 1, 7, 3};

  print_arr(arr1, n);
  sort(arr1, n);
  print_arr(arr1, n);
}