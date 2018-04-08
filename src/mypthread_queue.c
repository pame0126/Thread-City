/*
	Se usara una cola para manejar los TCB correspondientes para cada hilo reado
	La cola se impleta usando una lista doblemente enlazada
  */
#include "mypthreads.h"

/* cabeza de la cola*/
my_thread_private_t *my_thread_q_head;

void my_thread_q_init(my_thread_private_t * node)
{
	node->prev = node;
	node->next = node;

	my_thread_q_head = node;
}

void my_thread_q_add(my_thread_private_t * node)
{

	if (my_thread_q_head == NULL) {
		my_thread_q_init(node);
		return;
	}
	//se inserta al final de la cola
	node->next = my_thread_q_head;
	node->prev = my_thread_q_head->prev;
	my_thread_q_head->prev->next = node;
	my_thread_q_head->prev = node;

	return;

}

/* elmina un nodo en especifico*/
void my_thread_q_delete(my_thread_private_t * node)
{

	my_thread_private_t *p;
	if (node == my_thread_q_head && node->next == my_thread_q_head) {
		printf("The Queue is now Empty!\n");
		my_thread_q_head = NULL;
	}

	if (node == my_thread_q_head)
		my_thread_q_head = node->next;

	p = node->prev;

	p->next = node->next;
	node->next->prev = p;

	return;

}

void my_thread_q_state_display()
{

	if (my_thread_q_head != NULL) {

		printf("\n The Queue contents are -> \n");
		my_thread_private_t *p;
		p = my_thread_q_head;
		do {
			printf(" %d\n", p->state);
			p = p->next;
		} while (p != my_thread_q_head);

	}

}

my_thread_private_t *my_thread_q_search(unsigned long new_tid)
{

	my_thread_private_t *p;
	if (my_thread_q_head != NULL) {

		p = my_thread_q_head;
		do {		//traverse to the last node in Q
			if (p->tid == new_tid)
				return p;
			p = p->next;
		} while (p != my_thread_q_head);

	}
	return NULL;

}
