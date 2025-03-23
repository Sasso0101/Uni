#include <string>

void swap(int* A, int i, int j) {
  int temp = A[i];
  A[i] = A[j];
  A[j] = temp;
}

void alternaSort(int* A, int n) {
  for (int i = 0; i < n - 1; i++) {
    if (i % 2 == 0) {
      if (A[i + 1] < A[i]) {
        swap(A, i, i + 1);
      }
    } else {
      if (A[i + 1] > A[i]) {
        swap(A, i, i + 1);
      }
    }
  }
}

int main(int argc, char** argv) {
  int* A = new int[argc - 1];
  for (int i = 0; i < argc - 1; i++) {
    A[i] = std::stoi(argv[i + 1]);
  }

  alternaSort(A, argc - 1);

  for (int i = 0; i < argc - 1; i++) {
    printf("%d ", A[i]);
  }
  printf("\n");
}