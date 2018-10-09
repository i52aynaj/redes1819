#include <cstdio>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <fstream>
#include <signal.h>
#include <unistd.h>
#include <ctime>
#include <vector>
#include <arpa/inet.h>
#include <sstream>
#include <iostream>
#include "autenticar.hpp"
#include <sstream>
#include "juego.hpp"
#include "juego.cpp"

#define MSG_SIZE 250
#define MAX_CLIENTS 30

using namespace std;


void manejador(int signum);
void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);
vector<string> leercadena(string cadena);
bool comprobar(char letra,int posicion);
void convertiracadena1(Juego juego,int p1);
void convertiracadena2(Juego juego, int p2);


int main()
{
/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
int sd, new_sd;
struct sockaddr_in sockname, from;
socklen_t from_len;
struct hostent * host;
char buffer[MSG_SIZE];
  fd_set readfds, auxfds;
  int salida,valor;
  int arrayClientes[MAX_CLIENTS];
  int numClientes = 0;
  int primero,segundo;
  //contadores
  int i,j,k,n=0,contPart=0;
 int recibidos;
  char identificador[MSG_SIZE];
  std::vector<string> opcion;
  int on, ret,cont=0;
 	int p1,p2,disparoM1=0,disparoM2=0;
  char tableros[2];
  char mierda[2];

  Autenticar autenticar [MAX_CLIENTS]; 
  Juego juego[MAX_CLIENTS/2];


  	/* --------------------------------------------------
	Se abre el socket 
---------------------------------------------------*/
	sd = socket (AF_INET, SOCK_STREAM, 0);
if (sd == -1)
{
	perror("No se puede abrir el socket cliente\n");
  		exit (1);	
}


sockname.sin_family = AF_INET;
sockname.sin_port = htons(2050);
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

/*-----------------------------------------------------------------------
El servidor acepta una petición
------------------------------------------------------------------------ */

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
while(1)
{
  
  //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
    
  auxfds = readfds;

  salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
    
  if(salida > 0)
  {
           
    for(i=0; i<FD_SETSIZE; i++)
    {
          
      //Buscamos el socket por el que se ha establecido la comunicación
      if(FD_ISSET(i, &auxfds)) 
      {
          
        if( i == sd)
        {
              
          if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1)
          {
              perror("Error aceptando peticiones");
          }
          else
          {
              if(numClientes < MAX_CLIENTS)
              {
                  arrayClientes[numClientes] = new_sd;
                  autenticar[new_sd].setSocket(new_sd);
                  numClientes++;
                  FD_SET(new_sd,&readfds);
              
                  strcpy(buffer, "Bienvenido al Buscaminas.\n");
              	send(new_sd,buffer,strlen(buffer),0);
              	std::cout<<"Usuario con socket: "<<new_sd<<" en el sistema.\n";
              	autenticar[new_sd].desconectado();
                  
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
        else if (i == 0)
        {
          //Se ha introducido información de teclado
          bzero(buffer, sizeof(buffer));
          fgets(buffer, sizeof(buffer),stdin);
          
          //Controlar si se ha introducido "SALIR", cerrando todos los sockets y finalmente saliendo del servidor. (implementar)
          if(strcmp(buffer,"SALIR\n") == 0)
          {
           
              for (j = 0; j < numClientes; j++)
              {
                  send(arrayClientes[j], "Desconexion servidor\n", strlen("Desconexion servidor\n"),0);
                  close(arrayClientes[j]);
                  FD_CLR(arrayClientes[j],&readfds);
              }
                  close(sd);
                  exit(-1);
              
              
          }
          //Mensajes que se quieran mandar a los clientes (implementar)
          
        } 
        else
        {
          bzero(buffer,sizeof(buffer));
          
          recibidos = recv(i,buffer,sizeof(buffer),0);
          
          if(recibidos > 0)
          {
                  
            if(strcmp(buffer,"SALIR\n") == 0)
            {
                autenticar[numClientes].jugando(false);
                autenticar[numClientes].jugando(false);
                salirCliente(i,&readfds,&numClientes,arrayClientes);
                
            }
            else
            {
            	if (buffer[strlen(buffer)-1]== '\n')
            		{
        				buffer[strlen(buffer)-1]= '\0';
      		  		}
          		//Convertir todo a minuscula
          		for ( k = 0; k < MSG_SIZE; k++)
          			{
          			buffer[k]=tolower(buffer[k]);
	              }

            	opcion = leercadena(buffer);

              if(opcion[0]!="usuario" and opcion[0]!="password" and opcion[0]!="registro" and opcion[0]!="iniciar-partida")
              {
                if(opcion[0]!="descubrir" and opcion[0]!="salir" and opcion[0]!="poner-bandera")
                {
                  send(i,"\e[1;91mComando desconocido.\e[0m",strlen("\e[1;91mComando desconocido.\e[0m"),0);
                }
              }
                  
             	if (opcion[0]=="usuario")
          		{
          			string us = opcion[1];
          			
          			if (autenticar[i].autenticarUsuario(opcion[1]) and not autenticar[i].getConectado() )
          			{
                  autenticar[i].setUsuario(opcion[1]); //Comprobar la autenticación del usuario el año pasado no me iba.********
          				printf("+Ok. Usuario correcto\n");
                }
                
                if (not autenticar[i].autenticarUsuario(opcion[1]))
                {
                 send(i,"Este usuario no esta registrado.\n",strlen("Este usuario no esta registrado.\n"),0);
                }
                if (autenticar[i].autenticarUsuario(opcion[1]) and autenticar[i].getConectado() and autenticar[i].getUsuario()==(opcion[1]))
                {
                  for (k=0; k<30; k++)
                  {
                    if (autenticar[k].getUsuario()==autenticar[i].getUsuario())
                      send(i,"–Err. Usuario ya conectado\n",strlen("–Err. Usuario ya conectado\n"),0);
                  }
                
                }
              
              }
          	
              if (opcion[0]=="password")
      				{
               string us=opcion[1];

  							if (autenticar[i].autenticarContrasena(opcion[1],us))
  								{
  									printf("+Ok.Usuario validado\n");
  									autenticar[i].conectado();
                    autenticar[i].jugando(false);
  								
  								}
  							else
  								{
  									printf("–Err.Error en la validación\n");
                    send(i,"Error en contraseña.\n",strlen("Error en contraseña.\n"),0);
  									
  								}
  						}

              if (opcion[0]=="registro")
              {
            		if (autenticar[i].autenticarUsuario(opcion[2]))
            		{
            			printf("Usuario con dicho nombre registrado.\n");
            			
            		}
            		else
            		{
            			autenticar[i].registrar(opcion[2],opcion[4]);
            			printf("Usuario registrado correctamente.\n");
            		}

              }
              if (opcion[0]=="iniciar-partida" )
              {
                  if (opcion[0]=="iniciar-partida" and not autenticar[i].getConectado())
                  {
                      send(i,"\e[1;91m!Err. Debes estar logueado en el sistema para iniciar una partida.\e[0m",strlen("\e[1;91m!Err. Debes estar logueado en el sistema para iniciar una partida.\e[0m"),0);
                  }
                  else if (opcion[0]=="iniciar-partida" and autenticar[i].getConectado()  and autenticar[i].getJugando()==true )
                  {
                      send(i,"\e[1;91m!Err. No puedes iniciar una partida hasta que no se acabe la actual.\e[0m",strlen("\e[1;91m!Err. No puedes iniciar una partida hasta que no se acabe la actual.\e[0m"),0);
                  }
                  else if (opcion[0]=="iniciar-partida" and autenticar[i].getConectado() and autenticar[i].getJugando()==false and contPart<16)
                  {
                    cont++;
					          for (n=0; n<10; n++)
					            { 

                    		if (cont==1)
                    		{
                    		  
                    			autenticar[i].setPartida(n);
                          autenticar[i].jugando(true);
                          autenticar[i].setSocket(i);
                    			juego[n].setPlayerM1(i);
                          p1=juego[n].getPlayerM1();
                          primero=p1;
                          autenticar[i].setTurnoTrue();
                    			bzero(buffer,sizeof(buffer));
                    			strcpy(buffer, "Su peticion esta a la espera de un jugador.\n");
                  			  send(p1,buffer,strlen(buffer),0);
                  			  break;
                  			
                    		}

                    		if (cont==2)
                    		{
                          
                    			autenticar[i].setPartida(n);
                          autenticar[i].jugando(true);
                    			juego[n].setPlayerM2(i);
                          juego[n].setJugando(true);
                          p2=juego[n].getPlayerM2();
                          segundo=p2;
                    			bzero(buffer,sizeof(buffer));
                    			strcpy(buffer, "Va a comenzar la partida.\n");
                  			  send(p2,buffer,strlen(buffer),0);
                  			  send(p1,buffer,strlen(buffer),0);

                          //Procedemos a generar los tableros y a enviarlos
                          bzero(buffer,sizeof(buffer));
                    		  
                          //tablero jugador 1 y jugador 2
                          juego[n];
                          sleep(0.5);
                          //Dibujamos tablero jugador 1
                          convertiracadena1(juego[n],p1);

                          //Dibujamos tablero jugador 2
                          convertiracadena2(juego[n],p2);

                          send(primero,"\e[2;91m __Su turno__\e[0m",strlen("\e[2;91m __Su turno__\e[0m") ,0);

                          cont=0; 
                          break;
                        }
                      		
                      }
                  }
              }

              if (opcion[0]=="descubrir" )
              {
                if (opcion[0]=="descubrir" and not autenticar[i].getConectado() )
                {
                 send(i,"Debe loguearse primero en el sistema",strlen("Debe loguearse primero en el sistema"),0); 
                }

                if ( opcion[0]=="descubrir" and not autenticar[i].getJugando())
                {
                send(i,"\e[1;91m-Err. Debes estar en una partida para usar este comando.\e[0m\n",strlen("\e[1;91m-Err. Debes estar en una partida para usar este comando.\e[0m\n"),0);
                }

                if (opcion[0]=="descubrir" and autenticar[i].getConectado() and autenticar[i].getJugando() )
                {
                  
                  if (autenticar[i].getPartida()==n)
                  {

                    if (primero==i)
                    {
                      int devuelto;
                      const char *devuelve=opcion[1].c_str();

                      if (not comprobar(devuelve[0],devuelve[2]) )
                        {
                            send(i,"\e[1;91m!Debe introducir una letra entre A y J, y un valor numerico entre 1 y 10.\e[0m",strlen("\e[1;91m!Debe introducir una letra entre A y J, y un valor numerico entre 1 y 10.\e[0m"),0);
                        }
                      else
                        {
                          if (autenticar[i].getSocket()==p1)
                            {
                              if ( devuelve[3] )
                                {
                                valor=10;
                                printf("%d\n",valor);
                                }
                                else
                                {
                                valor=(devuelve[2] - '0');
                                }
                              disparoM1++;
                              devuelto=juego[n].esDisparadoM2(devuelve[0],valor);
                              juego[n].setdisparosM1(disparoM1);     
                              send(p2,"+Ok. Disparo en :\n",strlen("+Ok. Disparo en :\n"),0);
                              send(p2,opcion[1].c_str(),strlen(opcion[1].c_str()),0);
                              send(p2,"\n",strlen("\n"),0);

                              bzero(buffer,sizeof(buffer));
                              sleep(0.5);

                                if (devuelto==2 or devuelto==1)
                                {
                                  convertiracadena2(juego[n],p2);; 
                                }
                              
                            }
                          if (autenticar[i].getSocket()==p2)
                            {
                               if ( devuelve[3])
                                {
                                valor=10;
                                }
                                else
                                {
                                valor=(devuelve[2] - '0');
                                }
                              disparoM2++;
                              devuelto=juego[n].esDisparadoM1(devuelve[0],valor);
                              juego[n].setdisparosM2(disparoM2);
                              send(p1,"+Ok. Disparo en :\n",strlen("+Ok. Disparo en :\n"),0);
                              send(p1,opcion[1].c_str(),strlen(opcion[1].c_str()),0);
                              send(p1,"\n",strlen("\n"),0);
                              bzero(buffer,sizeof(buffer));
                              sleep(0.5);

                                if (devuelto==2 or devuelto==1)
                                {
                                  convertiracadena1(juego[n],p1);
                                }
                             
                            }
                        

                          if ( devuelto==1 )
                          {
                            sleep(0.5);
                            send(i,"OK.TOCADO:\n",strlen("OK.TOCADO:\n"),0);
                            send(i,devuelve,sizeof(devuelve),0);
                            sleep(0.5);
                            send(i,"\n\e[2;91m __Su turno__\e[0m",strlen("\n\e[2;91m __Su turno__\e[0m") ,0);

                          }
                          if ( devuelto==2 )
                          {
                            sleep(0.5);
                            send(i,"Ok.HUNDIDO:\n",strlen("OK.HUNDIDO:\n"),0);
                            send(i,devuelve,sizeof(devuelve),0);
                            sleep(0.5);
                            send(i,"\n\e[2;91m __Su turno__\e[0m",strlen("\n\e[2;91m __Su turno__\e[0m") ,0);
                            if(juego[n].FlotaDestruidaM1())
                            {          

                              string envia="+Ok." + autenticar[i].getUsuario() +" ha ganado, número de disparos: ";
                              const char *enviar=envia.c_str();
                              sprintf(buffer,"%s %d\n",enviar,juego[n].getdisparosM2());  
                              send(p1,buffer, strlen(buffer), 0);
                              send(p2,buffer, strlen(buffer), 0);
                              autenticar[p1].jugando(false);
                              autenticar[p2].jugando(false);
                              juego[n].liberarpartida();
                            }
                            if(juego[n].FlotaDestruidaM2())
                            {

                              string envia="+Ok."+autenticar[i].getUsuario()+" ha ganado, número de disparos: ";
                              const char *enviar=envia.c_str();
                              sprintf(buffer,"%s %d\n",enviar,juego[n].getdisparosM1()); 
                              send(p1,buffer, strlen(buffer), 0);
                              send(p2,buffer, strlen(buffer), 0);
                              autenticar[p1].jugando(false);
                              autenticar[p2].jugando(false);
                              juego[n].liberarpartida();
                              
                            }
                            
                           
                          }
                          if (devuelto==0)
                          {
                            sleep(0.5);
                            send(i,"AGUA",strlen("AGUA"),0);
                            primero=segundo;
                            segundo=i;
                            send(primero,"\e[2;91m __Su turno__\e[0m",strlen("\e[2;91m __Su turno__\e[0m") ,0);

                          }
                    
                        }
                    }
                    else if (primero!=i)
                    {
                      send(i,"\e[1;91m-Err. No es su turno.\e[0m\n",strlen("\e[1;91m-Err. No es su turno.\e[0m\n"),0);
                    }
                  
                  }


                }


              }
                    
            } 	
                  	
          }
                                                
          //Si el cliente introdujo ctrl+c
          if(recibidos== 0)
          {
              printf("El socket %d, ha introducido ctrl+c\n", i);
              //Eliminar ese socket
              salirCliente(i,&readfds,&numClientes,arrayClientes);
          }
            
        }
      }
    }
  }
}

close(sd);
return 0;

}

void salirCliente(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]){

  char buffer[250];
  int j;
  
  close(socket);
  FD_CLR(socket,readfds);
  
  //Re-estructurar el array de clientes
  for (j = 0; j < (*numClientes) - 1; j++)
      if (arrayClientes[j] == socket)
          break;
  for (; j < (*numClientes) - 1; j++)
      (arrayClientes[j] = arrayClientes[j+1]);
  
  (*numClientes)--;
  
  bzero(buffer,sizeof(buffer));
  sprintf(buffer,"Desconexión del cliente: %d\n",socket);
  
  for(j=0; j<(*numClientes); j++)
      if(arrayClientes[j] != socket)
          send(arrayClientes[j],buffer,strlen(buffer),0);


}


void manejador (int signum)
{
  printf("\nSe ha recibido la señal sigint\n");
  signal(SIGINT,manejador);
  
  //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}


std::vector<string> leercadena(string cadena)
{
std::stringstream x; 
string segment;
std::vector<std::string> seglist;
x.str(cadena);

while (std::getline(x,segment, ' '))
{
	seglist.push_back(segment);
}

return seglist;
}
				
bool comprobar(char letra ,int posicion)
{
  if (letra=='a' or letra=='b' or letra=='c' or letra=='d' or letra=='e' or letra=='f' or letra=='g' or letra=='h' or letra=='i' or letra=='j')
  {
    if (posicion>0 or posicion<11)
      return true;

  return false;
  }

return false;  
}

void convertiracadena1(Juego juego,int p1)
{
char enviar[200];
int t=0;
int e=1;

for (int i=0; i<10; i++)
{
  for (int j=0; j<10; j++)
  {
    enviar[t]=juego.gettableroM1(i,j);
    enviar[e]=',';
    t=t+2;
    e=e+2;
  }
}
send(p1,enviar,sizeof(enviar),0);
}

void convertiracadena2(Juego juego, int p2)
{
char enviar[200];
int t=0;
int e=1;

for (int i=0; i<10; i++)
{
  for (int j=0; j<10; j++)
  {
    enviar[t]=juego.gettableroM2(i,j);
    enviar[e]=',';
    t=t+2;
    e=e+2;
  }
}
send(p2,enviar,sizeof(enviar),0);
}