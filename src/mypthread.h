#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <mutex.h>  //Aqui estan las funciones mutex
#include <string.h>

#ifndef MYTHREAD_H
#define MYTHREAD_H

#define FALSE 0
#define TRUE 1

#define RUNNING 0
#define READY 	1 //espera para ser agendado (scheduling)
#define BLOCKED 2 //esperando unirse
#define DEFUNCT 3 //muerto

#define NTHREADS	8 //numero maximo de hilos

typedef struct mythread_attr {
	//valor por defecto: SIGSTKSZ
	unsigned long stackSize;
}mythread_attr_t;

typedef struct mythread {
	pid_t tid;
}mythread_t;

/*
 * Thread Control Block (TCB)
 */
typedef struct mythread_private {

	pid_t tid; 				
	int state;
	
	//solo se usa en el algoritmo de tiempo real
	int id;//ubicacion del arreglo del algortimo 		
	int priority;//si es igual a -1, es porque no tiene prioridad
	
	//Elementos de la funcion
	void * (*start_func) (void *);//funcion donde esta asignado el hilo
	void *args;//argumentos de la funcion
	void *returnValue;//valor de retorno de la funcion
	
	struct mythread_private *blockedForJoin;
	//mutex utilizado por el despachador para programar este hilo
	struct mutex sched_mutex;
	struct mythread_private *prev, *next; 

}mythread_private_t;

//puntero a la cabeza de la cola
extern mythread_private_t *mythread_q_head;

mythread_t mythread_self(void);


/************************** thread   ****************************/
/* mythread_create
 * Crea un nuevo hilo, asignandosele a una funcion.
 * Prepara el contexto de un nuevo hilo, attr se ignora por el momento
 */
int my_thread_create(mythread_t *new_thread_ID,
					mythread_attr_t *attr,
					void * (*start_func)(void *),
					void *arg,
					int priority_A);

/* mythread_yield
 * cambia de un hilo ejecutandose a otro que este listo para ejecutarse (READY)
 */
int my_thread_yield(void);

/* my_thread_chsched
 * cambia de algoritmo de scheduling
 */
int my_thread_chsched(int sched);

/* my_thread_init
 * inicializa variables globales
 */
void my_thread_init();

/* mythread_join
 * suspende la llamada del hilo si el hilo actual no ha terminado,
 * en cola el hilo actual, luego dispatch y lo marca como ready para
 * volver a ser llamado
*/
int my_thread_join(mythread_t target_thread, void **status);

/* mythread_end
 * Cierra un hilo
 * desencola un hilo y lo mata
 */
void my_thread_end(void *retval);

/*******************************************************************/
/* Muestra el id del hilo --> pid_t
 * Esta implementado en mypthread_utilites.c
 */
pid_t __mythread_gettid();

/* Muestra el struct del hilo actual.
 */
mythread_private_t *__mythread_ptr_act();

/*************     Algoritmos      ****************/
//Funciona bien
int my_thread_detach_FIFO(mythread_private_t *);

//Se puede mejorar para que aguante mas de 100 hilos
int my_thread_detach_Lottery(mythread_private_t *);

//no sirve
int my_thread_detach_RT(mythread_private_t *);

/* Se puede implementar como FIFO
 * falta implementar
 */
int my_thread_detach_Robin(mythread_private_t *);

/*******************************************************************/

////////////////////////   Debuger   ///////////////////////////
//no quitar
void __mythread_debug_futex_init();

/*
 * Solo se usa con el debuguer
 */
extern char debug_msg[1000];
extern struct mutex debug_futex;

#ifdef DEBUG
#define DEBUG_PRINTF(...) __mythread_debug_futex_init(); \
			mutex_down(&debug_futex); \
			sprintf(debug_msg, __VA_ARGS__); \
			write(1, debug_msg, strlen(debug_msg)); \
			mutex_up(&debug_futex);
#else
#define DEBUG_PRINTF(...) do {} while(0);
#endif

#define ERROR_PRINTF(...) __mythread_debug_futex_init(); \
			mutex_down(&debug_futex); \
			sprintf(debug_msg, __VA_ARGS__); \
			write(1, debug_msg, strlen(debug_msg)); \
			mutex_up(&debug_futex);

#define LOG_PRINTF(...) __mythread_debug_futex_init(); \
			mutex_down(&debug_futex); \
			sprintf(debug_msg, __VA_ARGS__); \
			write(1, debug_msg, strlen(debug_msg)); \
			mutex_up(&debug_futex);

#endif /* MYTHREAD_H */
