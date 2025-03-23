#include <stdio.h>

typedef struct subarray {
  int sum;
  int i;
  int j;
} subarray;


subarray max_sub_rec(int arr[], int n, int i, subarray best, subarray candidate) {
  if (i == n) {
    return best;
  } else {
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
    return max_sub_rec(arr, n, i+1, best, candidate);
  }
}

int main() {
  int arr[] = {-5, -4, -3};
  // int arr[] = {1, 2, 3, -8, 2, 3, -1, 3, 4, -3, 10, -3, 2};

  subarray best = {arr[0], 0, 0};
  best = max_sub_rec(arr, 3, 0, best, best);
  printf("Best sum: %d (from %d to %d)\n", best.sum, best.i, best.j);
}