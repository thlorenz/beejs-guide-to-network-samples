#include <string.h>
#include <netdb.h>

struct addrinfo init_hints(int sock_type, int flags) {
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family   =  AF_UNSPEC;
  hints.ai_socktype =  sock_type;
  if (flags) hints.ai_flags = flags;
  return hints;
}
