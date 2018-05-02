#ifndef THREADSERVER_H
#define THREADSERVER_H

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdio.h>
#include <mypthreads.h>
#include "hamburguesa.h"

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
int escuchar= 0, conexion = 0, tamano;
struct sockaddr_in serv_addr;
struct sockaddr_in client_addr;
char sendBuff[1024];
char reciveBuff[1024];
int tipoHamburguesa;
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Funciones ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
int conectar(int puerto, int cantidadProcesos, char *prioridad, char *recursos, char * configuracion);



#endif
