#include "interpreter.h"

#include <stdio.h>

int main(const int argc, char **const argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <rain script>", argv[0]);
    return 1;
  }

  struct interpreter_t *pi = interpreter_create();
  int rtn = interpreter_file(pi, argv[1]);
  interpreter_free(pi);

  return rtn;
}
