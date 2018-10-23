#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>


#define MSG_SIZE 250
#define MAX_CLIENTS 50


/*
 * El servidor ofrece el servicio de un chat
 */

void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);
int validarUsuario(char * usuario);
int validarPass(char * usuario, char * pass);
int registrarUsuario(char * usuario,char * pass);



int main ( )
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
	socklen_t from_len;
    fd_set readfds, auxfds;
    int salida;
    int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
    //contadores
    int i,j,k;
	int recibidos;
    char identificador[MSG_SIZE];
    char usuario[MSG_SIZE], pass[MSG_SIZE], reg[MSG_SIZE];
    int registrado = 0;
    
    int on, ret;

    
    
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}
    
    // Activaremos una propiedad del socket que permitir· que otros
    // sockets puedan reutilizar cualquier puerto al que nos enlacemos.
    // Esto permitir· en protocolos como el TCP, poder ejecutar un
    // mismo programa varias veces seguidas y enlazarlo siempre al
    // mismo puerto. De lo contrario habrÌa que esperar a que el puerto
    // quedase disponible (TIME_WAIT en el caso de TCP)
    on=1;
    ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));



	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		perror("Error en la operación bind");
		exit(1);
	}
	

   	/*---------------------------------------------------------------------
		Del las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/
		from_len = sizeof (from);


		if(listen(sd,1) == -1){
			perror("Error en la operación de listen");
			exit(1);
		}
    
    //Inicializar los conjuntos fd_set
    FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd,&readfds);
    FD_SET(0,&readfds);
    
		while(1){
            
            //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
            
            auxfds = readfds;
            
            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            if(salida > 0){
                
                
                for(i=0; i<FD_SETSIZE; i++){
                    
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)) {
                        
                        if( i == sd){
                            
                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                            }
                            else
                            {
                                if(numClientes < MAX_CLIENTS){
                                    arrayClientes[numClientes] = new_sd;
                                    numClientes++;
                                    FD_SET(new_sd,&readfds);
                                
                                    strcpy(buffer, "Ok, Usuario conectado\n");
                                
                                    send(new_sd,buffer,strlen(buffer),0);
                                
                                    for(j=0; j<(numClientes-1);j++){
                                    
                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "Nuevo Cliente conectado: %d\n",new_sd);
                                        send(arrayClientes[j],buffer,strlen(buffer),0);
                                    }
                                }
                                else
                                {
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,"Demasiados clientes conectados\n");
                                    send(new_sd,buffer,strlen(buffer),0);
                                    close(new_sd);
                                }
                                
                            }
                            
                            
                        }
                        else if (i == 0){
                            //Se ha introducido información de teclado
                            bzero(buffer, sizeof(buffer));
                            fgets(buffer, sizeof(buffer),stdin);
                            
                            //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
                           
                            //Mensajes que se quieran mandar a los clientes (implementar)
                            
                        } 
                        else{
                            bzero(buffer,sizeof(buffer));
                            
                            recibidos = recv(i,buffer,sizeof(buffer),0);
                            
                            if(recibidos > 0){

		                            	 if(strncmp(buffer,"USUARIO",7) == 0){

		                                    strcpy(usuario, &buffer[8]);
		                                    usuario[strlen(usuario)-1] = '\0';

		                                    if(validarUsuario(usuario) == 1){
		                                    	send(new_sd,"+Ok. Usuario correcto\n", strlen("+Ok. Usuario correcto\n"),0);
		                                    	registrado = 1;
		                                    }
		                                    else{
		                                    	send(new_sd,"-Err. Usuario incorrecto\n", strlen("-Err. Usuario incorrecto\n"),0);
		                                    }

		                                 }
		                             
		                             	  if(strncmp(buffer, "PASSWORD",8) == 0 && registrado == 1){

		                             	  	//Comprobar pass
		                             	  	strcpy(pass, &buffer[9]);
		                             	  	pass[strlen(pass)-1] = '\0';

		                             	  	if(validarPass(usuario, pass) == 1)
		                             	  		send(new_sd, "+Ok. Usuario validado", strlen("+Ok. Usuario validado"),0);
		                             	  	else
		                             	  		send(new_sd, "-Err. Error en la validacion", strlen("-Err. Error en la validacion"),0);
		                             	  }


		                             	  if(strncmp(buffer, "REGISTRO",8) == 0){

		                             	  	//Comprobar pass
		                             	  	strcpy(reg, &buffer[9]);
		                             	  	if(strstr(reg, "-u") != NULL || strstr(reg,"-p") != NULL){//Las opciones están bien

		                             	  		sscanf(reg,"-u %s -p %s", usuario, pass);

		                             	  		if(validarUsuario(usuario) == 1)
		                             	  			send(new_sd,"El nombre de usuario ya se encuentra registrado\n", strlen("El nombre de usuario ya se encuentra registrado\n"), 0);
		                             	  		else{
		                             	  			if(registrarUsuario(usuario,pass) > 0)
		                             	  				send(new_sd, "+Ok. Registro completado", strlen("+Ok. Registro completado"),0);

		                             	  		}


		                             	  	}

		                             	  }

		                                /*for (j = 0; j < numClientes; j++){
		                                    send(arrayClientes[j], "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
		                                    close(arrayClientes[j]);
		                                    FD_CLR(arrayClientes[j],&readfds);
		                                }
		                                    close(sd);
		                                    exit(-1);*/

		                                //printf("El usuario ha entrado", buffer);
		                                
		                                
                                
                                if(strcmp(buffer,"SALIR\n") == 0){
                                    
                                    //salirCliente(i,&readfds,&numClientes,arrayClientes);
                                    
                                }
                                else{
                                    
                                    sprintf(identificador,"%d: %s",i,buffer);
                                    bzero(buffer,sizeof(buffer));
                                    strcpy(buffer,identificador);
                                    
                                    for(j=0; j<numClientes; j++)
                                        if(arrayClientes[j] != i)
                                            send(arrayClientes[j],buffer,strlen(buffer),0);

                                    
                                }
                                                                
                                
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos== 0)
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                //salirCliente(i,&readfds,&numClientes,arrayClientes);
                            }
                        }
                    }
                }
            }
		}

	close(sd);
	return 0;
	
}

int validarUsuario(char * usuario){
	FILE * f;
	char * line = NULL;
	size_t len = 0;



	f = fopen("usuarios.txt", "r");

	while((getdelim(&line, &len,' ', f) != -1) && (getdelim(&line, &len,'\n', f) != -1)) {
		line[strlen(line)-1] = '\0';
		if(strcmp(line, usuario) == 0){
			return 1;
		}

	}

	fclose(f);
	return 0;
}

int validarPass(char * usuario, char * pass){
	FILE * f;
	char * line = NULL;
	char * line2 = NULL;
	size_t len = 0;



	f = fopen("usuarios.txt", "r");

	while((getdelim(&line, &len,' ', f) != -1) && (getdelim(&line2, &len,'\n', f) != -1)) {
		line[strlen(line)-1] = '\0';
		line2[strlen(line2)-1] = '\0';
		if(strcmp(line, usuario) == 0 && strcmp(line2, pass) == 0){
			return 1;
		}

	}

	fclose(f);
	return 0;
}

int registrarUsuario(char * usuario, char * pass){
	FILE * f;
	int exito;

	f = fopen("usuarios.txt", "a");

	exito = fprintf(f, "\n%s %s\n",usuario, pass);

	fclose(f);

	return exito;
}

