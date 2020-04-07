#include "task.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static size_t g_task_id = 0;

void task_free(struct task_t *pt) {
  free(pt->cmd);
  free(pt);
}

struct task_t *task_create(const char *cmd, const size_t n_cmd) {
  struct task_t *pt = malloc(sizeof(struct task_t));
  assert(pt != NULL);

  pt->cmd = strndup(cmd, n_cmd);
  assert(pt->n_cmd != NULL);

  pt->n_cmd = n_cmd;

  pt->state = STATE_PENDING;

  pt->id = g_task_id;
  ++g_task_id;

  printf("Registered task \"%s\" with id %lu.\n", cmd, pt->id);
  return pt;
}

void task_execute(const struct task_t *pt, const struct connection_t *pconn) {
  size_t n_command = pt->n_cmd + pconn->n_opt + pconn->n_addr + 12;

  char *command = malloc(n_command);
  assert(command != NULL);

  snprintf(command, n_command, "ssh %s %s \" %s \"", pconn->opt, pconn->addr,
           pt->cmd);

  printf("Running task \"%s\" on connection \"%s\".\n", pt->cmd, pconn->addr);

  pt->state = STATE_RUNNING;
  system(command);
  pt->state = STATE_FINISHED;

  free(command);
}

int main() {
  char ssh_opt[] = "-i ~/rsa_key";
  char server[] = "tirmazi@pi.int.seas.harvard.edu";
  char task[] = "echo Hello >> chicken.chicken";

  task_execute(server, strlen(server), ssh_opt, strlen(ssh_opt), task,
               strlen(task));
}
