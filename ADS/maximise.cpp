#include <stdio.h>

struct subarray {
  int sum;
  int i;
  int j;
};


subarray max_sub(int arr[], int n) {
  subarray best = subarray {
    sum: 0,
    i : -1,
    j: -1
  };
  subarray candidate = subarray {
    sum: 0,
    i : 0,
    j: 0
  };
  
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

int main() {
  int arr[] = {1, 3, 4, -8, 2, 3, -1, 3, 4, -3, 10, -3, 2};

  subarray best = max_sub(arr, 13);
  printf("Best sum: %d (from %d to %d)\n", best.sum, best.i, best.j);
}