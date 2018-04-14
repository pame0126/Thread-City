#define _GNU_SOURCE
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
//////////////////////
//////////////////////
//bibliotecas necesaria para clone
#include <signal.h>
#include <errno.h>
#include <sched.h>
//////////////////////
//////////////////////
#include "mypthreads.h"
#include "mymutex.h"
#include "mypthread_queue.h"
#include <sys/syscall.h>
#include <sys/types.h>

#define CLONE_SIGNAL            (CLONE_SIGHAND | CLONE_THREAD)

int mythread_wrapper(void *);
void *mythread_idle(void *);

mythread_private_t *cabeza_thread_q;
//puntero hacia el hilo principal
mythread_private_t *tcb_principal;

mythread_t idle_u_tcb;
//gfutex se hace global para evitar una carrera entre hilos por los recursos
struct mutex gfutex;

int scheduler;

int numero_hilo;

int hilos_inspeccionados[NTHREADS];


void mythread_init()
{
	int i;
	scheduler = 0;
	numero_hilo = 0;

	for (i = 0; i < NTHREADS; i++){
		hilos_inspeccionados[i] = 0;
	}
}

/*
 * creacion del hilo principal e inserccion en la cola
 */
static int __mythread_add_tcb_principal()
{
	//DEBUG_PRINTF("add_tcb_principal: Creating nodo for Main thread \n");
	tcb_principal = (mythread_private_t *) malloc(sizeof(mythread_private_t));
	if (tcb_principal == NULL) {
		ERROR_PRINTF("add_tcb_principal: Error allocating memory for main nodo\n");
		return -ENOMEM;
	}

	tcb_principal->funcion = NULL;
	tcb_principal->argumentos = NULL;
	tcb_principal->estado = PREPARADO;
	tcb_principal->id = numero_hilo++;
	//printf("numero de hilos %i\n", numero_hilo);
	tcb_principal->prioridad = -1;
	tcb_principal->valorRetorno = NULL;
	tcb_principal->bloqueadoPorJoin = NULL;

	tcb_principal->tid = __mythread_gettid();
	//se inicializa el mutex en "0"
	my_mutex_init(&tcb_principal->sched_mutex, 1);

	//se agrega a la cola el hilo principal
	mythread_q_add(tcb_principal);
	return 0;
}

/*
 * Crea un contexto del proceso compartido usando clone
 * Se usa funciones de mypthread_wrapper para configurar el entorno del hilo
 * y luego llama a la funcion asociada al hilo
 */
