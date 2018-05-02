#include <unistd.h>
#include "mypthreads.h"
#include "mymutex.h"

/*
 * Crear un hilo con esta funcion. Con el objetivo de suspender
 * el nuevo hilo hasta que sea programado (scheduler) por el despachador
 */
int mythread_wrapper(void *thread_tcb)
{
	mythread_private_t *new_tcb;
	new_tcb = (mythread_private_t *) thread_tcb;
	//se suspende hasta que se despierte
	mutex_down(&new_tcb->sched_mutex);
	//cuando se despierta, se llama la funcion definida por el usuario
	new_tcb->funcion(new_tcb->argumentos);

	return 0;
}
