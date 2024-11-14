### Optimizing for the architecture
Modern Intel processors support vector operations with 256 bits (AVX2) and 512 bits (AVX-512). AVX-512 doesn't make sense to use because the CPU lowers the clock, thus resulting in the same execution time. To enable this instruction set on supported CPUs, use the flag `-march=native (-mtune=native)`. To force vectorization with 512 bits use the flag `-mprefer-vector-width=512`. This is disabled by default because when using AVX-512 instructions the clock frequency is reduced, leading to the same performance [^1].

[^1]: https://en.wikipedia.org/wiki/AVX-512#Reception
### Checking for auto-vectorization
`gcc` can automatically (some) vectorize loops.

Vectorizable loops: https://gcc.gnu.org/projects/tree-ssa/vectorization.html

Guide to vectorization: https://colfaxresearch.com/knl-avx512/

Flag to print the auto-vectorization report: `-fopt-info-vec-all`.
To obtain more info about the gcc optimizations see https://gcc.gnu.org/onlinedocs/gcc/Developer-Options.html#index-fopt-info.

In the report gcc will sometimes complain that it cannot vectorize loops because of data dependencies (i.e. the result of one operation may affect the input of another one in the same vector operation). If you are sure that there are no data dependencies, you can force gcc to ignore this check by adding `#pragma GCC ivdep` right before the affected loop.

More loop pragmas can be found at https://gcc.gnu.org/onlinedocs/gcc/Loop-Specific-Pragmas.html.

### Useful commands for profiling
likwid docs: https://github.com/RRZE-HPC/likwid/wiki/likwid-perfctr

likwid tutorial: https://blogs.fau.de/hager/files/2021/10/sc21-06_tools_part2.pdf

To profile a specific function inside C code, use the following markers:
```c
#include <stdio.h>
#include <stdlib.h>
#include <likwid.h>
// ...
void function_to_profile() {
    LIKWID_MARKER_INIT;
    LIKWID_MARKER_START("Section to profile");
    // Code to profile
    LIKWID_MARKER_STOP("Section to profile");
    LIKWID_MARKER_CLOSE;
}

int main() {
  // ...
}

```
The compiler doesn't know how to interpret the flags, therefore add the following alias to .bashrc.
```bash
alias gcc-likwid='gcc -O3 -DLIKWID_PERFMON -I /apps/likwid-4.3.4/include -L /apps/lik
llikwid'
```
Now you can use the command `gcc-likwid` in the same way as `gcc`, but with added support for likwid profiling.

There are many metrics that can be profiled, which are categorized into different groups. The available groups can be printed using `likwid-perfctr -a`, while a detailed description for each group can be printed using `likwid-perfctr -H -g <GROUP>`. To profile the compiled program, run
```bash
likwid-perfctr -C 0 -g <GROUP> -m ./a.out
```

To print the FLOP/s use the group `FLOPS_SP` (for single precision). This will print also the number of AVX operations (used for 256 bit vectorization) and AVX512 operations (used for 512 bit vectorization).