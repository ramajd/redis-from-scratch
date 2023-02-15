# Makefile for building the client and server applications
# Time-stamp: <2023-02-15 23:32:37 by Reza Majd (reza-lenovo)>

CC      = gcc
CFLAGS  = -g -Wall

SRCS    = $(wildcard *.c)
HDRS    = $(wildcard *.h)
OBJS    = $(SRCS:.c=.o)


server: common.o server.o $(HDRS)
	$(CC) $(CFLAGS) -o $@ $^

client: common.o client.o $(HDRS)
	$(CC) $(CFLAGS) -o $@ $^


clean:
	rm -rf $(OBJS) server client

all: client server
