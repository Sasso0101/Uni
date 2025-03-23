#include <algorithm>
#include <string>

int circularMaxSum(int* A, int n) {
  int maxSoFar = 0;
  int maxHere = 0;
  int count = 0;
  int i = 0;

  while (i < 2*n && count < n) {
    if (maxHere + A[i % n] <= 0) {
      maxHere = 0;
      count = 0;
    } else {
      maxHere = maxHere + A[i % n];
      maxSoFar = std::max(maxSoFar, maxHere);
      count++;
    }
    i = i + 1;
  }
  return maxSoFar;
}

int main(int argc, char** argv) {
  int* A = new int[argc - 1];
  for (int i = 0; i < argc - 1; i++) {
    A[i] = std::stoi(argv[i + 1]);
  }

  printf("Max sum: %d\n", circularMaxSum(A, argc - 1));
}