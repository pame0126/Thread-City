#include <unistd.h>
#include "mypthreads.h"
#include "mymutex.h"

/*
 * Crear un hilo con esta funcion. Con el objetivo de suspender
 * el nuevo hilo hasta que sea programado (scheduler) por el despachador
 */
int my_thread_wrapper(void *tcb)
{
	my_thread_private_t *nuevo_tcb;
	nuevo_tcb = (my_thread_private_t *) tcb;

	//DEBUG_PRINTF("Wrapper: will sleep on futex: %ld %d\n",(unsigned long)__my_thread_gettid(),nuevo_tcb->sched_mutex.count);

	//se suspende hasta que se despierte
	mutex_down(&nuevo_tcb->sched_mutex);

	//DEBUG_PRINTF("Wrapper: futex value: %ld %d\n",(unsigned long)nuevo_tcb->tid, nuevo_tcb->sched_mutex.count);

	//cuando se despierta, se llama la funcion definida por el usuario
	nuevo_tcb->funcion(nuevo_tcb->argumentos);

	return 0;
}
