/**
 * Implementation of common functions
 * Time-stamp: <2023-02-13 16:40:32 by Reza Majd (reza-lenovo)>
 */

#include "common.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <unistd.h>

const size_t k_max_msg = 4096;

void msg(const char *msg) { fprintf(stderr, "%s\n", msg); }

void die(const char *msg) {
  int err = errno;
  fprintf(stderr, "[%d] %s\n", err, msg);
  abort();
}


int32_t read_all(int fd, char *buffer, size_t n) {
  while (n > 0) {
    ssize_t ret = read(fd, buffer, n);
    if (ret <= 0) {
      return -1; // error on read
    }
    assert((size_t) ret <= n);
    n -= (size_t)ret;
    buffer += ret;
  }
  return 0;
}

int write_all(int fd, char *buffer, size_t n) {
  while (n > 0) {
    ssize_t ret = write(fd, buffer, n);
    if (ret <= 0) {
      return -1; // error
    }
    assert((size_t)ret <= n);
    n -= (size_t) ret;
    buffer += ret;    
  }
  return 0;
}
