#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <mypthread.h>

//numero de hilos, se puede definir desde una funcion
//#define NTHREADS	8
#define NOT_RT		-1

/*
 * aumenta el contador en 50,
 * cuando tiene el control de regreso le suma otros 50 y sale
 */
void *thread_func(void *arg)
{
	int *count = (int *)arg;

	*count = *count + 50;
	LOG_PRINTF("Thread %ld: Incremented count by 50 and will now yield\n", (unsigned long)mythread_self().tid);
	my_thread_yield();
	*count = *count + 50;
	LOG_PRINTF("Thread %ld: Incremented count by 50 and will now exit\n", (unsigned long)mythread_self().tid);
	my_thread_end(NULL);
	return NULL;
}


/*
 * ejemplo de uso de la biblioteca
 */
int main(int argc, char *argv[])
{
	mythread_t threads[NTHREADS];
	int count[NTHREADS];
	int i;
	
	my_thread_init();

	for (i = 0; i < NTHREADS; i++) {
		count[i] = i;
		my_thread_create(&threads[i], NULL, thread_func, &count[i], NOT_RT);
	}
	my_thread_end(NULL);


	return 0;
}
