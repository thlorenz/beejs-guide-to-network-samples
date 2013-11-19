#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#include "../lib/types.h"
#include "../lib/init_hints.h"
#include "../lib/resolve_dns.h"
#include "../lib/get_in_addr.h"

#define PORT "3000"
#define MAXDATA 100

static sock_t connect_socket_to_address(struct addrinfo *servinfo) {
  struct addrinfo *p;
  int sockfd;
  for (p = servinfo; p != NULL; p = p->ai_next) {

    // socket
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("client: socket");
      continue;
    }

    // connect
    int err = connect(sockfd, p->ai_addr, p->ai_addrlen);
    if (err) {
      close(sockfd);
      perror("client: connect");
      continue;
    }

    // if we were able to connect sockfd to one of the addresses resolved by getaddrinfo
    break;
  }
  return (sock_t) { sockfd, p };
}

int main(int argc, const char *argv[]) {

  struct addrinfo hints = init_hints(SOCK_STREAM, 0);

  const char *host = argc == 2 ? argv[1] : "localhost";

  struct addrinfo *servinfo = resolve_dns(&hints, host, PORT);

  sock_t sock = connect_socket_to_address(servinfo);

  if (sock.addr == NULL) {
    fprintf(stderr, "client: failed to connect\n");
    exit(2);
  }

  void *server_in_addr = get_in_addr((struct sockaddr *)&sock.addr);
  char server_in_addr_s[INET6_ADDRSTRLEN];
  inet_ntop(sock.addr->ai_family, server_in_addr, server_in_addr_s, sizeof server_in_addr_s);

  printf("client: connecting to %s\n", server_in_addr_s);

  freeaddrinfo(servinfo);

  char buf[MAXDATA];
  int nbytes = recv(sock.fd, buf, MAXDATA - 1, 0);
  if (nbytes < 0) {
    perror("recv");
    exit(1);
  }
  buf[nbytes] = '\0';

  printf("client: received '%s'\n", buf);

  close(sock.fd);

  return 0;
}
