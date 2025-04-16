#ifndef COMMAND_LINE_H_
#define COMMAND_LINE_H_

#include <getopt.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>

#define HELP_LEN 1000
char help_string[HELP_LEN];

#define HELP_PRINTED	11
#define MISSING_PARAMETER	12

typedef struct {
  char filename[100];
  int runs;
  uint32_t source;
} Cli_Args;

int str_empty(const char* str) {
  if (str == NULL || str[0] == '\0') return 1;
  else return 0;
}

void add_help_line(char opt, const char* opt_arg, const char* text, const char* def) {
  const int buf_len = 100;
  char buf[buf_len];

  if (!str_empty(opt_arg))
    snprintf(buf, buf_len, "\n -%c <%s>: %-54s", opt, opt_arg, text);
  else
    snprintf(buf, buf_len, "\n -%c %s: %-54s", opt, "", text);

  if (!str_empty(def))
    snprintf(buf, buf_len, "%s [default: %s]", buf, def);
  strcat(help_string, buf);
}

int parse_args(int argc, char** argv, Cli_Args* args) {
  int c_opt;
  int filename_flag = 0, runs_flag = 0, source_flag = 0;
  while ((c_opt = getopt(argc, argv, "f:r:s:h")) != -1) {
    switch (c_opt) {
      case 'f':
        strncpy(args->filename, optarg, sizeof(args->filename));
        filename_flag = 1;
        break;
      case 'r':
        args->runs = atoi(optarg);
        runs_flag = 1;
        break;
      case 's':
        args->source = atoi(optarg);
        source_flag = 1;
        break;
      case '?':
        if (optopt == 'f' || optopt == 'r' || optopt == 's') {
          fprintf(stderr, "Option -%c requires an argument.\n", optopt);
          return MISSING_PARAMETER;
        } else if (isprint(optopt)) {
          fprintf(stderr, "Unknown option `-%c'.\n", optopt);
        } else {
          fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
        }
        break;
      case 'h':
        printf("Help: \n%s\n", help_string);
        return HELP_PRINTED;
        break;
    }
  }
  if (filename_flag == 0) {
    fprintf(stderr, "Filename not specified. Specify it with -f.\n");
    return MISSING_PARAMETER;
  }
  if (runs_flag == 0) {
    args->runs = 1;
  }
  if (source_flag == 0) {
    args->source = UINT32_MAX;
  }
  return 0;
}

void init_cli() {
  add_help_line('f', "file", "load graph from file", NULL);
  add_help_line('r', "runs", "number of runs", "1");
  add_help_line('s', "source", "ID of source vertex", "rand");
  add_help_line('h', "", "print this help message", NULL);
}

#endif