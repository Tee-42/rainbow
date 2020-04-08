#include "interpreter.h"

#include "parser.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct interpreter_t *interpreter_create(void) {
  struct interpreter_t *pi = malloc(sizeof(struct interpreter_t));
  assert(pi != NULL);

  pi->lst_conn = NULL;
  pi->lst_task = NULL;
  pi->lst_thread = NULL;

  pi->n_conn = 0;
  pi->n_task = 0;
  pi->n_thread = 0;

  return pi;
}

void interpreter_free(struct interpreter_t *pi) {
  if (pi->lst_conn != NULL) {
    free(pi->lst_conn);
  }

  if (pi->lst_task != NULL) {
    free(pi->lst_task);
  }

  if (pi->lst_thread != NULL) {
    free(pi->lst_thread);
  }

  free(pi);
}

struct _thread_args {
  struct task_t *ptask;
  struct connection_t *pconn;
};

static void *_thread_task_execute(void *a) {
  struct _thread_args *args = a;
  task_execute(args->ptask, args->pconn);
  free(args);
  return NULL;
}

static bool interpreter_next(struct interpreter_t *pi, struct command_t *pcmd) {
  if (pcmd->type == CMD_RUN) {
    size_t index_conn = pcmd->info.run.index_conn;
    size_t index_task = pcmd->info.run.index_task;

    if (index_conn < 0 || index_conn >= pi->n_conn) {
      fprintf(stderr, "Incorrect connection index: %lu\n", index_conn);
      return false;
    }

    if (index_task < 0 || index_task >= pi->n_task) {
      fprintf(stderr, "Incorrect task index: %lu\n", index_task);
      fprintf(stderr, "%lu blah", pi->n_task);
      return false;
    }

    pthread_t *resized = malloc(sizeof(pthread_t) * (pi->n_thread + 1));
    assert(resized != NULL);

    if (pi->lst_thread != NULL) {
      memcpy(resized, pi->lst_thread, sizeof(pthread_t) * pi->n_thread);
    }

    free(pi->lst_thread);
    pi->lst_thread = resized;

    pthread_t *last = &resized[pi->n_thread];

    struct _thread_args *args = malloc(sizeof(struct _thread_args));
    assert(args != NULL);

    args->pconn = &pi->lst_conn[index_conn];
    args->ptask = &pi->lst_task[index_task];

    if (pthread_create(last, NULL, _thread_task_execute, args) != 0) {
      fprintf(stderr, "Thread creation failed.\n");
      return false;
    }

    ++pi->n_thread;
    return true;
  }

  if (pcmd->type == CMD_CONNECT) {
    struct connection_t *resized =
        malloc(sizeof(struct connection_t) * (pi->n_conn + 1));
    assert(resized != NULL);

    if (pi->lst_conn != NULL) {
      memcpy(resized, pi->lst_conn, sizeof(struct connection_t) * pi->n_conn);
    }

    struct connection_t *last = &resized[pi->n_conn];
    last->addr = strndup(pcmd->info.connect.addr, PARSER_BUF);
    last->n_addr = strnlen(last->addr, PARSER_BUF);

    last->opt = strndup(pcmd->info.connect.opt, PARSER_BUF);
    last->n_opt = strnlen(last->opt, PARSER_BUF);

    free(pi->lst_conn);
    pi->lst_conn = resized;

    ++pi->n_conn;

    return true;
  }

  if (pcmd->type == CMD_TASK) {
    struct task_t *resized = malloc(sizeof(struct task_t) * (pi->n_task + 1));
    assert(resized != NULL);

    if (pi->lst_task != NULL) {
      memcpy(resized, pi->lst_task, sizeof(struct task_t) * pi->n_task);
    }

    struct task_t *temp =
        task_create(pcmd->info.task.cmd,
                    strnlen(pcmd->info.task.cmd, PARSER_BUF), pi->n_task);
    memcpy(&resized[pi->n_task], temp, sizeof(struct task_t));

    /* Note: calling task_free instead would be incorrect here as it also frees
     * the cmd string */
    free(temp);

    free(pi->lst_task);
    pi->lst_task = resized;

    ++pi->n_task;

    return true;
  }

  return false;
}

int interpreter_file(struct interpreter_t *pi, const char *filename) {
  struct command_t *cmd_arr;
  size_t n_cmd_arr;

  if (!parser_file(filename, &cmd_arr, &n_cmd_arr)) {
    fprintf(stderr, "Parsing failed.\n");
    return 1;
  }

  int rtn = 0;

  register size_t i;
  for (i = 0; i < n_cmd_arr; i++) {
    if (!interpreter_next(pi, &cmd_arr[i])) {
      rtn = 1;
      break;
    }
  }

  printf("Waiting for threads to finish.\n");

  for (i = 0; i < pi->n_thread; i++) {
    pthread_join(pi->lst_thread[i], NULL);
  }

  printf("Done.\n");
  return rtn;
}
