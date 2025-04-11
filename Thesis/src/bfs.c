#include "command_line.h"
#include "graph.h"

int main(int argc, char **argv) {
  Cli_Args args;
  init_cli();
  if (parse_args(argc, argv, &args) != 0) {
    return -1;
  }

  GraphCSR *graph = import_mtx(args.filename);
  if (graph == NULL) {
    fprintf(stderr, "Failed to import graph from file [%s]\n", args.filename);
    return -1;
  }

  print_graph(graph);

  free(graph);

  return 0;
}