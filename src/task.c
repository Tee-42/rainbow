#include "task.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void task_free(struct task_t *pt) {
  free(pt->cmd);
  free(pt);
}

struct task_t *task_create(const char *cmd, const size_t n_cmd,
                           const size_t id) {
  struct task_t *pt = malloc(sizeof(struct task_t));
  assert(pt != NULL);

  pt->cmd = strndup(cmd, n_cmd);
  assert(pt->cmd != NULL);

  pt->n_cmd = n_cmd;

  // pt->state = STATE_PENDING;
  pt->id = id;

  printf("Registered task \"%s\" with id %lu.\n", cmd, pt->id);
  return pt;
}

/* Note: according to the POSIX standard: "All functions that reference (FILE *)
 * objects, except those with names ending in _unlocked, shall behave as if they
 * use flockfile() and funlockfile() internally to obtain ownership of these
 * (FILE *) objects."
 * (https://pubs.opengroup.org/onlinepubs/9699919799/functions/flockfile.html).
 * Therefore, I am not encapsulating printf calls with mutex locks even though
 * this function is designed to be run by multiple threads in parallel. It's
 * okay if multiple print statements are interleaved between threads in this
 * particular application as long as every single print statement is atomic. */
void task_execute(const struct task_t *pt, const struct connection_t *pconn) {
  size_t n_command = pt->n_cmd + pconn->n_opt + pconn->n_addr + 12;

  char *command = malloc(sizeof(char) * n_command);
  assert(command != NULL);

  snprintf(command, n_command, "ssh %s %s \" %s \"", pconn->opt, pconn->addr,
           pt->cmd);

  printf("Task %lu [\"%s\"] on connection \"%s\" is in state RUNNING.\n",
         pt->id, pt->cmd, pconn->addr);
  fflush(stdout);

  // pt->state = STATE_RUNNING;
  system(command);
  // pt->state = STATE_FINISHED;

  printf("Task %lu [\"%s\"] on connection \"%s\" is in state FINISHED.\n",
         pt->id, pt->cmd, pconn->addr);
  fflush(stdout);

  free(command);
}
