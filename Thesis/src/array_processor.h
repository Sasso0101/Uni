#ifndef ARRAY_PROCESSOR_H_
#define ARRAY_PROCESSOR_H_

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

// Structure to manage the array processing state
typedef struct {
    size_t total_elements;        // Total number of elements in the array
    size_t chunk_size;            // Number of elements each thread processes at a time
    volatile size_t next_chunk_start_index; // Index of the start of the next available chunk
    pthread_mutex_t mutex;        // Mutex to protect access to next_chunk_start_index
} ArrayProcessor;

/**
 * @brief Initializes the ArrayProcessor.
 *
 * @param processor Pointer to the ArrayProcessor structure to initialize.
 * @param data Pointer to the array to be processed.
 * @param num_elements Total number of elements in the array.
 * @param elements_per_chunk The number of elements a thread should try to process at once.
 *                           If 0, it's an error.
 * @return 0 on success, -1 on error.
 */
static inline int array_processor_init(ArrayProcessor *processor,
                                       size_t num_elements,
                                       size_t elements_per_chunk) {
    if (!processor) {
        fprintf(stderr, "Error: Null pointer provided to array_processor_init.\n");
        return -1;
    }
    if (elements_per_chunk == 0) {
        fprintf(stderr, "Error: elements_per_chunk cannot be zero.\n");
        return -1;
    }

    processor->total_elements = num_elements;
    processor->chunk_size = elements_per_chunk;
    processor->next_chunk_start_index = 0;

    if (pthread_mutex_init(&processor->mutex, NULL) != 0) {
        fprintf(stderr, "Error: Mutex initialization failed.\n");
        return -1;
    }
    return 0;
}

/**
 * @brief Gets the next chunk of the array for a thread to process.
 *        This function is thread-safe.
 *
 * @param processor Pointer to the initialized ArrayProcessor.
 * @param[out] chunk_start_index Pointer to store the starting index of the assigned chunk.
 * @param[out] elements_in_chunk Pointer to store the number of elements in the assigned chunk.
 * @return true if a chunk was successfully assigned, false if no more chunks are available.
 */
static inline bool array_processor_get_chunk(ArrayProcessor *processor,
                                             size_t *chunk_start_index,
                                             size_t *elements_in_chunk) {
    pthread_mutex_lock(&processor->mutex);

    if (processor->next_chunk_start_index >= processor->total_elements) {
        // No more elements to process
        pthread_mutex_unlock(&processor->mutex);
        *elements_in_chunk = 0;
        return false;
    }

    *chunk_start_index = processor->next_chunk_start_index;
    size_t remaining_elements = processor->total_elements - *chunk_start_index;

    if (remaining_elements < processor->chunk_size) {
        *elements_in_chunk = remaining_elements;
    } else {
        *elements_in_chunk = processor->chunk_size;
    }

    processor->next_chunk_start_index += *elements_in_chunk;

    pthread_mutex_unlock(&processor->mutex);
    return true;
}

#endif // ARRAY_PROCESSOR_H_