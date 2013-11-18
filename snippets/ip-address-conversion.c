#include <stdio.h>

#include <arpa/inet.h>
#include <netinet/in.h>

void ip4 () {
  struct sockaddr_in sa4;
  inet_pton(AF_INET, "192.0.2.1", &(sa4.sin_addr));

  char ip4[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &(sa4.sin_addr), ip4, INET_ADDRSTRLEN);

  printf("round-tripped ip4: %s\n", ip4);
}

void ip6 () {
  struct sockaddr_in6 sa6;
  inet_pton(AF_INET6, "2001:db8:63b3:1::3490", &(sa6.sin6_addr));

  char ip6[INET6_ADDRSTRLEN];
  inet_ntop(AF_INET6, &(sa6.sin6_addr), ip6, INET6_ADDRSTRLEN);

  printf("round-tripped ip6: %s\n", ip6);
}

int main(void) {
  ip4();
  ip6();
  return 0;
}
