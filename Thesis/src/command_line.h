#ifndef COMMAND_LINE_H_
#define COMMAND_LINE_H_

#include <assert.h>
#include <ctype.h>
#include <getopt.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HELP_TEXT_BUFFER_LEN 2048
#define MAX_OPTIONS 20
#define MAX_PARAM_NAME_DISPLAY_LEN 15 // For formatting help: e.g. "<file>"
#define MAX_OPTION_DISPLAY_WIDTH 25 // Total width for "-f <file>" part in help
#define MAX_STRING_ARG_LEN 100

// Error codes
#define PARSE_SUCCESS 0
#define HELP_PRINTED 11
#define MISSING_PARAMETER 12
#define PARSE_ERROR 13

typedef enum {
  ARG_TYPE_STRING,
  ARG_TYPE_INT,
  ARG_TYPE_UINT32,
  ARG_TYPE_BOOL,
} ArgType;

typedef struct {
  char flag;
  const char *description;
  bool takes_parameter;
  const char *parameter_name; // e.g., "file", used to form <file>
  const char *default_value_str;
  ArgType type;
  void *value_ptr; // Pointer to field in Cli_Args or other struct
  bool required;
  bool found; // Internal: tracks if option was supplied
} Option;

// Parser state
static char help_string_buffer[HELP_TEXT_BUFFER_LEN];
static Option options_list[MAX_OPTIONS];
static int current_num_options = 0;
static char getopt_option_string[MAX_OPTIONS * 2 + 2]; // e.g., "f:n:vh" + '\0'

static inline int str_is_empty(const char *str) {
  return (str == NULL || str[0] == '\0');
}

static inline void safe_str_append(char *dest_buffer, const char *src_str,
                                   size_t dest_buffer_size) {
  assert(strlen(dest_buffer) + strlen(src_str) + 1 <= dest_buffer_size &&
         "String overflow in safe_str_append");
  strcat(dest_buffer, src_str);
}

static inline void cli_parser_init() {
  help_string_buffer[0] = '\0';
  getopt_option_string[0] = '\0';
  current_num_options = 0;
  strcpy(getopt_option_string, "h"); // Always include 'h' for standard help
}

static inline void add_option(char flag_char, const char *description_text,
                              bool has_parameter, const char *param_name_text,
                              const char *default_val_str, ArgType arg_val_type,
                              void *storage_ptr, bool is_required) {
  if (current_num_options >= MAX_OPTIONS) {
    fprintf(stderr, "Error: Maximum number of options (%d) exceeded.\n",
            MAX_OPTIONS);
    return;
  }
  if (flag_char == 'h' || (strchr(getopt_option_string, flag_char) != NULL)) {
    fprintf(stderr,
            "Error: Option flag '-%c' is reserved or already defined.\n",
            flag_char);
    return;
  }
  if (storage_ptr == NULL) {
    fprintf(stderr, "Error: storage_ptr cannot be NULL for option '-%c'.\n",
            flag_char);
    return;
  }

  Option *opt_entry = &options_list[current_num_options];
  opt_entry->flag = flag_char;
  opt_entry->description = description_text;
  opt_entry->takes_parameter = has_parameter;
  opt_entry->parameter_name =
      (has_parameter) ? (param_name_text ? param_name_text : "ARG") : "";
  opt_entry->default_value_str = default_val_str ? default_val_str : "";
  opt_entry->type = arg_val_type;
  opt_entry->value_ptr = storage_ptr;
  opt_entry->required = is_required;
  opt_entry->found = false;

  char option_signature_buf[MAX_PARAM_NAME_DISPLAY_LEN + 5]; // For "-c <PARAM>"
  if (has_parameter) {
    snprintf(option_signature_buf, sizeof(option_signature_buf), "-%c <%s>",
             flag_char, opt_entry->parameter_name);
  } else {
    snprintf(option_signature_buf, sizeof(option_signature_buf), "-%c",
             flag_char);
  }

  char help_line_buf[300]; // Buffer for a single help line
  snprintf(help_line_buf, sizeof(help_line_buf), "\n  %-*s  %s",
           MAX_OPTION_DISPLAY_WIDTH, option_signature_buf, description_text);
  safe_str_append(help_string_buffer, help_line_buf, HELP_TEXT_BUFFER_LEN);

  if (!str_is_empty(opt_entry->default_value_str)) {
    snprintf(help_line_buf, sizeof(help_line_buf), " (default: %s)",
             opt_entry->default_value_str);
    safe_str_append(help_string_buffer, help_line_buf, HELP_TEXT_BUFFER_LEN);
  }

  char getopt_char_str[3] = {flag_char, '\0', '\0'};
  if (has_parameter) {
    getopt_char_str[1] = ':';
  }
  safe_str_append(getopt_option_string, getopt_char_str,
                  sizeof(getopt_option_string));
  current_num_options++;
}

