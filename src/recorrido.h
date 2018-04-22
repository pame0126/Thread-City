#ifndef RECORRIDO_H
#define RECORRIDO_H

#define INF 9999

#define V 110



int matriz_nodos[V][V];

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

int**genera_ruta();

#endif
