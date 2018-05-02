#include "cliente.h"

int conectarCliente(char *ip, int puerto, char* configuracion)
{
    memset(BufferLectura, '0', sizeof(BufferLectura));//llena de ceros el buffer
    //inicia el socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : No pudo crear el sockett \n");
        return 1;
    }
    //guarda ip y direcciones
    memset(&serv_addr, '0', sizeof(serv_addr));//llena de ceros la direccion del server
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(puerto);
    // revisar si se puede conectar
    if (inet_pton(AF_INET, ip, &serv_addr.sin_addr) <= 0)
    {
        printf("\n ocurrio error con el inet_pton \n");
        return 1;
    }
    //conexion al servidor
    if ( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\n Error : Conexión fallida \n");
        return 1;
    }
    // hace el pedido de la orden 
    if( write(sockfd , configuracion , strlen(configuracion)) < 0) {
        printf("Fallo el envio, intente nuevamente\n");
        return 1;
    }

    printf("\nSolicitud realizada, POR FAVOR ESPERE SU ORDEN!\n");
    //lee mensaje del server
    if ( (lectura = read(sockfd, BufferLectura, sizeof(BufferLectura) - 1)) > 0)
    {
		//revisa si no esta vacio
        BufferLectura[lectura] = 0;
        if (fputs(BufferLectura, stdout) == EOF)
        {
            printf("\n Fallo de impresion \n");
        }
    }

    if (lectura < 0)
    {
        printf("\n Fallo al recibir el mensaje \n");
    }

    return 0;
}

int main(int argc, char *argv[])
{
	//variables para las banderas
    int argumento, bandera = 1;
    char *ip = NULL, *puerto = NULL, *configuracion = NULL;

    while ((argumento = getopt(argc, argv, "h:p:c:H")) != -1)
    {
        switch (argumento)
        {
        case 'h':
            ip = (char *)optarg;
            break;
        case 'p':
            puerto = (char *)optarg;
            break;
        case 'c':
            configuracion = (char *)optarg;
            break;
        case 'H':
            bandera = 0;
            printf("\nUso del programa client\n");
            printf("client -h <ip-server> -p <server-port> -c <configuracion-orden>\n\n");
             printf("La configuración es para cajero rapido y cajero lenton\n\n");
            break;
        }
    }

    if (ip != NULL && puerto != NULL && configuracion != NULL)
    {
		//conectar el cliente
        conectarCliente(ip, atoi(puerto), configuracion);
    }
    else 
    {
        if (bandera)
        {
            printf("\nUse la opcion -H para ver la ayuda de este programa\n\n");
        }
    }

    return 0;
}
