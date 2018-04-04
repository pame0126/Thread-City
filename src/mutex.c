#include "mutex.h"
#include <errno.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <sys/time.h>

/* Syscall, futex */
int sys_mutex(int *uaddr, int op, int val, const struct timespec *timeout)
{
  return syscall(SYS_futex, uaddr, op, val, timeout, NULL, 0);
}

/* Valores a retornar:
 * -1 si falla
 * 0 si esta despierto
 * 1 si paso
 * 2 si no se durmio
 */
int __mutex_down_slow(struct mutex *futx, int val, struct timespec *rel)
{
	if (sys_mutex(&futx->count, FUTEX_WAIT, val, rel) == 0) {
		//si alguien mas lo disminuye
		if (futx->count <= MUTEX_PASSED) {
			futx->count = -1;
			return 1;
		}
		return 0;
	}
	//EWOULDBLOCK: bandera que indica si el valor cambio antes de dormir
	if (errno == EWOULDBLOCK){
		return 2;
	}
	return -1;
}

int __mutex_up_slow(struct mutex *futx)
{
	futx->count = 1;
	my_mutex_lock();
	return sys_mutex(&futx->count, FUTEX_WAKE, 1, NULL);
}

int mutex_await(struct mutex *futx, int signal)
{
	return sys_mutex(&futx->count, FUTEX_FD, signal, NULL);
}

void my_mutex_init(struct mutex *futx, int val)
{
	futx->count = val;
	my_mutex_lock();
}
