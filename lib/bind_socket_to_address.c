#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include "types.h"

static void allow_port_reuse (int sockfd) {
  int yes = 1;

  int err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
  if (err) {
    perror("setsockopt");
    exit(1);
  }
}

struct sock bind_socket_to_address(struct addrinfo* servinfo) {
  struct addrinfo *p;
  int sockfd;

  for (p = servinfo; p != NULL; p = p->ai_next) {

    // socket
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("server: socket");
      continue;
    }

    allow_port_reuse(sockfd);

    // bind
    int err = bind(sockfd, p->ai_addr, p->ai_addrlen);
    if (err) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    // if we were able to bind sockfd to one of the addresses resolved by getaddrinfo
    break;
  }
  return (struct sock) { sockfd, p };
}
