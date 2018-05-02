#ifndef HAMBURGUESA_H
#define HAMBURGUESA_H

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
#include <unistd.h>
#include <mypthreads.h>


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototipos ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
void hacer_torta();
void hacer_pan();
void colocar_salsas();
void hacer_ensalada();
void* armar_hamburgesa1(void *esperar);
void* armar_hamburgesa2(void *esperar);
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Funciones ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
// Funciones que simulan la creacion de una hamburgesa
void hacer_torta()
{
	sleep(2);
}

void hacer_pan()
{
	sleep(2);
}

void colocar_salsas()
{
	sleep(1);
}

void hacer_ensalada()
{
	sleep(2);
}

//funcion que arma la hamburguesa
void* armar_hamburgesa1(void *esperar)
{
	//int *pausaPtr = (int *)esperar;
	//int pausa = *pausaPtr;
	mythread_yield();
	hacer_pan();
	mythread_yield();
	hacer_torta();
	mythread_yield();
	hacer_ensalada();
	mythread_yield();
    colocar_salsas();
    mythread_yield();
    printf("Hamburgesa terminada, CajeroRapido!\n");
    mythread_yield();
    mythread_end(NULL);
    return NULL;
}
//funcion que arma la hamburguesa
void* armar_hamburgesa2(void *esperar)
{
	//int *pausaPtr = (int *)esperar;
	//int pausa = *pausaPtr;
	mythread_yield();
	hacer_pan();
	mythread_yield();
	hacer_torta();
	mythread_yield();
	hacer_ensalada();
	mythread_yield();
    colocar_salsas();
    mythread_yield();
    sleep(10);
    mythread_yield();
    printf("Hamburgesa terminada, CajeroLento!\n");
    mythread_yield();
    mythread_end(NULL);
    return NULL;
}

#endif