int mythread_create(mythread_t * new_thread_ID,
		    mythread_attr_t * attr,
		    void *(*funcion) (void *), void *arg, int prioridad_A)
{
	//puntero de la pila usado por el proceso hijo (creado por clone)
	char *nueva_pila;
	unsigned long largoPila;
	mythread_private_t *nuevo_nodo;
	pid_t tid;
	int valorRetorno;
	//syscall clone con sus banderas respectivas
	int banderas_clonadas = (CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGNAL
			   | CLONE_PARENT_SETTID
			   | CLONE_CHILD_CLEARTID | CLONE_SYSVSEM);

	if (cabeza_thread_q == NULL) {
		//primer hilo que se crea, por lo tanto se crea el hilo principal
		valorRetorno = __mythread_add_tcb_principal();
		if (valorRetorno != 0){
			return valorRetorno;
		}
		//se inicializa el mutex de manera global
		my_mutex_init(&gfutex, 1);
		//se crea el hilo idle
		mythread_create(&idle_u_tcb, NULL, mythread_idle, NULL, prioridad_A);
	}

	// tomado de http://foro.elhacker.net/programacion_cc-b49.0/
	/* This particular piece of code was added as a result of a weird bug encontadorered in the __futex_down().
	 * In 2.6.35 (our kernel version), all threads can access main thread's stack, but
	 * on the OS machine, this stack is somehow private to main thread only.
	 */
	nuevo_nodo = (mythread_private_t *) malloc(sizeof(mythread_private_t));
	if (nuevo_nodo == NULL) {
		ERROR_PRINTF("Cannot allocate memory for nodo\n");
		return -ENOMEM;
	}

	//si no se especifica el tamanio, entonces se usa el valor por defecto
	if (attr == NULL){
		largoPila = SIGSTKSZ;
	}else{
		largoPila = attr->largoPila;
	}
	//tomado de: http://foro.elhacker.net/programacion_cc-b49.0/
	/* posix_memalign aligns the allocated memory at a 64-bit boundry. */
	if (posix_memalign((void **)&nueva_pila, 8, largoPila)) {
		ERROR_PRINTF("posix_memalign failed! \n");
		return -ENOMEM;
	}

	//se deja espacio para una invocacion en la base de la pila
	nueva_pila = nueva_pila + largoPila - sizeof(sigset_t);

	//inicializacion del nuevo nodo
	nuevo_nodo->funcion = funcion;
	nuevo_nodo->argumentos = arg;
	nuevo_nodo->estado = PREPARADO;
	nuevo_nodo->id = numero_hilo++;
	//printf("numero de hilos %i\n", numero_hilo);
	nuevo_nodo->prioridad = prioridad_A;
	nuevo_nodo->valorRetorno = NULL;
	nuevo_nodo->bloqueadoPorJoin = NULL;
	//mutex del tcb inicializado en 0
	my_mutex_init(&nuevo_nodo->sched_mutex, 0);

	mythread_q_add(nuevo_nodo);

	if ((tid = clone(mythread_wrapper, (char *)nueva_pila, banderas_clonadas, nuevo_nodo)) == -1) {
		printf("clone failed! \n");
		printf("ERROR: %s \n", strerror(errno));
		return (-errno);
	}

	new_thread_ID->tid = tid;
	nuevo_nodo->tid = tid;

	//DEBUG_PRINTF("create: Finished initialising new thread: %ld\n", (unsigned long)new_thread_ID->tid);

	return 0;
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//funciones para END

/*
 * mata al hilo
 */
static void __mythread_do_exit()
{
	syscall(SYS_exit, 0);
}

/*
 * Ve si algun hilo esta bloqueado para unirse.
 * Si lo esta, entonces se marca como PREPARADO y se mata a los mismo hilos
 */
void mythread_end(void *valorRetorn)
{
	mythread_private_t *hilo;

	//obtiene el puntero del TCB
	hilo = __mythread_selfptr();

	//estado muerto
	hilo->estado = MUERTO;
	hilo->valorRetorno = valorRetorn;

	//cambia el estado de cualquier que espera
	if (hilo->bloqueadoPorJoin != NULL)
		hilo->bloqueadoPorJoin->estado = PREPARADO;

	//mythread_detach(hilo);
	switch (scheduler){
		case 0:
			mythread_detach_RoundRobin(hilo);
			break;
		case 1:
			break;
		case 2:
			mythread_detach_Lottery(hilo);
			break;
		case 3:
			break;
		default:
			ERROR_PRINTF("Problems with the scheduler \n");
			exit(0);
	}

	__mythread_do_exit();

}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//funciones para IDLE
/*
 * El hilo comprueba si es el único vivo, en caso afirmativo, exit ()
 * o sigue programando (scheduling) a otro hilo.
 */
void *mythread_idle(void *vacio)
{
	mythread_private_t *tcb_alterna;
	pid_t idle_tcb_tid;

	while (1) {
		//DEBUG_PRINTF("I am idle\n");
		//puntero del TCB
		tcb_alterna = __mythread_selfptr();
		idle_tcb_tid = tcb_alterna->tid;
		tcb_alterna = tcb_alterna->siguiente;

		//si no hay un hilo con el estado MUERTO, entonces no mata al hilo
		while (tcb_alterna->tid != idle_tcb_tid) {
			if (tcb_alterna->estado != MUERTO) {
				break;
			}
			tcb_alterna = tcb_alterna->siguiente;
		}

		// si el idle es el unico hilo vivo, se mata el proceso
		if (tcb_alterna->tid == idle_tcb_tid){
			exit(0);
		}
		//si todavia falta algun hilo para ser ejecutado, entonces se ejecuta
		mythread_yield();
	}
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//funciones de JOIN

/*
 * Espera por el hilo identificado como "destino_thread"
 * Si el hilo ya murio entonces retorna los datos, si no, entonces se espera a que
 * muera para retornar los datos
 */
int mythread_join(mythread_t destino_thread, void **estado)
{
	mythread_private_t *destino, *hilo;

	hilo = __mythread_selfptr();
	//DEBUG_PRINTF("Join: Got tid: %ld\n", (unsigned long)hilo->tid);
	destino = mythread_q_search(destino_thread.tid);

	// si el hilo esta muerto, no necesita esperar, solo se retorna los datos
	if (destino->estado == MUERTO) {
		*estado = destino->valorRetorno;
		return 0;
	}

	//DEBUG_PRINTF("Join: Checking for BLOQUEADO for join\n");
	//si el hilo no esta muerto y alguien mas esta esperando entonces da error
	if (destino->bloqueadoPorJoin != NULL){
		return -1;
	}
	//se establece la espera al hilo
	destino->bloqueadoPorJoin = hilo;
	//DEBUG_PRINTF("Join: Setting estado of %ld to %d\n",(unsigned long)hilo->tid, BLOQUEADO);
	//y se pone como bloqueado para no volverlo a programar (scheduling)
	hilo->estado = BLOQUEADO;

	//programar otro hilo
	mythread_yield();

	//cuando el hilo muere, se retorna los datos del hilo
	*estado = destino->valorRetorno;
	return 0;
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//funciones para manipular la informacio del hilo

/*
 * Retorna "EL" hilo (principal)
 */
mythread_t mythread_self()
{
	pid_t tid;
	mythread_t tcb;

	tid = __mythread_gettid();
	tcb.tid = tid;

	return (tcb);
}

/*
 * Retorna el puntero del TCB
 */
mythread_private_t *__mythread_selfptr()
{
	return mythread_q_search(__mythread_gettid());
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//funcion para detach
//se hace varios detach, uno por cada scheduler

 /*
  * Busca por el mejor hilo adecuado que este PREPARADO para ejecutarlo
  */
int mythread_detach_Lottery(mythread_private_t * nodo)
{
	mythread_private_t *puntero = nodo->siguiente;
	//este blucle siempre se termina porque el hilo IDLE siempre esta PREPARADO
	int b;
	b = rand() % 11;
	while (b > 0) {
		puntero = puntero->siguiente;
		b--;
	}
	while (puntero->estado != PREPARADO){

		puntero = puntero->siguiente;
	}
	//si ningun otro esta en PREPARADO, entonces no se hace nada
	if (puntero == nodo){
		return -1;
	}else {
		//DEBUG_PRINTF("Dispatcher: Wake-up:%ld Sleep:%ld %d %d\n",(unsigned long)ptr->tid, (unsigned long)nodo->tid,ptr->sched_mutex.contador, ptr->estado);

		//despierta el hilo "destino_thread"
		mutex_up(&puntero->sched_mutex);

		//DEBUG_PRINTF("Dispatcher: Woken up:%ld, to %d\n",(unsigned long)ptr->tid, ptr->sched_mutex.contador);

		return 0;
	}
}

/*
  * Busca por el mejor hilo adecuado que este PREPARADO para ejecutarlo
  */
int mythread_detach_RoundRobin(mythread_private_t * nodo)
{
	mythread_private_t *puntero = nodo->siguiente;
	//este blucle siempre se termina porque el hilo IDLE siempre esta PREPARADO
	while (puntero->estado != PREPARADO){
		//fifo
		puntero = puntero->siguiente;

	}
	//si ningun otro esta en PREPARADO, entonces no se hace nada
	if (puntero == nodo){
		return -1;
	}else {
		//DEBUG_PRINTF("Dispatcher: Wake-up:%ld Sleep:%ld %d %d\n",(unsigned long)ptr->tid, (unsigned long)nodo->tid,ptr->sched_mutex.contador, ptr->estado);

		//despierta el hilo "destino_thread"
		mutex_up(&puntero->sched_mutex);

		//DEBUG_PRINTF("Dispatcher: Woken up:%ld, to %d\n",(unsigned long)ptr->tid, ptr->sched_mutex.contador);

		return 0;
	}
}

/*
  * Busca por el mejor hilo adecuado que este PREPARADO para ejecutarlo
  * algoritmo de tiempo real, por prioridad
  */
int mythread_detach_RT(mythread_private_t * nodo)
{
	mythread_private_t *puntero = nodo->siguiente;
	int prioridad_A = 0;
	mythread_private_t *auxiliar = NULL;
	//printf("pid = %i\n", nodo->tid);
	//printf("numero de hilos %i\n", numero_hilo);
	while (puntero->tid != nodo->tid){
		//printf("hola %i\n", ptr->tid);
		if (hilos_inspeccionados[puntero->id] == 0) {
			if (puntero->prioridad >= prioridad_A) {
				prioridad_A = puntero->prioridad;
				auxiliar = puntero;
			}
		}
		puntero = puntero->siguiente;

	}
	//printf("pid = %i\n", ptr->tid);
	puntero = auxiliar;
	hilos_inspeccionados[puntero->id] = 1;
	//printf("pid = %i\n", ptr->tid);
	//printf("priridad = %i\n", ptr->prioridad);

	//si ningun otro esta en PREPARADO, entonces no se hace nada
	if (puntero == nodo){
		return -1;
	}else {
		//DEBUG_PRINTF("Dispatcher: Wake-up:%ld Sleep:%ld %d %d\n",(unsigned long)ptr->tid, (unsigned long)nodo->tid,ptr->sched_mutex.contador, ptr->estado);

		//despierta el hilo "destino_thread"
		mutex_up(&puntero->sched_mutex);

		//DEBUG_PRINTF("Dispatcher: Woken up:%ld, to %d\n",(unsigned long)ptr->tid, ptr->sched_mutex.contador);

		return 0;
	}
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//funciones del yield

/*
 * mythread_detach() selecciona el mejor hilo, lo despierta,
 * y el hilo actual se duerme
 * Al hilo actual se le da el procesador
 */
int mythread_yield()
{
	mythread_private_t *hilo;
	int valorRetorno;

	hilo = __mythread_selfptr();

	//el mutex es global para evitar carreras y deadblocks
	mutex_down(&gfutex);

	//valorRetorno = mythread_detach(self);

	switch (scheduler){
		case 0:
			valorRetorno = mythread_detach_RoundRobin(hilo);
			break;
		case 1:
			break;
		case 2:
			valorRetorno = mythread_detach_Lottery(hilo);
			break;
		case 3:
			valorRetorno = mythread_detach_RT(hilo);
			break;
		default:
			ERROR_PRINTF("Problems with the scheduler \n");
			exit(0);
	}


	//si no hay mas hilos aparte del principal, no se hace nada
	if (valorRetorno == -1) {
		mutex_up(&gfutex);
		return 0;
	}

	//DEBUG_PRINTF("Yield: Might sleep on first down %ld %d\n",(unsigned long)self->tid, self->sched_mutex.contador);

	if (hilo->sched_mutex.contador > 0){
		mutex_down(&hilo->sched_mutex);
	}
	mutex_up(&gfutex);

	//DEBUG_PRINTF("Yield: Might sleep on second down %ld %d\n",(unsigned long)self->tid, self->sched_mutex.contador);
	//se duerme el hilo hasta que otro lo despierte
	mutex_down(&hilo->sched_mutex);

	return 0;
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
int mythread_chsched(int sched)
{
	scheduler = sched;
	switch (sched){
		case 0:
			LOG_PRINTF("Changed Scheduler to FIFO\n");
			break;
		case 1:
			scheduler = 0;//se pone esta linea porque aun no hay algoritmo designado
			LOG_PRINTF("Changed Scheduler to Round Robin\n");
			break;
		case 2:
			LOG_PRINTF("Changed Scheduler to Lottery\n");
			break;
		case 3:
			LOG_PRINTF("Changed Scheduler to Real Time\n");
			break;
		default:
			ERROR_PRINTF("Incorrect option, possible options 0 - 3 \n");
			exit(0);
			return 1;

	}

	return 0;

}
