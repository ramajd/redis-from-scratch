/**
 * Simple server implementation
 * Time-stamp: <2023-02-13 16:42:09 by Reza Majd (reza-lenovo)>
 */

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "common.h"


static int one_request(int connfd) {
  char read_buffer[4 + k_max_msg + 1];
  errno = 0;
  int32_t err = read_all(connfd, read_buffer, 4);
  if (err) {
    if (errno == 0) {
      msg("EOF");
    } else {
      msg("read() error");
    }
    return err;
  }

  uint32_t len = 0;
  memcpy(&len, read_buffer, 4);
  if (len > k_max_msg) {
    msg("too long");
    return -1;
  }

  err = read_all(connfd, &read_buffer[4], len);
  if (err) {
    msg("read() error");
    return err;
  }

  // do something
  read_buffer[4 + len] = '\0';
  printf("client says: %s\n", &read_buffer[4]);

  // reply using same protocol
  const char reply[] = "world";
  char write_buffer[4 + sizeof(reply)];
  len = (uint32_t)strlen(reply);
  memcpy(write_buffer, &len, 4);
  memcpy(&write_buffer[4], reply, len);
  return write_all(connfd, write_buffer, 4 + len);
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

    while (true) {
      int32_t err = one_request(connfd);
      if (err) {
        break;
      }
    }
    close(connfd);
  }  
}
