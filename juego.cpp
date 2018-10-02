#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <time.h>
#include "juego.hpp"

using namespace std;


bool Juego::FlotaDestruidaM1()
{


  return true;
}

bool Juego::FlotaDestruidaM2()
{


  }

  return true;
}

bool Juego::estatocadoM1(int fila,int columna)
{

}

bool Juego::estatocadoM2(int fila,int columna)
{



}


int Juego::esDisparadoM1(char letra,int posicion)
{

}

int Juego::esDisparadoM2(char letra,int posicion)
{


}

void Juego::InicializarTablero1()
{
	for (int i=0; i<20; i++)
	{
		tableroM1_[rand()%10][rand()%10]= '*';
	}
}

void Juego::InicializarTablero2()
{
		for (int i=0; i<20; i++)
	{
		tableroM2_[rand()%10][rand()%10]= '*';
	}

}


void Juego::TransformarMatrizM1()
{

  for (int i=0; i<10; i++){
    for (int j=0; j<10; j++){
      if (tableroM1_[i][j] == 0)
        tableroM1_[i][j] = 'A';
      else
        tableroM1_[i][j] = 'B';
    }
  }

}

void Juego::TransformarMatrizM2()
{

  for (int i=0; i<10; i++){
    for (int j=0; j<10; j++){
      if (tableroM2_[i][j] == 0)
        tableroM2_[i][j] = 'A';
      else
        tableroM2_[i][j] = 'B';
    }
  }

}

void Juego::ImprimirTableroPropio(){

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

      printf("%c ", tableroM1_[i][j]);
    }
  }
  printf("\n");
}

void Juego::liberarpartida()
{
setPlayerM1(0);
setPlayerM2(0);
setdisparosM1(0);
setdisparosM2(0);
setJugando(false);
InicializarMatrizPropiaM1();
InicializarMatrizPropiaM2();
TransformarMatrizM1();
TransformarMatrizM2();
}