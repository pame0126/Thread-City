#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include "mymutex.h"

struct mutex debug_futex;
static int debug_futex_init_done = 0;

char debug_msg[1000];

pid_t __my_thread_gettid()
{
	return (pid_t) syscall(SYS_gettid);
}

void __my_thread_debug_futex_init()
{
	if (debug_futex_init_done != 1) {
		my_mutex_init(&debug_futex, 1);
		debug_futex_init_done = 0;
	}
}
