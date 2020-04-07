#include "connection.h"

void connection_free(struct connection_t *pconn) {
  free(pconn->addr);
  free(pconn->opt);
  free(pconn);
}

struct connection_t *connection_create(const char *addr, const size_t n_addr,
                                       const char *opt, const size_t n_opt) {
  struct connection_t *pconn = malloc(sizeof(struct connection_t));
  assert(pconn != NULL);

  pconn->addr = strndup(addr, n_addr);
  assert(pconn->addr != NULL);

  pconn->n_addr = n_addr;

  pconn->opt = strndup(opt, n_opt);
  assert(pconn->opt != NULL);

  pconn->n_opt = n_opt;

  return pconn;
}
