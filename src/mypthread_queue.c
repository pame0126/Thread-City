/*
	Se usara una cola para manejar los TCB correspondientes para cada hilo reado
	La cola se impleta usando una lista doblemente enlazada
  */
#include "mypthreads.h"

/* cabeza de la cola*/
my_thread_private_t *my_thread_q_cabeza;

void my_thread_q_init(my_thread_private_t * nodo)
{
	nodo->anterior = nodo;
	nodo->siguiente = nodo;

	my_thread_q_cabeza = nodo;
}

void my_thread_q_add(my_thread_private_t * nodo)
{

	if (my_thread_q_cabeza == NULL) {
		my_thread_q_init(nodo);
		return;
	}
	//se inserta al final de la cola
	nodo->siguiente = my_thread_q_cabeza;
	nodo->anterior = my_thread_q_cabeza->anterior;
	my_thread_q_cabeza->anterior->siguiente = nodo;
	my_thread_q_cabeza->anterior = nodo;

	return;

}

/* elmina un nodo en especifico*/
void my_thread_q_delete(my_thread_private_t * nodo)
{

	my_thread_private_t *p;
	if (nodo == my_thread_q_cabeza && nodo->siguiente == my_thread_q_cabeza) {
		printf("The Queue is now Empty!\n");
		my_thread_q_cabeza = NULL;
	}

	if (nodo == my_thread_q_cabeza)
		my_thread_q_cabeza = nodo->siguiente;

	p = nodo->anterior;

	p->siguiente = nodo->siguiente;
	nodo->siguiente->anterior = p;

	return;

}

void my_thread_q_estado_display()
{

	if (my_thread_q_cabeza != NULL) {

		printf("\n The Queue contents are -> \n");
		my_thread_private_t *p;
		p = my_thread_q_cabeza;
		do {
			printf(" %d\n", p->estado);
			p = p->siguiente;
		} while (p != my_thread_q_cabeza);

	}

}

my_thread_private_t *my_thread_q_search(unsigned long new_tid)
{

	my_thread_private_t *p;
	if (my_thread_q_cabeza != NULL) {

		p = my_thread_q_cabeza;
		do {		//traverse to the last nodo in Q
			if (p->tid == new_tid)
				return p;
			p = p->siguiente;
		} while (p != my_thread_q_cabeza);

	}
	return NULL;

}
