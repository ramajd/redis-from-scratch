/**
 * Simple server implementtion
 * Time-stamp: <2023-02-13 14:41:46 by Reza Majd (reza-lenovo)>
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>

static void msg(const char *msg) {
  fprintf(stderr, "%s\n", msg);
}

static void die(const char *msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  abort();
}

static void do_something(int connfd) {
  char read_buffer[64];
  ssize_t n = read(connfd, read_buffer, sizeof(read_buffer) - 1);
  if (n < 0) {
    msg("read() error");
    return;
  }
  printf("client says: %s\n", read_buffer);

  char write_buffer[] = "world";
  write(connfd, write_buffer, strlen(write_buffer)); 
}

int main() {

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    die("socket()");
  }

  int val = 1;
  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));

  
  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(0); // wildcard address for 0.0.0.0
  int ret = bind(fd, (const struct sockaddr *) &addr, sizeof(addr));
  if (ret) {
    die("bind()");
  }

  ret = listen(fd, SOMAXCONN);
  if (ret) {
    die("listen()");
  }

  while (true) {
    struct sockaddr_in client_addr = {};
    socklen_t socklen = sizeof(client_addr);
    int connfd = accept(fd, (struct sockaddr *) &client_addr, &socklen);
    if (connfd < 0) {
      continue; // Error
    }

    do_something(connfd);
    close(connfd);
  }  
}
