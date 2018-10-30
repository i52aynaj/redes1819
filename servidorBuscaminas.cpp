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
#include "partida.hpp"
#include "jugador.hpp"
#include "tablero.hpp"
#include "funciones.hpp"

using namespace std;

#define MSG_SIZE 250
#define MAX_CLIENTS 50

void manejador(int signum);

int main(){

    srand(time(NULL));

	int sd, new_sd, on, ret, salida	;
	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
    char reg[MSG_SIZE], usuario[MSG_SIZE], password[MSG_SIZE];
    char param1[MSG_SIZE], param2[MSG_SIZE],prim;
    string user;
    string pass;
    char *letra;
	socklen_t from_len;
    fd_set readfds, auxfds;
    vector<Jugador> jugadores;
    vector<Partida> partidas;
    int numClientes = 0, numValidados = 0;
    int i, j, k;
    int recibidos, contador_partidas = 0;
    int primero, segundo,numero;

    jugadores.resize(30);
    partidas.resize(15);


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
    
   	
    //Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT,manejador);

    /*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
		while(1){
            //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)

            auxfds = readfds;

            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            if(salida > 0){
				for(i=0; i<FD_SETSIZE; i++){

                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)){

                        if( i == sd){

                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                            }
                            else
                            {
                                if(numClientes < MAX_CLIENTS){
                                    jugadores[numClientes].setIdentifier(new_sd);
                                    numClientes++;
                                    FD_SET(new_sd,&readfds);

                                    strcpy(buffer, "Bienvenido al Buscaminas\n");

                                    send(new_sd,buffer,strlen(buffer),0);

                                    for(j=0; j<(numClientes-1);j++){

                                        bzero(buffer,sizeof(buffer));
                                        sprintf(buffer, "Nuevo Cliente conectado: %d\n",new_sd);
                                        send(jugadores[j].getIdentifier(),buffer,strlen(buffer),0);
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
                            if(strcmp(buffer,"SALIR\n") == 0){

                                for (j = 0; j < numClientes; j++){
                                    send(jugadores[j].getIdentifier(), "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
                                    close(jugadores[j].getIdentifier());
                                    FD_CLR(jugadores[j].getIdentifier(),&readfds);
                                }
                                    close(sd);
                                    exit(-1); 
                            }
                            //Mensajes que se quieran mandar a los clientes (implementar)
                        } 
                        else{
                            bzero(buffer,sizeof(buffer));
                            recibidos = recv(i,buffer,sizeof(buffer),0);
                            if(recibidos > 0){

                            	if(strncmp(buffer,"USUARIO",7) == 0){

                                    strcpy(usuario, &buffer[8]);
                                    usuario[strlen(usuario)-1] = '\0';

                                    user = usuario;
                                    bool existe = false;

                                    for(int j = 0; j < numClientes; j++){
                                    	if(jugadores[j].getUser() == user and jugadores[j].isValidated()){
                                            existe = true;
                                    		send(i,"El usuario ya esta conectado\n", strlen("El usuario ya esta conectado\n"), 0);

                                    	}
                                    }

                                    for(int j = 0; j < numClientes; j++){
                                    	if(jugadores[j].getIdentifier() == i and !existe){
                                    		if(isRegistered(user)){
                                    			send(i, "+Ok. Usuario correcto\n", strlen("+Ok. Usuario correcto\n"), 0);
                                    		}else{
                                    			send(i, "-Err. Usuario incorrecto\n", strlen("-Err. Usuario incorrecto\n"), 0);
                                    		}
                                    	}
                                    }

                                }else if(strncmp (buffer, "PASSWORD", 8) == 0){

                                    strcpy(password, &buffer[9]);
                                    password[strlen(password)-1] = '\0';

                                    pass = password;

                                    for(int j = 0; j < numClientes; j++){
                                        if(jugadores[j].getIdentifier() == i){
                                            if(comprobarPass(user, pass)){
                                                jugadores[j].logIn(user, pass);
                                                send(i, "+Ok. Usuario validado\n", strlen("+Ok. Usuario validado\n"),0);
                                                numValidados++;
                                            }else{
                                                send(i, "-Err. Error en la validacion\n", strlen("-Err. Error en la validacion\n"),0);
                                            }
                                        }
                                    }
                                }else if(strncmp (buffer, "REGISTRO", 8) == 0){

                                    
                                    strcpy(reg, &buffer[9]);
                                    sscanf(reg,"%s %s %s %s\n", param1, usuario, param2, password);
                                    user = usuario;
                                    pass = password;

                                    if(strcmp(param1,"-u") != 0 or strcmp(param2, "-p") != 0){
                                        send(i, "Las opciones son -u y -p\n", strlen("Las opciones son -u y -p\n"), 0);
                                    }else{
                                        for(int j = 0; j < numClientes; j++){
                                            if(jugadores[j].getIdentifier() == i){
                                                if(isRegistered(user))
                                                    send(i, "El nombre de usuario ya existe\n", strlen("El nombre de usuario ya existe\n"), 0);
                                                else{
                                                    signIn(user, pass);
                                                    send(i, "Usuario registrado satisfactoriamente\n", strlen("Usuario registrado satisfactoriamente\n"), 0);
                                                }
                                            }
                                        }
                                    }

                                }else if(strcmp(buffer, "INICIAR-PARTIDA\n") == 0){                               		
                                        for(int j = 0; j < numClientes; j++){
                                            if(jugadores[j].getIdentifier() == i){
                                                if(!jugadores[j].isValidated()){
                                                    send(i,"Debe iniciar sesion para empezar una partida\n", strlen("Debe iniciar sesion para empezar una partida\n"), 0);
                                                }else if(numValidados%2 != 0){
                                                	primero = 0;
                                                	segundo = 0;
                                                    send(i, "Esperando a otro jugador para iniciar partida\n", strlen("Esperando a otro jugador para iniciar partida\n"), 0);
                                                    partidas[contador_partidas].setJugador1(jugadores[j]);
                                                    partidas[contador_partidas].setId(contador_partidas);
                                                    primero = partidas[contador_partidas].getJugador1().getIdentifier();
                                                }else{
                                                    partidas[contador_partidas].setJugador2(jugadores[j]);
                                                    segundo = partidas[contador_partidas].getJugador2().getIdentifier();
                                                    send(partidas[contador_partidas].getJugador1().getIdentifier(), "+Ok. Empieza la partida\n", strlen("+Ok. Empieza la partida\n"), 0);
                                                    send(partidas[contador_partidas].getJugador2().getIdentifier(), "+Ok. Empieza la partida\n", strlen("+Ok. Empieza la partida\n"), 0);                                               
                                                    contador_partidas++;
                                                }

                                                
                                            }
                                        }
                                    if(primero > 0 and segundo > 0)
                                        partidas[contador_partidas-1].enviarTablero(primero, segundo); 

                                    if (strncmp(buffer, "DESCUBRIR",9) == 0 )
                                    {
                                        strcpy(reg, &buffer[10]);
                                        sscanf(reg,"%s , %s \n",usuario,password); //Guardamos la letra y el numero.

                                        letra = &usuario[0u];
                                        numero = atoi(password);

                                        if (i==primero)
                                        {
                                            send(primero,"\e[2;91m __Su turno__\e[0m",strlen("\e[2;91m __Su turno__\e[0m") ,0);

                                            if (partidas[contador_partidas].getJugador1().getIdentifier()==primero)
                                            prim='A';

                                            if (partidas[contador_partidas].getJugador2().getIdentifier()==primero)
                                            prim='B';

                                            if (partidas[contador_partidas-1].getTablero().comprobarcasilla(letra,numero,prim))
                                            {
                                                //Cambiamos de turno.
                                                int turno1=0;
                                                turno1=primero
                                                primero=segundo;
                                                segundo=turno1;

                                            }
                                            else
                                            {
                                                send(primero,)
                                                send(segundo,)
                                                salirCliente(i,&readfds,&numClientes,arrayClientes);
                                            }

                                        }
                                        else
                                        {
                                            send(i,"\e[1;91m-Err. Debe esperar su turno.\e[0m\n",strlen("\e[1;91m-Err. Debe esperar su turno.\e[0m\n"),0);
                                        }

                                    }

                                } 
                                else{
                                    send(i,"\e[1;91m-Err. Comando no reconocido.\e[0m\n",strlen("\e[1;91m-Err. Comando no reconocido.\e[0m\n"),0);

                                } 
/*else if(strncmp(buffer, "PASSWORD",8) == 0 && registrado == 1){

  	//Comprobar pass
  	strcpy(pass, &buffer[9]);
  	pass[strlen(pass)-1] = '\0';

  	inicio_sesion = validarPass(usuario, pass);
  	if(inicio_sesion)
  		send(new_sd, "+Ok. Usuario validado", strlen("+Ok. Usuario validado"),0);
  	else
  		send(new_sd, "-Err. Error en la validacion", strlen("-Err. Error en la validacion"),0);
}


/*if(strcmp(buffer,"SALIR\n") == 0){

salirCliente(i,&readfds,&numClientes,arrayClientes);

}
else{

sprintf(identificador,"%d: %s",i,buffer);
bzero(buffer,sizeof(buffer));
strcpy(buffer,identificador);

for(j=0; j<numClientes; j++)
    if(arrayClientes[j].identificador != i)
        send(arrayClientes[j].identificador,buffer,strlen(buffer),0);


}*/
                            

}
//Si el cliente introdujo ctrl+c
/*if(recibidos== 0)
{
printf("El socket %d, ha introducido ctrl+c\n", i);
//Eliminar ese socket
salirCliente(i,&readfds,&numClientes,arrayClientes);
}*/
                        }
                    }
                }
            }
		}

    close(sd);
	return 0;
	
}


void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}