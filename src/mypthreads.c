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

int my_thread_wrapper(void *);
void *my_thread_idle(void *);

my_thread_private_t *my_thread_q_head;
//puntero hacia el hilo principal
my_thread_private_t *main_tcb;

my_thread_t idle_u_tcb;
//gfutex se hace global para evitar una carrera entre hilos por los recursos
struct mutex gfutex;

int scheduler;

int number_thread;

int threads_checked[NTHREADS];


void my_thread_init()
{
	int i;
	scheduler = 0;
	number_thread = 0;

	for (i = 0; i < NTHREADS; i++){
		threads_checked[i] = 0;
	}
}

/*
 * creacion del hilo principal e inserccion en la cola
 */
static int __my_thread_add_main_tcb()
{
	//DEBUG_PRINTF("add_main_tcb: Creating node for Main thread \n");
	main_tcb = (my_thread_private_t *) malloc(sizeof(my_thread_private_t));
	if (main_tcb == NULL) {
		ERROR_PRINTF("add_main_tcb: Error allocating memory for main node\n");
		return -ENOMEM;
	}

	main_tcb->start_func = NULL;
	main_tcb->args = NULL;
	main_tcb->state = READY;
	main_tcb->id = number_thread++;
	//printf("numero de hilos %i\n", number_thread);
	main_tcb->priority = -1;
	main_tcb->returnValue = NULL;
	main_tcb->blockedForJoin = NULL;

	main_tcb->tid = __my_thread_gettid();
	//se inicializa el mutex en "0"
	my_mutex_init(&main_tcb->sched_mutex, 1);

	//se agrega a la cola el hilo principal
	my_thread_q_add(main_tcb);
	return 0;
}

/*
 * Crea un contexto del proceso compartido usando clone
 * Se usa funciones de mypthread_wrapper para configurar el entorno del hilo
 * y luego llama a la funcion asociada al hilo
 */
