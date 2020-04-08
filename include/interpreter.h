#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "connection.h"
#include "task.h"

struct interpreter_t {
  struct connection_t *lst_conn;
  struct task_t *lst_task;

  size_t n_conn;
  size_t n_task;
};

// struct interpreter_t *interpreter_create(void) {}

#endif /* INTERPRETER_H */
