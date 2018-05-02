SRCS =  mymutex.c mypthread_queue.c mypthreads.c mypthread_utilities.c mypthread_wrapper.c   threadserver.c
INC = mymutex.h  mutex_inline.h  mutex_assam.h  mypthreads.h  mypthread_queue.h hamburguesa.h threadserver.h
OBJS = $(SRCS:.c=.o)
TEST_SRCS = threadserver.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

CFLAGS = -Wall -Werror -I. -g
LDFLAGS = -L.
LIB = libmythread.a

AR = /usr/bin/ar
CC = gcc

.PHONY: all lib clean tags test a5
a5:  all
all: lib test cliente

lib: $(LIB)

libmythread.a: $(OBJS) $(INC)
	$(AR) rcs $(LIB) $(OBJS)

%.o: %.c $(INC)
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TEST_OBJS) $(LIB) *~threadCity threadCity city

tags:
	find . -name "*.[cChH]" | xargs ctags
	find . -name "*.[cChH]" | etags -

test:	$(TEST_OBJS) $(INC) lib
	$(CC) -o Extra $(CFLAGS) $(EXTRA_CFLAGS) $(TEST_OBJS) $(LIB)

cliente: cliente.o
	gcc cliente.c -o client
