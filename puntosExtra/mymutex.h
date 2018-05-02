#ifndef	_MUTEX_H
#define	_MUTEX_H
#include <unistd.h>
#include <linux/futex.h>
#include <stdio.h>

struct mutex
{
	int contador;
};

#include "mutex_inline.h"
#define MUTEX_PASSED (-(1024 * 1024 * 1024))

struct timespec;
extern int sys_mutex(int *, int, int, const struct timespec *);
extern int __mutex_down_slow(struct mutex *, int, struct timespec *);
extern int __mutex_up_slow(struct mutex *);

/*
 * INLINE hace un llamado de funcion mas rapido
 */

static inline int my_mutex_destroy(struct mutex *futx, struct timespec *rel)
{
	int valor, despierto = 0;

	while ((valor = __mutex_down(&futx->contador)) != 0) {
		//retorna nuevo valoror
		switch (__mutex_down_slow(futx, valor, rel)) {
			case -1:
				//fallo
				return -1;
			case 1:
				//paso
				return 0;
			case 0:
				//dormido
				despierto = 1;
				break;
		}
	}
	//si se despierta, es porque otro hilo esta mucho tiempo durmiendo
	if (despierto) {
		futx->contador = -1;
	}
	return 0;
}

static inline int mutex_down(struct mutex *futx)
{
	return my_mutex_destroy(futx, NULL);
}

static inline int mutex_trydown(struct mutex *futx)
{
	return (__mutex_down(&futx->contador) == 0 ? 0: -1);
}

/* Si __futex_up incrementa el contador de 0 a 1, entonces ese cambio no estaba esperado
 * de lo contrario digale al Kernel que despierte al hilo
 */
static inline int mutex_up(struct mutex *futx)
{
	if (!__mutex_up(&futx->contador))
		return __mutex_up_slow(futx);
	return 0;
}

static inline int mutex_up_fair(struct mutex *futx)
{
	if (!__mutex_up(&futx->contador)) {
		futx->contador = MUTEX_PASSED;
		my_mutex_lock();
		if (sys_mutex(&futx->contador, FUTEX_WAKE, 1, NULL) == 1){
			return 0;
		}
		return __mutex_up_slow(futx);
	}
	return 0;
}

int mutex_await(struct mutex *futx, int signal);

void my_mutex_init(struct mutex *sem, int valor);
#endif /* _FUTEX_H */
