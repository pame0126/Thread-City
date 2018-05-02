#include "mymutex.h"
#include <errno.h>
#include <sys/syscall.h>
#include <linux/futex.h>
#include <sys/time.h>

/* Syscall, futex */
int sys_mutex(int *uaddr, int op, int valor, const struct timespec *timeout)
{
  return syscall(SYS_futex, uaddr, op, valor, timeout, NULL, 0);
}

/* Valores a retornar:
 * -1 si falla
 * 0 si esta despierto
 * 1 si paso
 * 2 si no se durmio
 */
int __mutex_down_slow(struct mutex *futx, int valor, struct timespec *rel)
{
	if (sys_mutex(&futx->contador, FUTEX_WAIT, valor, rel) == 0) {
		//si alguien mas lo disminuye
		if (futx->contador <= MUTEX_PASSED) {
			futx->contador = -1;
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
	futx->contador = 1;
	my_mutex_lock();
	return sys_mutex(&futx->contador, FUTEX_WAKE, 1, NULL);
}

int mutex_await(struct mutex *futx, int signal)
{
	return sys_mutex(&futx->contador, FUTEX_FD, signal, NULL);
}

void my_mutex_init(struct mutex *futx, int valor)
{
	futx->contador = valor;
	my_mutex_lock();
}
