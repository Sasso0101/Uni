#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *key;
  char* value;
} element_t;

typedef struct {
  int size;
  int occupied;
  element_t* table;
} hash_table_t;

void insert(hash_table_t* hash_table, int key, char* value) {
  // Rehash if table is full
  if (hash_table->occupied == hash_table->size) {
    hash_table->size *= 2;
    hash_table->occupied = 0;
    element_t* old_table = hash_table->table;
    hash_table->table = calloc(hash_table->size, sizeof(element_t));

    for (int i = 0; i < (hash_table->size)/2; i++) {
      if (old_table[i].key != NULL) {
        insert(hash_table, *old_table[i].key, old_table[i].value);
      }
    }
    free(old_table);
  }
  
  int* key_p = malloc(sizeof(int));
  *key_p = key;
  int index = *key_p % hash_table->size;
  while (hash_table->table[index].key != NULL) {
    index = (index + 1) % hash_table->size;
  }
  hash_table->table[index].key = key_p;
  hash_table->table[index].value = value;
  hash_table->occupied++;
}

char* search(hash_table_t* hash_table, int key) {
  int start_index = key % hash_table->size;
  int index = start_index;
  // Loop until we find the key, reach an empty slot or reach the start index
  while (*(hash_table->table[index].key) != key && hash_table->table[index].key != NULL && (index + 1) % hash_table->size != start_index) {
    index = (index + 1) % hash_table->size;
  }
  if (hash_table->table[index].key == NULL || (index == start_index && *(hash_table->table[index].key) != key)) {
    return NULL;
  }
  return hash_table->table[index].value;
}

void print_table(hash_table_t* hash_table) {
  printf("Size: %d, Occupied: %d\n", hash_table->size, hash_table->occupied);
  for (int i = 0; i < hash_table->size; i++) {
    if (hash_table->table[i].key != NULL) {
      printf("Key: %d, Value: %s\n", *(hash_table->table[i].key), hash_table->table[i].value);
    }
  }
}


int main() {
  // Initialize table
  element_t* table_ptr = calloc(1, sizeof(element_t));
  hash_table_t* hash_table = malloc(sizeof(hash_table_t));
  hash_table->occupied = 0;
  hash_table->size = 1;
  hash_table->table = table_ptr;

  int choice;
  int key;
  char* value;

  while (1) {
    printf("Enter your choice (1: insert, 2: search, 3: delete, 4: quit): ");
    scanf("%d", &choice);
    switch (choice) {
      case 1:
        // Insert operation
        printf("Enter key and value to insert: ");
        value = malloc(256);
        scanf("%d", &key);
        scanf("%s", value);
        insert(hash_table, key, value);
        break;
      case 2:
        // Search operation
        printf("Enter key to search: ");
        scanf("%d", &key);
        char* value = search(hash_table, key);
        if (value == NULL) {
          printf("Key not found\n");
        } else {
          printf("Value: %s\n", value);
        }
        break;
      case 3:
        // Delete operation
        printf("Enter key to delete: ");
        int key;
        scanf("%d", &key);
        // Call delete function (not implemented)
        printf("Not implemented\n");
        break;
      case 4:
        // Quit
        break;
      default:
        printf("Invalid choice\n");
    }
    print_table(hash_table);
  }
  
  return 0;
}