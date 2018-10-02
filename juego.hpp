#ifndef JUEGO_HPP_
#define JUEGO_HPP_
//Ficheros que incluye
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <cstring>
#include <time.h>

using namespace std;

class Juego{

private:

	int playerM1_;							//Para asignar jugador1.
	int playerM2_;							//Asignar jugador2
	int disparosM1_;						//Disparos de jugador1.
	int disparosM2_;						//Disparos de jugador2.
	char tableroM1_[10][10];					//Matriz que representa el tablero de jugador 1.
	char tableroM2_[10][10];					//Matriz que representa el tablero del jugador 2.	
	bool barcos_;							//Variable indicando si todos los barcos estan correctamente colocados
	bool jugando_;							//Variable que indica si la partida esta iniciada

public:
	//Observadores
	int getPlayerM1(){return playerM1_;};
	int getPlayerM2(){return playerM2_;};
	int getdisparosM1(){return disparosM1_;};
	int getdisparosM2(){return disparosM2_;};
	bool getbarcos(){return barcos_;};
	char gettableroM1(int i, int j){return tableroM1_[i][j];};
	char gettableroM2(int i,int j){return tableroM2_[i][j];};
	bool getjugando(){return jugando_;};

	//Modificadores
	void setPlayerM1(int playerM1){playerM1_=playerM1;};
	void setPlayerM2(int playerM2){playerM2_=playerM2;};
	void setdisparosM1(int disparosM1){disparosM1_=disparosM1;};
	void setdisparosM2(int disparosM2){disparosM2_=disparosM2;};
	void setBarcos(bool barcos){barcos_=barcos;};
	void setJugando(bool jugando){jugando_=jugando;};


//Constructor Juego
Juego()
{
srand(time(NULL));
barcos_=false;
disparosM1_=0;
disparosM2_=0;
jugando_=false;
InicializarMatrizPropiaM1();
InicializarMatrizPropiaM2();
TransformarMatrizM1();
TransformarMatrizM2();

}

void InicializarTablero1();
void InicializarTablero2();
void AleatorioM1(int barco);
void AleatorioM2(int barco);
bool prueba_aleatorioM1();
bool prueba_aleatorioM2();
void TransformarMatrizM1();
void TransformarMatrizM2();
bool FlotaDestruidaM1();
bool FlotaDestruidaM2();
int esDisparadoM2(char letra,int posicion);
int esDisparadoM1(char letra,int posicion);
bool estatocadoM1(int fila,int columna);
bool estatocadoM2(int fila,int columna);
void ImprimirTableroPropio();
void liberarpartida();




};
#endif 
