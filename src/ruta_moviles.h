#ifndef RUTA_MOVILES_H
#define RUTA_MOVILES_H

#define SIZE_AUTOS 30

#define SIZE_X 9
#define SIZE_Y 7

int matriz_ciudad[SIZE_X][SIZE_Y];

#define PRIORIDAD_BAJA 0
#define PRIORIDAD_ALTA 1


#define PUENTE 1
#define PUENTE_BLOQUEADO 2

#define BARCO 5


#define ROJO 3
#define VERDE 0

typedef struct carro{
	pid_t id;
	int len_ruta;
	int *arregloX;//ruta posiciones x
	int *arregloY;//ruta posiciones y
	int prioridad;
	int i;
	int j;
}carro;

typedef struct automomiviles{
	struct carro **list_carros;
	int len;
}autos;


autos*inicia_lista_carros();

int**genera_ruta_carro();

void *print_matriz();

void generar_lista_carros(autos*lista, int cant);

void*arrancar_carro(void*arg);

void*control_semaforos(void*arg);

void*puente_un_carril(void*arg);

#endif
