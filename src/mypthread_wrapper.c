#include <unistd.h>
#include <mypthread.h>
#include <mutex.h>

/*
 * Crear un hilo con esta funcion. Con el objetivo de suspender
 * el nuevo hilo hasta que sea programado (scheduler) por el despachador
 */
int mythread_wrapper(void *thread_tcb)
{
	mythread_private_t *new_tcb;
	new_tcb = (mythread_private_t *) thread_tcb;

	//DEBUG_PRINTF("Wrapper: will sleep on futex: %ld %d\n",(unsigned long)__mythread_gettid(),new_tcb->sched_mutex.count);

	//se suspende hasta que se despierte
	mutex_down(&new_tcb->sched_mutex);

	//DEBUG_PRINTF("Wrapper: futex value: %ld %d\n",(unsigned long)new_tcb->tid, new_tcb->sched_mutex.count);

	//cuando se despierta, se llama la funcion definida por el usuario
	new_tcb->start_func(new_tcb->args);

	return 0;
}
