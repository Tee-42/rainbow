#ifndef TASK_H
#define TASK_H

#include "connection.h"

enum state_t { STATE_PENDING, STATE_RUNNING, STATE_FINISHED };

struct task_t {
  char *cmd;
  size_t n_cmd;

  size_t id;

  enum state_t state;
};

struct task_t *task_create(const char *cmd, const size_t n_cmd);

void task_execute(const struct task_t *pt, const struct connection_t *pconn);

#endif /* TASK_H */
