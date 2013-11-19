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

static struct sock make_socket(struct addrinfo *servinfo) {
  struct addrinfo *p;
  int sockfd;

  for(p = servinfo; p != NULL; p = p->ai_next) {
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("talker: socket");
      continue;
    }
    break;
  }

  return (struct sock) { sockfd, p };
}

int main(int argc, const char *argv[])
{
  const char *host;
  const char *msg;

  if (argc < 2) {
    fprintf(stderr, "usage: talker [hostname] message\n");
    exit(1);
  }

  if (argc == 2) {
    host = "localhost";
    msg = argv[1];
  } else {
    host = argv[1];
    msg = argv[2];
  }


  struct addrinfo hints     =  init_hints(SOCK_DGRAM, 0);
  struct addrinfo *servinfo =  resolve_dns(&hints, host, PORT);
  struct sock sock          =  make_socket(servinfo);


  if (sock.addr == NULL) {
    fprintf(stderr, "talker: failed to make socket\n");
    exit(1);
  }

  int nbytes = sendto(sock.fd, msg, strlen(msg), 0, sock.addr->ai_addr, sock.addr->ai_addrlen);
  if (nbytes == -1) {
    perror("talker: sendto");
    exit(1);
  }

  freeaddrinfo(servinfo);

  printf("talker: sent %d bytes to %s\n", nbytes, host);
  close(sock.fd);

  return 0;
}
