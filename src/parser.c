#include "parser.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static void parser_connect(const char *cmd, const size_t len) {}

struct command_t parser_next(void) {

  char *cmd = NULL;
  size_t len = 0;
  ssize_t nread;

  const size_t k_len_task = 4;
  const size_t k_len_connect = 7;
  const size_t k_len_run = 3;

  nread = getline(&cmd, &len, stdin);

  if (nread == -1) {
    return false;
  }

  if (strncmp("connect", cmd, k_len_connect) == 0) {
    return parser_connect(cmd + k_len_connect, len - k_len_connect);
  }

  if (strncmp("task", cmd, k_len_task) == 0) {
    return parser_task(cmd + k_len_task, len - k_len_task);
  }

  if (strncmp("run", cmd, k_len_run) == 0) {
    return parser_run(cmd + k_len_run, ken - k_len_run);
  }

  struct command_t cmd;
  cmd->type = CMD_INVALID;
  return cmd;
}
