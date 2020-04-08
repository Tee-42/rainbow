#include "parser.h"

#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

static struct command_t parser_task(char *cmd, size_t len) {
  struct command_t parsed_cmd;
  parsed_cmd.type = CMD_INVALID;

  if (len == 0) {
    return parsed_cmd;
  }

  if (*cmd != ' ') {
    return parsed_cmd;
  }

  ++cmd;
  --len;

  /* remove trailing new line '\n', carriage return  etc. */
  while (isspace(cmd[len - 1]) || cmd[len - 1] == '\0') {
    --len;
  }

  if (len == 0) {
    return parsed_cmd;
  }

  parsed_cmd.type = CMD_TASK;

  memset(parsed_cmd.info.task.cmd, '\0', PARSER_BUF);

  strncpy(parsed_cmd.info.task.cmd, cmd, len);

  return parsed_cmd;
}

static struct command_t parser_run(char *cmd, size_t len) {

  struct command_t parsed_cmd;
  parsed_cmd.type = CMD_INVALID;

  if (len == 0) {
    return parsed_cmd;
  }

  if (*cmd != ' ') {
    return parsed_cmd;
  }

  ++cmd;
  --len;

  if (len == 0) {
    return parsed_cmd;
  }

  size_t counter = 0;
  while (cmd[counter] != ' ') {
    if (counter >= len) {
      return parsed_cmd;
    }

    ++counter;
  }

  /* TODO: atol is sloppy. Replace with strtol and proper error checking. */
  char *temp = strndup(cmd, counter);
  parsed_cmd.info.run.index_conn = atol(temp);
  free(temp);

  ++counter;
  cmd += counter;
  len -= counter;

  /* remove trailing new line '\n', carriage return  etc. */
  while (isspace(cmd[len - 1]) || cmd[len - 1] == '\0') {
    --len;
  }

  if (len == 0) {
    return parsed_cmd;
  }

  /* TODO: atol is sloppy. Replace with strtol and proper error checking. */
  temp = strndup(cmd, len);
  parsed_cmd.info.run.index_task = atol(temp);
  free(temp);

  parsed_cmd.type = CMD_RUN;
  return parsed_cmd;
}

static struct command_t parser_connect(char *cmd, size_t len) {
  struct command_t parsed_cmd;
  parsed_cmd.type = CMD_INVALID;

  if (len == 0) {
    return parsed_cmd;
  }

  if (*cmd != ' ') {
    return parsed_cmd;
  }

  ++cmd;
  --len;

  if (len == 0) {
    return parsed_cmd;
  }

  size_t counter = 0;

  while (cmd[counter] != ' ' && counter < len) {
    ++counter;
  }

  assert(counter < PARSER_BUF);

  memset(parsed_cmd.info.connect.addr, '\0', PARSER_BUF);
  memset(parsed_cmd.info.connect.opt, '\0', PARSER_BUF);

  strncpy(parsed_cmd.info.connect.addr, cmd, counter);

  parsed_cmd.type = CMD_CONNECT;

  cmd += counter;
  len -= counter;

  while (isspace(cmd[len - 1]) || cmd[len - 1] == '\0') {
    --len;
  }

  if (len == 0) {
    return parsed_cmd;
  }

  cmd++;
  len--;

  assert(len < PARSER_BUF);

  strncpy(parsed_cmd.info.connect.opt, cmd, len);

  return parsed_cmd;
}

static struct command_t parser_next(FILE *stream) {

  char *cmd = NULL;
  size_t len = 0;
  ssize_t nread;

  const size_t k_len_task = 4;
  const size_t k_len_connect = 7;
  const size_t k_len_run = 3;

  struct command_t parsed_cmd;
  parsed_cmd.type = CMD_INVALID;

  nread = getline(&cmd, &len, stream);

  if (nread == -1) {
    parsed_cmd.type = CMD_NONE;
    return parsed_cmd;
  }

  if (strncmp("connect", cmd, k_len_connect) == 0) {
    return parser_connect(cmd + k_len_connect, len - k_len_connect);
  }

  if (strncmp("task", cmd, k_len_task) == 0) {
    return parser_task(cmd + k_len_task, len - k_len_task);
  }

  if (strncmp("run", cmd, k_len_run) == 0) {
    return parser_run(cmd + k_len_run, len - k_len_run);
  }

  return parsed_cmd;
}

bool parser_file(const char *filename, struct command_t **pcmd_arr,
                 size_t *pn_cmd_arr) {
  FILE *pf;
  if ((pf = fopen(filename, "r")) == NULL) {
    fprintf(stderr, "Error opening rain script file.");
    return false;
  }

  struct command_t *cmd_arr = NULL;
  size_t n_cmd_arr = 0;

  struct command_t cmd;

  while (1) {
    cmd = parser_next(pf);

    if (cmd.type == CMD_NONE) {
      break;
    }

    if (cmd.type == CMD_INVALID) {
      fprintf(stderr, "Invalid command at line: %lu\n", n_cmd_arr + 1);
      free(cmd_arr);
      fclose(pf);
      return false;
    }

    struct command_t *resized =
        malloc(sizeof(struct command_t) * (n_cmd_arr + 1));
    assert(resized != NULL);

    if (cmd_arr != NULL) {
      memcpy(resized, cmd_arr, sizeof(struct command_t) * n_cmd_arr);
    }

    resized[n_cmd_arr] = cmd;

    free(cmd_arr);

    cmd_arr = resized;
    ++n_cmd_arr;
  }

  *pcmd_arr = cmd_arr;
  *pn_cmd_arr = n_cmd_arr;

  fclose(pf);
  return true;
}
