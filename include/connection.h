#ifndef CONNECTION_H
#define CONNECTION_H

struct connection_t {
  char *addr;
  size_t n_addr;

  char *opt;
  size_t n_opt;
};

#endif /* CONNECTION_H */