static inline int parse_supplied_option_value(const char *value_str,
                                              ArgType val_type, void *dest_ptr,
                                              char opt_flag) {
  char *endptr;
  if (value_str == NULL && val_type != ARG_TYPE_BOOL) {
    fprintf(stderr, "Error: Missing argument value for -%c.\n", opt_flag);
    return -1;
  }

  switch (val_type) {
  case ARG_TYPE_STRING:
    strncpy((char *)dest_ptr, value_str, MAX_STRING_ARG_LEN - 1);
    ((char *)dest_ptr)[MAX_STRING_ARG_LEN - 1] =
        '\0'; // Ensure null termination
    break;
  case ARG_TYPE_INT:
    if (str_is_empty(value_str)) { // strtol specific: empty string is 0,
                                   // endptr=value_str
      fprintf(stderr, "Error: Empty integer value for -%c.\n", opt_flag);
      return -1;
    }
    *(int *)dest_ptr = strtol(value_str, &endptr, 10);
    if (*endptr != '\0') {
      fprintf(stderr, "Error: Invalid integer value for -%c: '%s'\n", opt_flag,
              value_str);
      return -1;
    }
    break;
  case ARG_TYPE_UINT32:
    if (str_is_empty(value_str)) {
      fprintf(stderr, "Error: Empty unsigned integer value for -%c.\n",
              opt_flag);
      return -1;
    }
    *(uint32_t *)dest_ptr = strtoul(value_str, &endptr, 10);
    if (*endptr != '\0') {
      fprintf(stderr, "Error: Invalid unsigned integer value for -%c: '%s'\n",
              opt_flag, value_str);
      return -1;
    }
    break;
  case ARG_TYPE_BOOL:
    *(bool *)dest_ptr =
        true; // Presence of flag implies true. value_str not used.
    break;
  }
  return 0;
}

static inline int parse_args(int argc, char **argv) {
  // Initialize arguments with default values
  for (int i = 0; i < current_num_options; ++i) {
    Option *opt = &options_list[i];
    if (!str_is_empty(opt->default_value_str)) {
      if (parse_supplied_option_value(opt->default_value_str, opt->type,
                                      opt->value_ptr, opt->flag) != 0) {
        // Error already printed by parse_supplied_option_value
        return PARSE_ERROR;
      }
    } else {
      if (opt->type == ARG_TYPE_BOOL) {
        *(bool *)(opt->value_ptr) =
            false; // Booleans default to false if no explicit default
      }
    }
  }

  int c_opt;
  opterr = 0; // Suppress getopt's default error messages.
  optind = 1; // Reset getopt for potential re-parsing scenarios.

  while ((c_opt = getopt(argc, argv, getopt_option_string)) != -1) {
    if (c_opt == 'h') {
      printf("Usage: %s [options]\n", argv[0]);
      printf("Options:%s\n\n", help_string_buffer);
      return HELP_PRINTED;
    }
    if (c_opt ==
        '?') { // getopt signals an error (unknown option or missing argument)
      fprintf(stderr, "Error: ");
      if (optopt != 0) { // optopt contains the problematic option character
        bool known_opt_missing_arg = false;
        for (int i = 0; i < current_num_options; ++i) {
          if (options_list[i].flag == optopt &&
              options_list[i].takes_parameter) {
            fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            known_opt_missing_arg = true;
            break;
          }
        }
        if (!known_opt_missing_arg) { // If not a missing arg for known option,
                                      // it's unknown
          if (isprint(optopt))
            fprintf(stderr, "Unknown option `-%c'.\n", optopt);
          else
            fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
        }
      } else { // Error not tied to a specific option char, check argv[optind-1]
        fprintf(stderr, "Invalid argument or option usage near '%s'.\n",
                argv[optind - 1]);
      }
      fprintf(stderr, "Try '%s -h' for more information.\n", argv[0]);
      return MISSING_PARAMETER;
    }

    bool option_processed_flag = false;
    for (int i = 0; i < current_num_options; ++i) {
      if (options_list[i].flag == c_opt) {
        options_list[i].found = true;
        if (options_list[i].takes_parameter) {
          if (parse_supplied_option_value(optarg, options_list[i].type,
                                          options_list[i].value_ptr,
                                          c_opt) != 0) {
            return PARSE_ERROR; // Error details already printed
          }
        } else { // Flag without parameter (boolean type)
          if (options_list[i].type == ARG_TYPE_BOOL) {
            *(bool *)(options_list[i].value_ptr) = true;
          } else {
            fprintf(stderr,
                    "Internal Error: Non-boolean option -%c misconfigured as "
                    "not taking a parameter.\n",
                    c_opt);
            return PARSE_ERROR;
          }
        }
        option_processed_flag = true;
        break;
      }
    }
    if (!option_processed_flag && c_opt != 'h' && c_opt != '?') {
      // Should not be reached if getopt_option_string is correct and all
      // options registered.
      fprintf(stderr, "Internal Error: Unhandled option -%c.\n", c_opt);
      return PARSE_ERROR;
    }
  }

  // Check for required arguments that were not supplied
  for (int i = 0; i < current_num_options; ++i) {
    if (options_list[i].required && !options_list[i].found) {
      fprintf(stderr, "Error: Required option -%c (%s) is missing.\n",
              options_list[i].flag, options_list[i].description);
      fprintf(stderr, "Try '%s -h' for more information.\n", argv[0]);
      return MISSING_PARAMETER;
    }
  }

  // Non-option arguments (positional arguments: argv[optind] to argv[argc-1])
  // are currently ignored. To handle them: iterate from argv[optind] up to
  // argc.

  return PARSE_SUCCESS;
}

#endif // COMMAND_LINE_H_