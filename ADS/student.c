#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 20

typedef struct student {
  int score;
  int student_id;
} student;

void insert(student** arr, int pos, int score, int student_id) {
  arr[pos] = malloc(sizeof(student));
  arr[pos]->score = score;
  arr[pos]->student_id = student_id;
}

void print(student** arr, int pos) {
  arr[pos] = malloc(sizeof(student));
  printf("pos: %d, score: %d, student ID: %d", pos, arr[pos]->score, arr[pos]->student_id);
}

int main() {
  student** arr = malloc(sizeof(student*));
  insert(arr, 3, 5, 2);
  print(arr, 3);
}