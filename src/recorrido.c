
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <recorrido.h>

int origenes[15] =   {11,13,11,17,18,31,33,35,37,38,91,93,95,97,98};
int destinos[15] = {11,13,15,17,18,31,33,35,37,38,91,93,95,97,98};
/**
 * Valor minimo
 */
int Minimo(int x, int y) {
	if (x < y) {
		return x;
	}
	return y;
}

//retorna filas del arreglo
int* returnFilas(int* arreglo){
	 int tan = arreglo[0]+1;
	 int* arreglox=(int*)calloc(tan+1, sizeof(int));
	//memset( arreglox, 0, tan*sizeof(int) );
	arreglox[0]=tan;
	for(int i=1;i<=tan;i++){
		arreglox[i]=arreglo[i] / 10;
	}
	return arreglox;
}


//retorna columnas del arreglo
int* returnColumnas(int* arreglo){
	int tan = arreglo[0]+1;
	//int j=0;
	int* arregloy=(int*)calloc(tan+1, sizeof(int));
	//memset( arregloy, 0, tan*sizeof(int) );
	//(int*)calloc(tan, sizeof(int));
	arregloy[0]=tan;
	for(int i=1;i<=tan;i++){
		arregloy[i]= arreglo[i] %10;
	}
	return arregloy;
}	


//revisa si esta marcado, el elemento de la lista 
int EstaMarcado(int v, int VerticesMarcados[], int markedVerticesIdx) {

	int i = 0;
	for (i = 0; i < markedVerticesIdx; i++) {
		if (v == VerticesMarcados[i]) {
			return 1;
		}
	}
	return 0;
}

//encuentra el camino mas corto
int* dijkstra(int graph[V][V], int ini, int dest) {
	//variables
	int i, r, c,
		tmpC,
		min,
		ActualVertex,
		finalWt = 0,
		destValor = 0,
		ValorMarcado = 0,
		wtTablaR = 0,
		VerticesMarcadosIdx = 0;
	//vertices del arreglo del thread
	int verticesCaminoCorto[V] = {-1};
	int CaminoCortoVerticeIdx = 0;
	
	//guarda el peso
	int TablaPesos[V][V];
	for (r = 0; r < V; r++) {
		for (c = 0; c < V; c++) {
			TablaPesos[r][c] = INF;
		}
	}
	TablaPesos[wtTablaR++][ini] = 0;

	//marca los pesos
	int VerticesMarcados[V] = {-1};
	VerticesMarcados[VerticesMarcadosIdx++] = ini;
	ActualVertex = ini;
	
	//encuentra camino
	while(ActualVertex != dest) {

		//copia valores marcados al siguente linea de peso
		for (i = 0; i < VerticesMarcadosIdx; i++) {
			c = VerticesMarcados[i];
			TablaPesos[wtTablaR][c] = TablaPesos[wtTablaR - 1][c];
		}

		 //encuentra peso del peso del vertice a los demas vertices
		 //conectados y que no han sido marcados
		for (c = 0; c < V; c++) {
			if (c != ActualVertex && !EstaMarcado(c, VerticesMarcados, VerticesMarcadosIdx)) {

				finalWt = graph[ActualVertex][c];
				destValor = TablaPesos[wtTablaR - 1][c];
				ValorMarcado = TablaPesos[wtTablaR][ActualVertex];

				min = Minimo(destValor, ValorMarcado + finalWt);

				TablaPesos[wtTablaR][c] = min;
			}
		}

		//encuentra vertices mas pequeno no marcado 
		min = INF;
		for (c = 0; c < V; c++) {

			if (!EstaMarcado(c, VerticesMarcados, VerticesMarcadosIdx)) {
				if (TablaPesos[wtTablaR][c] < min) {
					min = TablaPesos[wtTablaR][c];
					tmpC = c;
				}
			}

		}

		//nuevo vertice para marcar
		VerticesMarcados[VerticesMarcadosIdx++] = tmpC;
		ActualVertex = tmpC;
		
		//variables actualizadas
		wtTablaR++;
	}

	//vertices del camino mas corto
	c = dest;
	verticesCaminoCorto[CaminoCortoVerticeIdx++] = c;
	ValorMarcado = TablaPesos[wtTablaR - 1][dest];
	for (r = wtTablaR - 2; r >= 0; r--) {

		if (TablaPesos[r][c] != ValorMarcado) {
			c = VerticesMarcados[r];
			ValorMarcado = TablaPesos[r][c];
			verticesCaminoCorto[CaminoCortoVerticeIdx++] = c;
		}

	}

	//imprime camino
	int* solucion=(int*)calloc(CaminoCortoVerticeIdx+1, sizeof(int));
	//printf("Camino mas corta %d and %d\n", ini, dest);
	solucion[0]=CaminoCortoVerticeIdx;
	int j=1;
	for (i = CaminoCortoVerticeIdx-1; i >= 0; i--) {
		//printf("%d",verticesCaminoCorto[i]);
		solucion[j]=verticesCaminoCorto[i];
		if (i > 0) {
			//printf(" --> ");
		}
		j++;
	}
	//printf("\n");
	//printf("Peso del camino: %d\n", TablaPesos[wtTablaR-1][dest]);
	
	//free(TablaPesos);
	return solucion;

}

/* Se inicializa la matriz
 */
void ini_matriz_nodos(){
	
	for(int i = 0;i < V;i++){
		for(int j = 0;j < V;j++){
			matriz_nodos[i][j] = INF;
		}		
	}
	//diagonales = 0
	for (int i = 0;i < V;i++){
		matriz_nodos[i][i] = 0;
	}
}

int **genera_ruta(int origen, int destino){
	//inicio y destino
	int **ruta = calloc(2, sizeof(int*));
	
	int*solucion = dijkstra(matriz_nodos, origen, destino);
	
	//filas
	int *x;
	x=returnFilas(solucion);
	ruta[0] = x;
	
	//columnas
	int *y;
	y=returnColumnas(solucion);
	ruta[1] = y;
	
	return ruta;
}


void cargar_rutas(){
	int cont = 0;
	int a, b;
	int cont_ambulancia = 0;
	int cont_planta = 0;
	for(int i = 0;i < 15;i++){
		a = origenes[i];
		for(int j = 0;j < 15;j++){
			b = destinos[j];
			if(a != b){//no cargar rutas repetidas
				rutas_carros[cont] = genera_ruta(a,b);
				if( b == 17 || b == 18 || b == 37 || b == 38 || b == 97 || b == 98 ){
					rutas_ambulancias[cont_ambulancia] = rutas_carros[cont];
					cont_ambulancia++;
				}
				if( b == 15 ){
					rutas_planta_nuclear[cont_planta] = rutas_carros[cont];
					cont_planta++;
				}
				
				//printf("pos = %d, x-> %d, y-> %d\n",cont,a,b);
				cont++;
			}
		}
	}
}
