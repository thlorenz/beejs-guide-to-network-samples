struct sock_s {
  int fd;
  struct addrinfo *addr;
};

typedef struct sock_s sock_t;
