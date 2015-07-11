# bmake

RM = rm -rf
SRCS = main.c
CC = clang
CFLAGS = -Wall -std=c11

.export CC CFLAGS

all:
	mpicc ${SRCS}

clean:
	${RM} a.out *.a *.o

.PHONY: all clean
