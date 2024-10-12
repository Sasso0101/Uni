#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_DIM 10

void init_arr(char arr[]) {
  for (int i = 0; i < MAX_DIM; i++) {
    switch (rand() % 3) {
      case 0:
        arr[i] = 'r';
        break;
      case 1:
        arr[i] = 'g';
        break;
      case 2:
        arr[i] = 'b';
        break;
    }
  }
}

void print_arr(char arr[]) {
  for (int i = 0; i < MAX_DIM; i++) {
    printf("%c ", arr[i]);
  }
  printf("\n");
}

int main() {
  char* arr = calloc(10, sizeof(char));
  srand(time(NULL));
  init_arr(arr);
  printf("Unsorted: ");
  print_arr(arr);

  int r, g, b;
  r = g = b = 0;

  for (int i = 0; i < MAX_DIM; i++) {
    switch (arr[i]) {
      case 'r':
        r++;
        break;
      case 'g':
        g++;
        break;
      case 'b':
        b++;    
        break;
    }
  }
  for (int i = 0; i < r; i++) {
    arr[i] = 'r';
  }
  for (int i = r; i < r+g; i++) {
    arr[i] = 'g';
  }
  for (int i = r+g; i < r+g+b; i++) {
    arr[i] = 'b';
  }
  printf("Sorted:   ");
  print_arr(arr);
}