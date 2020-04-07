#ifndef PARSER_H
#define PARSER_H

#define PARSER_BUF 1024

enum cmd_type_t { CMD_CONNECT, CMD_TASK, CMD_RUN, CMD_INVALID };

struct cmd_run_t {
  size_t index_conn;
  size_t index_task;
};

struct cmd_connect_t {
  char opt[PARSER_BUF];
  char addr[PARSER_BUF];
};

struct cmd_task_t {
  char cmd[PARSER_BUF];
};

union cmd_info_t {
  struct cmd_run_t run;
  struct cmd_connect_t connect;
  struct cmd_task_t task;
};

struct command_t {
  enum cmd_type_t type;
  union cmd_info_t info;
};

#endif /* PARSER_H */
