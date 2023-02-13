/**
 * Header file containing the common variable and function declarations
 * Time-stamp: <2023-02-13 16:41:34 by Reza Majd (reza-lenovo)>
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>


extern const size_t k_max_msg;

void msg(const char *msg);
void die(const char *msg);

int32_t read_all(int fd, char *buffer, size_t n);
int32_t write_all(int fd, char *buffer, size_t n);


#endif // COMMON_H_
