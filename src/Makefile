SRCS =  mymutex.c mypthread_queue.c mypthreads.c mypthread_utilities.c mypthread_wrapper.c recorrido.c ruta_moviles.c  main.c
INC = mymutex.h  mutex_inline.h  mutex_assam.h  mypthreads.h  mypthread_queue.h recorrido.h ruta_moviles.h
OBJS = $(SRCS:.c=.o)
TEST_SRCS = main.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

CFLAGS = -Wall -Werror -I. -g
LDFLAGS = -L.
LIB = libmythread.a

AR = /usr/bin/ar
CC = gcc

.PHONY: all lib clean tags test a5
a5:  all
all: lib test ejecutor

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
	$(CC) -o threadCity $(CFLAGS) $(EXTRA_CFLAGS) $(TEST_OBJS) $(LIB)

ejecutor:
	gcc prueba.c -o city
