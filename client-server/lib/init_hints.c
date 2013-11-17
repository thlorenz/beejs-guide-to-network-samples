#include <string.h>
#include <netdb.h>

struct addrinfo init_hints(int sock_type) {
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family   =  AF_UNSPEC;
  hints.ai_socktype =  sock_type;
  hints.ai_flags    =  AI_PASSIVE;
  return hints;
}
