/*
	Se usara una cola para manejar los TCB correspondientes para cada hilo reado
	La cola se impleta usando una lista doblemente enlazada
  */
#include "mypthreads.h"

/* cabeza de la cola*/
mythread_private_t *mythread_q_cabeza;

void mythread_q_init(mythread_private_t * nodo)
{
	nodo->anterior = nodo;
	nodo->siguiente = nodo;

	mythread_q_cabeza = nodo;
}

void mythread_q_add(mythread_private_t * nodo)
{

	if (mythread_q_cabeza == NULL) {
		mythread_q_init(nodo);
		return;
	}
	//se inserta al final de la cola
	nodo->siguiente = mythread_q_cabeza;
	nodo->anterior = mythread_q_cabeza->anterior;
	mythread_q_cabeza->anterior->siguiente = nodo;
	mythread_q_cabeza->anterior = nodo;

	return;

}

/* elmina un nodo en especifico*/
void mythread_q_delete(mythread_private_t * nodo)
{

	mythread_private_t *p;
	if (nodo == mythread_q_cabeza && nodo->siguiente == mythread_q_cabeza) {
		mythread_q_cabeza = NULL;
	}

	if (nodo == mythread_q_cabeza)
		mythread_q_cabeza = nodo->siguiente;

	p = nodo->anterior;

	p->siguiente = nodo->siguiente;
	nodo->siguiente->anterior = p;

	return;

}

void mythread_q_estado_display()
{

	if (mythread_q_cabeza != NULL) {

		printf("\n Los contenidos son -> \n");
		mythread_private_t *p;
		p = mythread_q_cabeza;
		do {
			printf(" %d\n", p->estado);
			p = p->siguiente;
		} while (p != mythread_q_cabeza);

	}

}

mythread_private_t *mythread_q_search(unsigned long new_tid)
{

	mythread_private_t *p;
	if (mythread_q_cabeza != NULL) {

		p = mythread_q_cabeza;
		do {
			if (p->tid == new_tid)
				return p;
			p = p->siguiente;
		} while (p != mythread_q_cabeza);

	}
	return NULL;

}

int mythread_q_len()
{
	int len = 0;
	if (mythread_q_cabeza != NULL) {
		mythread_private_t *p;
		p = mythread_q_cabeza;
		do {
			++len;
			p = p->siguiente;
		} while (p != mythread_q_cabeza);

	}
	return len;
}
