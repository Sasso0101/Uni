#ifndef CONFIG_H
#define CONFIG_H

#define MAX_THREADS 6

#define CHUNK_SIZE 128
#define CHUNKS_PER_THREAD 128

// Seed for picking source vertices using mt19937-64 RNG
// Using same seed as in GAP benchmark for reproducible experiments
// https://github.com/sbeamer/gapbs/blob/b5e3e19c2845f22fb338f4a4bc4b1ccee861d026/src/util.h#L22
#define SEED 27491095

#endif // CONFIG_H