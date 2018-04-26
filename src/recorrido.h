#ifndef RECORRIDO_H
#define RECORRIDO_H

#define INF 9999

#define V 110
#define R 210
#define ARRAY_AMBULANCIA 84
#define ARRAY_PLANTA_NUCLEAR 16

////////////arreglo de rutas////////////////

int matriz_nodos[V][V];

int **rutas_carros[R];

int **rutas_ambulancias[ARRAY_AMBULANCIA];

int **rutas_planta_nuclear[ARRAY_PLANTA_NUCLEAR];

//////////////////////////////////////////////////////

int Minimo(int x, int y);

int returnFila(int valor);

int* returnFilas(int* arreglo);

int returnColumna(int valor);

int* returnColumnas(int* arreglo);

int EstaMarcado(int v, int VerticesMarcados[], int markedVerticesIdx);

//Encuentra el camino mas corto en la matriz de nodos
int* dijkstra(int graph[V][V], int ini, int dest);

//inicializa la matriz de adyacencia
void ini_matriz_nodos();

int**genera_ruta(int,int);

void cargar_rutas();

#endif
