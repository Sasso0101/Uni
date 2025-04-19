#ifndef CONFIG_H
#define CONFIG_H

#define MAX_THREADS 24
#define TRUE_THREADS_TEST 24

#define CHUNK_SIZE 64
#define CHUNKS_PER_THREAD 128

// Seed used for picking source vertices
// Using same seed as in GAP benchmark for reproducible experiments
// https://github.com/sbeamer/gapbs/blob/b5e3e19c2845f22fb338f4a4bc4b1ccee861d026/src/util.h#L22
#define SEED 27491095

#endif // CONFIG_H