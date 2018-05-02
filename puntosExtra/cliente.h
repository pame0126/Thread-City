#ifndef CLIENTE_H
#define CLIENTE_H

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Includes ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <getopt.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <stdio.h>
#include <errno.h>
#include <stdio.h>
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
 int sockfd = 0;
 int lectura = 0;
 char BufferLectura[1024];
 struct sockaddr_in serv_addr;

 
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Funciones ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
int conectarCliente(char *ip, int puerto, char* configuracion);



#endif
