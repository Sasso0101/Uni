#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10

void init(int arr[]) {
  for (size_t i = 0; i < MAX_SIZE; i++)
  {
    arr[i] = rand() % 10;
  }
}

void print_arr(int arr[]) {
  for (size_t i = 0; i < MAX_SIZE; i++)
  {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

int min(int arr[]) {
  int min = arr[0];
  for (size_t i = 1; i < MAX_SIZE; i++)
  {
    if (arr[i] < min) {
      min = arr[i];
    }
  }
  return min;
}

int main() {
  srand(time(NULL));
  int arr[MAX_SIZE];
  init(arr);
  print_arr(arr);
  printf("%d", min(arr));
}