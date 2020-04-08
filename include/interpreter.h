#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "connection.h"
#include "task.h"

#include <pthread.h>

struct interpreter_t {
  struct connection_t *lst_conn;
  struct task_t *lst_task;

  pthread_t *lst_thread;

  size_t n_conn;
  size_t n_task;
  size_t n_thread;
};

struct interpreter_t *interpreter_create(void);

int interpreter_file(struct interpreter_t *pi, const char *filename);

void interpreter_free(struct interpreter_t *pi);

#endif /* INTERPRETER_H */