int my_thread_create(my_thread_t * new_thread_ID,
		    my_thread_attr_t * attr,
		    void *(*start_func) (void *), void *arg, int priority_A)
{
	//puntero de la pila usado por el proceso hijo (creado por clone)
	char *child_stack;
	unsigned long stackSize;
	my_thread_private_t *new_node;
	pid_t tid;
	int retval;
	//syscall clone con sus banderas respectivas
	int clone_flags = (CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGNAL
			   | CLONE_PARENT_SETTID
			   | CLONE_CHILD_CLEARTID | CLONE_SYSVSEM);

	if (my_thread_q_head == NULL) {
		//primer hilo que se crea, por lo tanto se crea el hilo principal
		retval = __my_thread_add_main_tcb();
		if (retval != 0){
			return retval;
		}
		//se inicializa el mutex de manera global
		my_mutex_init(&gfutex, 1);
		//se crea el hilo idle
		my_thread_create(&idle_u_tcb, NULL, my_thread_idle, NULL, priority_A);
	}

	// tomado de http://foro.elhacker.net/programacion_cc-b49.0/
	/* This particular piece of code was added as a result of a weird bug encountered in the __futex_down().
	 * In 2.6.35 (our kernel version), all threads can access main thread's stack, but
	 * on the OS machine, this stack is somehow private to main thread only.
	 */
	new_node = (my_thread_private_t *) malloc(sizeof(my_thread_private_t));
	if (new_node == NULL) {
		ERROR_PRINTF("Cannot allocate memory for node\n");
		return -ENOMEM;
	}

	//si no se especifica el tamanio, entonces se usa el valor por defecto
	if (attr == NULL){
		stackSize = SIGSTKSZ;
	}else{
		stackSize = attr->stackSize;
	}
	//tomado de: http://foro.elhacker.net/programacion_cc-b49.0/
	/* posix_memalign aligns the allocated memory at a 64-bit boundry. */
	if (posix_memalign((void **)&child_stack, 8, stackSize)) {
		ERROR_PRINTF("posix_memalign failed! \n");
		return -ENOMEM;
	}

	//se deja espacio para una invocacion en la base de la pila
	child_stack = child_stack + stackSize - sizeof(sigset_t);

	//inicializacion del nuevo nodo
	new_node->start_func = start_func;
	new_node->args = arg;
	new_node->state = READY;
	new_node->id = number_thread++;
	//printf("numero de hilos %i\n", number_thread);
	new_node->priority = priority_A;
	new_node->returnValue = NULL;
	new_node->blockedForJoin = NULL;
	//mutex del tcb inicializado en 0
	my_mutex_init(&new_node->sched_mutex, 0);

	my_thread_q_add(new_node);

	if ((tid = clone(my_thread_wrapper, (char *)child_stack, clone_flags, new_node)) == -1) {
		printf("clone failed! \n");
		printf("ERROR: %s \n", strerror(errno));
		return (-errno);
	}

	new_thread_ID->tid = tid;
	new_node->tid = tid;

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
static void __my_thread_do_exit()
{
	syscall(SYS_exit, 0);
}

/*
 * Ve si algun hilo esta bloqueado para unirse.
 * Si lo esta, entonces se marca como ready y se mata a los mismo hilos
 */
void my_thread_end(void *return_val)
{
	my_thread_private_t *self_ptr;

	//obtiene el puntero del TCB
	self_ptr = __my_thread_selfptr();

	//estado muerto
	self_ptr->state = DEFUNCT;
	self_ptr->returnValue = return_val;

	//cambia el estado de cualquier que espera
	if (self_ptr->blockedForJoin != NULL)
		self_ptr->blockedForJoin->state = READY;

	//my_thread_detach(self_ptr);
	switch (scheduler){
		case 0:
			my_thread_detach_RoundRobin(self_ptr);
			break;
		case 1:
			break;
		case 2:
			my_thread_detach_Lottery(self_ptr);
			break;
		case 3:
			break;
		default:
			ERROR_PRINTF("Problems with the scheduler \n");
			exit(0);
	}

	__my_thread_do_exit();

}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//funciones para IDLE
/*
 * El hilo comprueba si es el único vivo, en caso afirmativo, exit ()
 * o sigue programando (scheduling) a otro hilo.
 */
void *my_thread_idle(void *phony)
{
	my_thread_private_t *traverse_tcb;
	pid_t idle_tcb_tid;

	while (1) {
		//DEBUG_PRINTF("I am idle\n");
		//puntero del TCB
		traverse_tcb = __my_thread_selfptr();
		idle_tcb_tid = traverse_tcb->tid;
		traverse_tcb = traverse_tcb->next;

		//si no hay un hilo con el estado DEFUNCT, entonces no mata al hilo
		while (traverse_tcb->tid != idle_tcb_tid) {
			if (traverse_tcb->state != DEFUNCT) {
				break;
			}
			traverse_tcb = traverse_tcb->next;
		}

		// si el idle es el unico hilo vivo, se mata el proceso
		if (traverse_tcb->tid == idle_tcb_tid){
			exit(0);
		}
		//si todavia falta algun hilo para ser ejecutado, entonces se ejecuta
		my_thread_yield();
	}
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//funciones de JOIN

/*
 * Espera por el hilo identificado como "target_thread"
 * Si el hilo ya murio entonces retorna los datos, si no, entonces se espera a que
 * muera para retornar los datos
 */
int my_thread_join(my_thread_t target_thread, void **status)
{
	my_thread_private_t *target, *self_ptr;

	self_ptr = __my_thread_selfptr();
	//DEBUG_PRINTF("Join: Got tid: %ld\n", (unsigned long)self_ptr->tid);
	target = my_thread_q_search(target_thread.tid);

	// si el hilo esta muerto, no necesita esperar, solo se retorna los datos
	if (target->state == DEFUNCT) {
		*status = target->returnValue;
		return 0;
	}

	//DEBUG_PRINTF("Join: Checking for blocked for join\n");
	//si el hilo no esta muerto y alguien mas esta esperando entonces da error
	if (target->blockedForJoin != NULL){
		return -1;
	}
	//se establece la espera al hilo
	target->blockedForJoin = self_ptr;
	//DEBUG_PRINTF("Join: Setting state of %ld to %d\n",(unsigned long)self_ptr->tid, BLOCKED);
	//y se pone como bloqueado para no volverlo a programar (scheduling)
	self_ptr->state = BLOCKED;

	//programar otro hilo
	my_thread_yield();

	//cuando el hilo muere, se retorna los datos del hilo
	*status = target->returnValue;
	return 0;
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//funciones para manipular la informacio del hilo

/*
 * Retorna "EL" hilo (principal)
 */
my_thread_t my_thread_self()
{
	pid_t tid;
	my_thread_t self_tcb;

	tid = __my_thread_gettid();
	self_tcb.tid = tid;

	return (self_tcb);
}

/*
 * Retorna el puntero del TCB
 */
my_thread_private_t *__my_thread_selfptr()
{
	return my_thread_q_search(__my_thread_gettid());
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//funcion para detach
//se hace varios detach, uno por cada scheduler

 /*
  * Busca por el mejor hilo adecuado que este READY para ejecutarlo
  */
int my_thread_detach_Lottery(my_thread_private_t * node)
{
	my_thread_private_t *ptr = node->next;
	//este blucle siempre se termina porque el hilo IDLE siempre esta READY
	int b;
	b = rand() % 11;
	while (b > 0) {
		ptr = ptr->next;
		b--;
	}
	while (ptr->state != READY){

		ptr = ptr->next;
	}
	//si ningun otro esta en ready, entonces no se hace nada
	if (ptr == node){
		return -1;
	}else {
		//DEBUG_PRINTF("Dispatcher: Wake-up:%ld Sleep:%ld %d %d\n",(unsigned long)ptr->tid, (unsigned long)node->tid,ptr->sched_mutex.count, ptr->state);

		//despierta el hilo "target_thread"
		mutex_up(&ptr->sched_mutex);

		//DEBUG_PRINTF("Dispatcher: Woken up:%ld, to %d\n",(unsigned long)ptr->tid, ptr->sched_mutex.count);

		return 0;
	}
}

/*
  * Busca por el mejor hilo adecuado que este READY para ejecutarlo
  */
int my_thread_detach_RoundRobin(my_thread_private_t * node)
{
	my_thread_private_t *ptr = node->next;
	//este blucle siempre se termina porque el hilo IDLE siempre esta READY
	while (ptr->state != READY){
		//fifo
		ptr = ptr->next;

	}
	//si ningun otro esta en ready, entonces no se hace nada
	if (ptr == node){
		return -1;
	}else {
		//DEBUG_PRINTF("Dispatcher: Wake-up:%ld Sleep:%ld %d %d\n",(unsigned long)ptr->tid, (unsigned long)node->tid,ptr->sched_mutex.count, ptr->state);

		//despierta el hilo "target_thread"
		mutex_up(&ptr->sched_mutex);

		//DEBUG_PRINTF("Dispatcher: Woken up:%ld, to %d\n",(unsigned long)ptr->tid, ptr->sched_mutex.count);

		return 0;
	}
}

/*
  * Busca por el mejor hilo adecuado que este READY para ejecutarlo
  * algoritmo de tiempo real, por prioridad
  */
int my_thread_detach_RT(my_thread_private_t * node)
{
	my_thread_private_t *ptr = node->next;
	int priority_A = 0;
	my_thread_private_t *aux = NULL;
	//printf("pid = %i\n", node->tid);
	//printf("numero de hilos %i\n", number_thread);
	while (ptr->tid != node->tid){
		//printf("hola %i\n", ptr->tid);
		if (threads_checked[ptr->id] == 0) {
			if (ptr->priority >= priority_A) {
				priority_A = ptr->priority;
				aux = ptr;
			}
		}
		ptr = ptr->next;

	}
	//printf("pid = %i\n", ptr->tid);
	ptr = aux;
	threads_checked[ptr->id] = 1;
	//printf("pid = %i\n", ptr->tid);
	//printf("priridad = %i\n", ptr->priority);

	//si ningun otro esta en ready, entonces no se hace nada
	if (ptr == node){
		return -1;
	}else {
		//DEBUG_PRINTF("Dispatcher: Wake-up:%ld Sleep:%ld %d %d\n",(unsigned long)ptr->tid, (unsigned long)node->tid,ptr->sched_mutex.count, ptr->state);

		//despierta el hilo "target_thread"
		mutex_up(&ptr->sched_mutex);

		//DEBUG_PRINTF("Dispatcher: Woken up:%ld, to %d\n",(unsigned long)ptr->tid, ptr->sched_mutex.count);

		return 0;
	}
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//funciones del yield

/*
 * my_thread_detach() selecciona el mejor hilo, lo despierta,
 * y el hilo actual se duerme
 * Al hilo actual se le da el procesador
 */
int my_thread_yield()
{
	my_thread_private_t *self;
	int retval;

	self = __my_thread_selfptr();

	//el mutex es global para evitar carreras y deadblocks
	mutex_down(&gfutex);

	//retval = my_thread_detach(self);

	switch (scheduler){
		case 0:
			retval = my_thread_detach_RoundRobin(self);
			break;
		case 1:
			break;
		case 2:
			retval = my_thread_detach_Lottery(self);
			break;
		case 3:
			retval = my_thread_detach_RT(self);
			break;
		default:
			ERROR_PRINTF("Problems with the scheduler \n");
			exit(0);
	}


	//si no hay mas hilos aparte del principal, no se hace nada
	if (retval == -1) {
		mutex_up(&gfutex);
		return 0;
	}

	//DEBUG_PRINTF("Yield: Might sleep on first down %ld %d\n",(unsigned long)self->tid, self->sched_mutex.count);

	if (self->sched_mutex.count > 0){
		mutex_down(&self->sched_mutex);
	}
	mutex_up(&gfutex);

	//DEBUG_PRINTF("Yield: Might sleep on second down %ld %d\n",(unsigned long)self->tid, self->sched_mutex.count);
	//se duerme el hilo hasta que otro lo despierte
	mutex_down(&self->sched_mutex);

	return 0;
}
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
int my_thread_chsched(int sched)
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
