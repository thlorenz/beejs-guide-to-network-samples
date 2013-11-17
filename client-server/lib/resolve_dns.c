#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct addrinfo *resolve_dns(struct addrinfo *hints, const char* host, const char *port) {
  struct addrinfo *servinfo;

  int err = getaddrinfo(host, port, hints, &servinfo);
  if (err) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    exit(1);
  }
  return servinfo;
}
