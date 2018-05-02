#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mymutex.h"

#ifndef mythreads_H
#define mythreads_H

#define FALSE 0
#define TRUE 1

#define CORRIENDO 0
#define PREPARADO 1 //espera para ser agendado (scheduling)
#define BLOQUEADO 2 //esperando unirse
#define MUERTO 3 //muerto

#define NTHREADS 8 //numero maximo de hilos

typedef struct mythread_attr {
	//valor por defecto: SIGSTKSZ
	unsigned long largoPila;
}mythread_attr_t;

typedef struct mythread {
	pid_t tid;
}mythread_t;

/*
 * Thread Control Block (TCB)
 */
typedef struct mythread_private {

	pid_t tid;
	int estado;
	//solo se usa en el algortimo de tiempo real, ubicacion del arreglo del algortimo
	int id;
	//solo se usa en el algoritmo de tiempo real, si es igual a -1, es porque no tiene prioridad
	int prioridad;
	//funcion donde esta asignado el hilo
	void * (*funcion) (void *);
	//argumentos de la funcion
	void *argumentos;
	//valor de retorno de la funcion
	void *valorRetorno;
	struct mythread_private *bloqueadoPorJoin;
	//mutex utilizado por el despachador para programar este hilo
	struct mutex sched_mutex;
	struct mythread_private *anterior, *siguiente;

}mythread_private_t;

//puntero a la cabeza de la cola
mythread_private_t *mythread_q_cabeza;

mythread_t mythread_self(void);

/*
 * Prepara el contexto de un nuevo hilo, attr se ignora por el momento
 */
int mythread_create(mythread_t *thread_ID,
					mythread_attr_t *attr,
					void * (*funcion)(void *),
					void *argumentos,
					int prioridad_A);

/*
 * cambia de un hilo ejecutandose a otro que este listo para ejecutarse (READY)
 */
int mythread_yield(void);

/*
 * cambia de algoritmo de scheduling
 */
int mythread_chsched(int sched);

/*
 * inicializa variables globales
 */
void mythread_init();

/*
 * suspende la llamada del hilo si el hilo actual no ha terminado,
 * en cola el hilo actual, luego dispatch y lo marca como ready para
 * volver a ser llamado
*/
int mythread_join(mythread_t target_thread, void **estado);

/*
 * desencola un hilo y lo mata
 */
void mythread_end(void *valorRetorno);

pid_t __mythread_gettid();

mythread_private_t *__mythread_selfptr();

int mythread_detach_RoundRobin(mythread_private_t *);

int mythread_detach_Lottery(mythread_private_t *);

int mythread_detach_RT(mythread_private_t *);

void __mythread_debug_futex_init();

#endif /* mythread_H */
