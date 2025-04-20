# Thesis

Optimized BFS implementation in C using pthreads.

## Installation

Run make in the root directory to build the project:
  ```bash
  make
  ```

## Usage

To run the BFS implementation, use the following command:
```bash
./bin/bfs -f <input file> -n <number of runs>
```
More options are available by using the `-h`.

Note: the input file must be in the `.mtx` format. You can find many graphs in the [SuiteSparse Matrix Collection](https://sparse.tamu.edu/).