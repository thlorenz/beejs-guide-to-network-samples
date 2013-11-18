#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int showip(const char* host) {

  struct addrinfo hints, *res, *p;
  int status;
  char ipstr[INET6_ADDRSTRLEN];

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  if ((status = getaddrinfo(host, NULL  , &hints, &res) != 0)) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  }

  printf("\n\nIP addresses for %s\n", host);

  for (p = res; p != NULL; p = p->ai_next) {
    void *addr;
    char *ipver;
    int family = p->ai_family;

    // get pointer to address itself (different for IPv4 vs. IPv6)
    if (family == AF_INET) {
      ipver = "IPv4";
      struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
    } else {
      ipver = "IPv6";
      struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
    }

    inet_ntop(family, addr, ipstr, sizeof ipstr);
    printf("  %s: %s\n", ipver, ipstr);
  }

  freeaddrinfo(res);
  return 0;
}

int main(int argc, const char *argv[])
{
  showip("google.com");
  showip("mozilla.org");
  showip("apple.com");
  showip("microsoft.com");

  return 0;
}
