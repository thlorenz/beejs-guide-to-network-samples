#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <sys/wait.h>
#include <signal.h>

#define PORT "3000"
#define BACKLOG 10

static void sigchld_handler(int s) {
  // wait for process termination
  while(waitpid(-1, NULL, WNOHANG) > 0);
}

static void reap_dead_processes() {
  struct sigaction sa;
  sa.sa_handler = sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  int err = sigaction(SIGCHLD, &sa, NULL);
  if (err) {
    perror("sigaction");
    exit(1);
  }
}

static void *get_in_addr(struct sockaddr *sa) {
  return sa->sa_family == AF_INET
    ? (void *) &(((struct sockaddr_in*)sa)->sin_addr)
    : (void *) &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(void)
{

  int yes = 1;
  struct addrinfo hints;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family   =  AF_UNSPEC;
  hints.ai_socktype =  SOCK_STREAM;
  hints.ai_flags    =  AI_PASSIVE;

  int err;

  //
  // 1. getaddrinfo
  //
  struct addrinfo *servinfo;

  err = getaddrinfo(NULL, PORT, &hints, &servinfo);
  if (err) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    exit(1);
  }

  struct addrinfo *p;
  int sockfd;
  for (p = servinfo; p != NULL; p = p->ai_next) {

    //
    // 2. socket
    //
    sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
    if (sockfd == -1) {
      perror("server: socket");
      continue;
    }

    err = setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if (err) {
      perror("setsockopt");
      exit(1);
    }

    //
    // 3. bind
    //
    err = bind(sockfd, p->ai_addr, p->ai_addrlen);
    if (err) {
      close(sockfd);
      perror("server: bind");
      continue;
    }

    // if we were able to bind sockfd to one of the addresses resolved by getaddrinfo
    break;
  }
  freeaddrinfo(servinfo);

  if (p == NULL) {
    fprintf(stderr, "server: failed to bind to any of the resolved addresses\n");
    exit(2);
  }

  //
  // 4. listen
  //
  err = listen(sockfd, BACKLOG);
  if (err) {
    perror("listen");
    exit(1);
  }

  reap_dead_processes();

  printf("server: waiting for connections on port %s ...\n", PORT);

  //
  // 5. accept
  //
  struct sockaddr_storage client_addr;
  socklen_t sin_size;
  int client_sock_fd;
  char client_in_addr_s[INET6_ADDRSTRLEN];
  int is_child_process;
  while(1) {
    sin_size = sizeof client_addr;
    client_sock_fd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);

    if (client_sock_fd == -1) {
      perror("accept");
      continue;
    }
    void *client_in_addr = get_in_addr((struct sockaddr *)&client_addr);

    inet_ntop(client_addr.ss_family, client_in_addr, client_in_addr_s, sizeof client_in_addr_s);
    printf("server: got connection from %s\n", client_in_addr_s);

    is_child_process = !fork();
    if (is_child_process) {
      close(sockfd);
      err = send(client_sock_fd, "Hello, world!\n", 14, 0);
      if (err) {
        perror("send");
      }
      close(client_sock_fd);
      exit(0);
    }
    close(client_sock_fd);
  }

  return 0;
}