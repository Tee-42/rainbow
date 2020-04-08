#include "interpreter.h"

#include "parser.h"

#include <stdio.h>
#include <stdlib.h>

int main(const int argc, char **const argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <rain script>", argv[0]);
    return 1;
  }

  return 0;
}
