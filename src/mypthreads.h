#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymutex.h"

#ifndef mythreads_H
#define mythreads_H

#define FALSE 0
#define TRUE 1

#define RUNNING 0
#define READY 	1 //espera para ser agendado (scheduling)
#define BLOCKED 2 //esperando unirse
#define DEFUNCT 3 //muerto

#define NTHREADS	8 //numero maximo de hilos

typedef struct my_thread_attr {
	//valor por defecto: SIGSTKSZ
	unsigned long stackSize;
}my_thread_attr_t;

typedef struct my_thread {
	pid_t tid;
}my_thread_t;

/*
 * Thread Control Block (TCB)
 */
typedef struct my_thread_private {

	pid_t tid;
	int state;
	//solo se usa en el algortimo de tiempo real, ubicacion del arreglo del algortimo
	int id;
	//solo se usa en el algoritmo de tiempo real, si es igual a -1, es porque no tiene prioridad
	int priority;
	//funcion donde esta asignado el hilo
	void * (*start_func) (void *);
	//argumentos de la funcion
	void *args;
	//valor de retorno de la funcion
	void *returnValue;
	struct my_thread_private *blockedForJoin;
	//mutex utilizado por el despachador para programar este hilo
	struct mutex sched_mutex;
	struct my_thread_private *prev, *next;

}my_thread_private_t;

//puntero a la cabeza de la cola
extern my_thread_private_t *my_thread_q_head;

my_thread_t my_thread_self(void);

/*
 * Prepara el contexto de un nuevo hilo, attr se ignora por el momento
 */
int my_thread_create(my_thread_t *new_thread_ID,
					my_thread_attr_t *attr,
					void * (*start_func)(void *),
					void *arg,
					int priority_A);

/*
 * cambia de un hilo ejecutandose a otro que este listo para ejecutarse (READY)
 */
int my_thread_yield(void);

/*
 * cambia de algoritmo de scheduling
 */
int my_thread_chsched(int sched);

/*
 * inicializa variables globales
 */
void my_thread_init();

/*
 * suspende la llamada del hilo si el hilo actual no ha terminado,
 * en cola el hilo actual, luego dispatch y lo marca como ready para
 * volver a ser llamado
*/
int my_thread_join(my_thread_t target_thread, void **status);

/*
 * desencola un hilo y lo mata
 */
void my_thread_end(void *retval);

pid_t __my_thread_gettid();

my_thread_private_t *__my_thread_selfptr();

int my_thread_detach_RoundRobin(my_thread_private_t *);

int my_thread_detach_Lottery(my_thread_private_t *);

int my_thread_detach_RT(my_thread_private_t *);

void __my_thread_debug_futex_init();

/*
 * Solo se usa con el debuguer
 */
extern char debug_msg[1000];
extern struct mutex debug_futex;

#ifdef DEBUG
#define DEBUG_PRINTF(...) __my_thread_debug_futex_init(); \
			mutex_down(&debug_futex); \
			sprintf(debug_msg, __VA_ARGS__); \
			write(1, debug_msg, strlen(debug_msg)); \
			mutex_up(&debug_futex);
#else
#define DEBUG_PRINTF(...) do {} while(0);
#endif

#define ERROR_PRINTF(...) __my_thread_debug_futex_init(); \
			mutex_down(&debug_futex); \
			sprintf(debug_msg, __VA_ARGS__); \
			write(1, debug_msg, strlen(debug_msg)); \
			mutex_up(&debug_futex);

#define LOG_PRINTF(...) __my_thread_debug_futex_init(); \
			mutex_down(&debug_futex); \
			sprintf(debug_msg, __VA_ARGS__); \
			write(1, debug_msg, strlen(debug_msg)); \
			mutex_up(&debug_futex);

#endif /* mythread_H */
