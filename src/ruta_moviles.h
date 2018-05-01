#ifndef RUTA_MOVILES_H
#define RUTA_MOVILES_H

#define SIZE_AUTOS 10
#define SIZE_BARCOS 3
#define SIZE_AMBULANCIAS 4

#define SIZE_X 11
#define SIZE_Y 10

int matriz_ciudad[SIZE_X][SIZE_Y];
int ciudad_edificios[SIZE_X][SIZE_Y];

#define PRIORIDAD_BAJA 0
#define PRIORIDAD_ALTA 1


#define PUENTE 11
#define PUENTE_BLOQUEADO 22
#define PUENTE_ESPERA 33
#define BARCO 44
#define ROJO 55
#define VERDE 0
#define AMBULANCIA 99
#define PLANTA_NUCLEAR 666
#define PLANTA_DESTRUIDA 777


void*pausa(void*arg);

int**genera_ruta_carro();

void *print_matriz();

void*carro(void*arg);

void*control_semaforos(void*arg);

void*puente_un_carril(void*arg);

void*barco(void*arg);

void*planta_nuclear(void*arg);

void*ambulancia(void*arg);
#endif
