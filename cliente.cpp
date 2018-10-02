#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>

void dibujartablero(char *cadena);


int main (int argc,char *argv[])
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd;
	struct sockaddr_in sockname;
	char buffer[250];
	char buffer2[250];
	socklen_t len_sockname;
    fd_set readfds, auxfds;
    int salida;
    int fin = 0;
	char dir[30];
	char *opcion;


	if (argc == 2)
	{
		strcpy(dir,argv[1]);
	}
	else
	{
		strcpy(dir,"127.0.0.1");
	}

	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}

   
    
	/* ------------------------------------------------------------------
		Se rellenan los campos de la estructura con la IP del 
		servidor y el puerto del servicio que solicitamos
	-------------------------------------------------------------------*/
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2050);
	sockname.sin_addr.s_addr =  inet_addr(dir);

	/* ------------------------------------------------------------------
		Se solicita la conexión con el servidor
	-------------------------------------------------------------------*/
	len_sockname = sizeof(sockname);
	
	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
	{
		perror ("Error de conexión");
		exit(1);
	}
    
    //Inicializamos las estructuras
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);
    
    FD_SET(0,&readfds);
    FD_SET(sd,&readfds);

    
	/* ------------------------------------------------------------------
		Se transmite la información
	-------------------------------------------------------------------*/
	do
	{
        auxfds = readfds;
        salida = select(sd+1,&auxfds,NULL,NULL,NULL);
        
        //Tengo mensaje desde el servidor
        if(FD_ISSET(sd, &auxfds)){
            
            bzero(buffer,sizeof(buffer));
            recv(sd,buffer,sizeof(buffer),0);
            
            strcpy(buffer2, buffer);
            opcion =strtok(buffer2, ",");

            if ( strcmp(opcion, "A")==0 or strcmp(opcion, "B")==0 or strcmp(opcion, "X")==0)
            {
            dibujartablero(buffer);
        	}
            else
            {
            printf("\n%s\n",buffer);
            }

            if(strcmp(buffer,"Demasiados clientes conectados\n") == 0)
                fin =1;
            
            if(strcmp(buffer,"Desconexion servidor\n") == 0)
                fin =1;
            
        }
        else
        {
            
            //He introducido información por teclado
            if(FD_ISSET(0,&auxfds)){
                bzero(buffer,sizeof(buffer));
                
                fgets(buffer,sizeof(buffer),stdin);
                


                if(strcmp(buffer,"SALIR\n") == 0)
                {
                        fin = 1;
                	
                	
                }
                
                send(sd,buffer,sizeof(buffer),0);
                
            }
            
            
        }
        
        
				
    }while(fin == 0);
		
    close(sd);

    return 0;
		
}

void dibujartablero(char *cadena)
{
int t=0;
std::cout << "\33[2J";
printf ("    A B C D E F G H I J\n   ---------------------");

  for (int i=0; i<10; i++)
  {
    for (int j=0; j<10; j++)
    {
      if (j == 0)
      {
        if (i !=9)
          printf("\n%d | ", i+1);

      	if (i == 9)
        printf("\n%d| ", i+1);
      }
     printf("%c ", cadena[t]);
    t=t+2;
    }
  	
  }    
printf("\n\n");
}
