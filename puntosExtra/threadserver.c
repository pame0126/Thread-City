#include "threadserver.h"
#include "hamburguesa.h"

void guardarPedido(int elemento, char *recurso){
	//funcion que guarda el pedido del usuario
	FILE *fp;
	fp = fopen(recurso, "arw+");
	if(elemento==0){
		//rapido
	   fputs("0\n",fp);
	}else if(elemento==1){
		//lento
		fputs("1\n",fp);
	}else if(elemento ==2){
		//termina rapido
		fputs("2\n",fp);
	}else{
		//termina lento
		fputs("3\n",fp);
		}
   fclose(fp);
}
int conectarServer(int puerto, int cantidadProcesos,char* prioridad,char *recursos, char * configuracion)
{
    //crea socket
	escuchar = socket(AF_INET, SOCK_STREAM, 0);
	//printf("asa");
	printf("Socket creado con exito\n");
	//printf("asa");
	memset(&serv_addr, '0', sizeof(serv_addr));
	memset(sendBuff, '0', sizeof(sendBuff));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(puerto);
    //bindea el socket
    //printf("asa");
	bind(escuchar, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    //cantidad de conexiones que va ha escuchar
	listen(escuchar, cantidadProcesos);
	//printf("asa");
    tamano = sizeof(struct sockaddr_in);
    //largo de la lista de threads
    mythread_t threads[cantidadProcesos];
	
	int i=0;
	char *status;
	mythread_chsched(0);
	mythread_init();
	
	int ordenes=0;
    //ciclo que va ha estar esperando conexiones para hacer las ordenes de hamburguesas
    while ((conexion = accept(escuchar, (struct sockaddr*)&client_addr, (socklen_t*)&tamano)))
	{
        if (read(conexion, reciveBuff, sizeof(reciveBuff) > 0))
        {
			//cantidad de threads corriendo
			//ordenes= threadpoolTrabajando(thpool);
			if (ordenes < cantidadProcesos)
			{
			printf("Se ha conectado un nuevo cliente \n");
            strcpy(sendBuff, "\nSolicitud en cola, por favor espere!\n");
		    write(conexion, sendBuff, strlen(sendBuff));
            if ((!(strcmp(reciveBuff, "alta")))||(!(strcmp(reciveBuff, "a"))))
            {
				//cajero rapido
                tipoHamburguesa = 0;
                int x = 0;
                int final=cantidadProcesos/2-1;
                 for(; x < final;x++){
					 //lento
					 //guardarPedido(tipoHamburguesa,recursos);
					 guardarPedido(1,recursos);
					 mythread_create(&threads[x], NULL, armar_hamburgesa2, NULL, 0);
					 //guardarPedido(2,recursos);
					 guardarPedido(3,recursos);
				}
				 for(int p=final; x < cantidadProcesos;x++){
					 //rapido
					 guardarPedido(tipoHamburguesa,recursos);
					 guardarPedido(1,recursos);
					 mythread_create(&threads[x], NULL, armar_hamburgesa1, NULL, 0);
					 //guardarPedido(3,recursos);
					 guardarPedido(2,recursos);
					 p++;
				}
				 for (i = 0; i < x; i++) {
					  //guardarPedido(2,recursos);
					  mythread_join(threads[i], (void **)&status);
				}
               i++;
            }
            else
            {
				//cajero lento
				tipoHamburguesa = 1; 
				int x = 0;
                int final=cantidadProcesos/2+2;
                 for(; x < final;x++){
					 //lento
					 //guardarPedido(tipoHamburguesa,recursos);
					 guardarPedido(1,recursos);
					 mythread_create(&threads[x], NULL, armar_hamburgesa2, NULL, 0);
					 //guardarPedido(2,recursos);
					 guardarPedido(3,recursos);
				}
				 for(int p=final; x < cantidadProcesos;x++){
					 //rapido
					 guardarPedido(tipoHamburguesa,recursos);
					 guardarPedido(1,recursos);
					 mythread_create(&threads[x], NULL, armar_hamburgesa1, NULL, 0);
					 //guardarPedido(3,recursos);
					 guardarPedido(2,recursos);
					 p++;
				}
				 for (i = 0; i < x; i++) {
					  //guardarPedido(2,recursos);
					  mythread_join(threads[i], (void **)&status);
				}
               i++;
            }
		}else{
				strcpy(sendBuff, "Servidor lleno!\n");
				write(conexion, sendBuff, strlen(sendBuff));
			}
        }
        mythread_yield();
		sleep(1);
	}



   // threadpooldestruir(thpool);
   mythread_end(NULL);
	return 0;
}
int main(int argc, char *argv[])
{
	// parametros que se dan por terminal
	int argumento, cantidadProcesos = 0, puerto = 0, bandera = 1;
	char *recursos = NULL;
	char *configuracion = NULL;
	char *prioridad = NULL;

	while ((argumento = getopt(argc, argv, "n:P:p:r:c:h")) != -1)
	{
		switch (argumento)
		{
		case 'n':
			cantidadProcesos = atoi(optarg);
			break;
		case 'P':
			prioridad = (char *)optarg;
			break;
		case 'p':
			puerto = atoi(optarg);
			break;
		case 'r':
			recursos = (char *)optarg;
			break;
		case 'c':
			configuracion = (char *)optarg;
			break;
		case 'h':
            bandera = 0;
            printf("\nComo usar el prethread-Server\n");
            printf("prethread-Server -n <cantidad-hilos> -P <prioridad> -r <recursos> -p <port> -c <configuracion>\n\n");
            printf("La configuración hace que sea en cajero rapido o lento\n\n");
            printf("La configuración alta, seria cajero rapido y la configuración baja, cajero lento\n\n");
            break;
		}
	}
	
    if (puerto != 0 && cantidadProcesos != 0 && prioridad != NULL && recursos != NULL && configuracion != NULL)
    {
		//crear un archivo para guardas los pedidos de los clientes
		FILE *fp;
		strcat(recursos,"conexiones.txt");
		fp = fopen(recursos, "w+");
		fputs("",fp);
		fclose(fp);
		//crear la conexion con el server
        conectarServer(puerto, cantidadProcesos, prioridad, recursos, configuracion);
    }
    else 
    {
        if (bandera)
        {
            printf("\nEscriba  -h para ver la ayuda de este programa\n\n");
        }
    }

	return 0;
}
