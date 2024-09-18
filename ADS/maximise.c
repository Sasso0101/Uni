#include <stdio.h>

typedef struct subarray {
  int sum;
  int i;
  int j;
} subarray;


subarray max_sub_empty(int arr[], int n) {
  subarray best = {0, -1, -1};
  subarray candidate = {0, 0, 0};
  
  for (int i = 0; i < n; i++) {
    if (candidate.sum <= 0) {
      candidate.sum = arr[i];
      candidate.i = i;
      candidate.j = i;
    } else {
      candidate.sum += arr[i];
      candidate.j = i;
    }
    if (candidate.sum > best.sum) {
      best = candidate;
    }
  }
  return best;
}

subarray max_sub(int arr[], int n) {
  subarray best = {arr[0], 0, 0};
  subarray candidate = {arr[0], 0, 0};
  
  for (int i = 1; i < n; i++) {
    if (candidate.sum <= arr[i]) {
      candidate.sum = arr[i];
      candidate.i = i;
      candidate.j = i;
    } else {
      candidate.sum += arr[i];
      candidate.j = i;
    }
    if (candidate.sum > best.sum) {
      best = candidate;
    }
  }
  return best;
}

int main() {
  int arr[] = {-5, -4};

  subarray best = max_sub_empty(arr, 2);
  printf("Best sum (empty): %d (from %d to %d)\n", best.sum, best.i, best.j);
  best = max_sub(arr, 2);
  printf("Best sum: %d (from %d to %d)\n", best.sum, best.i, best.j);

}