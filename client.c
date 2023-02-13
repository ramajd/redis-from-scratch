/**
 * Simple client implementation
 * Time-stamp: <2023-02-13 16:39:53 by Reza Majd (reza-lenovo)>
 */

#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "common.h"


static size_t query(int fd, const char *text) {
  // write text
  uint32_t len = strlen(text);
  if (len > k_max_msg) {
    return -1;
  }

  char write_buffer[4 + k_max_msg];
  memcpy(write_buffer, &len, 4);
  memcpy(&write_buffer[4], text, len);
  int32_t err = write_all(fd, write_buffer, 4 + len);
  if (err) {
    return err;
  }
  
  // read response header
  char read_buffer[4 + k_max_msg + 1];
  errno = 0;
  err = read_all(fd, read_buffer, 4);
  if (err) {
    if (err == 0) {
      msg("EOF");
    } else {
      msg("read() error");
    }
    return err;
  }

  memcpy(&len, read_buffer, 4); // assume little endian
  if (len > k_max_msg) {
    msg("response too long");
    return -1;
  }

  // read response body
  err = read_all(fd, &read_buffer[4], len);
  if (err) {
    msg("read() error");
    return err;
  }

  // do something
  read_buffer[4 + len] = '\0';
  printf("server says: %s\n", &read_buffer[4]);
  return 0;
}

int main() {

  int fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd < 0) {
    die("socket()");
  }

  struct sockaddr_in addr = {};
  addr.sin_family = AF_INET;
  addr.sin_port = ntohs(1234);
  addr.sin_addr.s_addr = ntohl(INADDR_LOOPBACK); // 127.0.0.1
  int ret = connect(fd, (const struct sockaddr *)&addr, sizeof(addr));
  if (ret) {
    die("connect()");
  }

  if (query(fd, "Hello1") ||
      query(fd, "Hello2") ||
      query(fd, "Hello3")) {
    // no-op
  }

  close(fd);
  return 0;
}
