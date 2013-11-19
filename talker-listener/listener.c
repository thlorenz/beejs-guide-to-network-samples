#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../lib/init_hints.h"
#include "../lib/resolve_dns.h"
#include "../lib/types.h"
#include "../lib/bind_socket_to_address.h"
#include "../lib/get_in_addr.h"

#define PORT "3000"
#define MAXBUF 100

void receive(sock_t sock) {
  struct sockaddr_storage client_addr;
  socklen_t addr_len = sizeof client_addr;
  char *buf[MAXBUF];

  int nbytes = recvfrom(sock.fd, buf, MAXBUF -1, 0, (struct sockaddr *)&client_addr, &addr_len);
  if (nbytes == -1) {
    perror("recvfrom");
    exit(1);
  }

  void *client_in_addr = get_in_addr((struct sockaddr *)&client_addr);
  char client_in_addr_s[INET6_ADDRSTRLEN];
  const char *res = inet_ntop(client_addr.ss_family, client_in_addr, client_in_addr_s, sizeof client_in_addr_s);
  printf("listener: got packet from %s\n", res);

  printf("listener: packet is %d bytes long\n", nbytes);
  buf[nbytes] = '\0';
}

int main(void)
{
  struct addrinfo hints     =  init_hints(SOCK_DGRAM, AI_PASSIVE);
  struct addrinfo *servinfo =  resolve_dns(&hints, NULL, PORT);
  sock_t sock          =  bind_socket_to_address(servinfo);
  freeaddrinfo(servinfo);

  if (sock.addr == NULL) {
    fprintf(stderr, "listener: failed to bind to any of the resolved addresses\n");
    exit(2);
  }

  printf("listener: waiting to recvfrom...\n");

  for(;;) receive(sock);

  close(sock.fd);
  return 0;
}

