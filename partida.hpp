#ifndef PARTIDA_H
#define PARTIDA_H

#include "jugador.hpp"
#include "tablero.hpp"

using namespace std;

class Partida
{
	private:
		int _id;
		Jugador _jugador1;
		Jugador _jugador2;
		Tablero _tablero;
		bool _iniciada;

	public:
		Partida(bool iniciada = false){
			_iniciada = iniciada;
			Tablero();
		};

		~Partida(){};

		//Observadores

		Jugador getJugador1(){return _jugador1;}
		Jugador getJugador2(){return _jugador2;}
		Tablero getTablero(){return _tablero;}

		//Modificadores
		
		void libermemoria()
		{
			Partida();
		}
		void setJugador1(Jugador jugador){_jugador1 = jugador;}
		void setJugador2(Jugador jugador){_jugador2 = jugador;}
		void setId(int id){_id = id;}
		void iniciar(){_iniciada = true;}
		bool empezada(){return _iniciada;}
		void enviarTablero(int p1, int p2){
			char * csr;
			char buffer[512];
			char aux[512];

			bzero(buffer,sizeof(buffer));
			sprintf(buffer, "");


			for(int i = 0; i < _tablero.getRows(); i++)
			{
				sprintf(aux, "");
				for(int j = 0; j < _tablero.getCols(); j++){
					string pos = _tablero.getPos(i,j);
					csr = &pos[0];
					strcat(aux, csr);
					if(j < 9)
						strcat(aux, ",");
					else
						strcat(aux, ";");

				}
				strcat(buffer, aux);
			}
			sleep(0.5);
			send(p1, buffer, strlen(buffer), 0);
			send(p2, buffer, strlen(buffer), 0);
		}

bool comprobarcasilla(char letras, int numero,char usuario,int socket,Tablero tablero)
		{
			_tablero=tablero;
			int posicion=0;
			char letra;
			letra = tolower(letras);
			

			switch(letra)
			{
				case 'a':
				{
					posicion=0;
				break;
				}
				case 'b':
				{
					posicion=1;
				break;
				}
				case 'c':
				{
					posicion=2;
				break;
				}
				case 'd':
				{
					posicion=3;
				break;
				}
				case 'e':
				{
					posicion=4;
				break;
				}
				case 'f':
				{
					posicion=5;
				break;
				}
				case 'g':
				{
					posicion=6;
				break;
				}
				case 'h':
				{
					posicion=7;
				break;
				}
				case 'i':
				{
					posicion=8;
				break;
				}
				case 'j':
				{
					posicion=9;
				break;
				}
			}

			if (_tablero.getSol(numero-1,posicion)=="*")
			{
				return false;
			}

_tablero.setPos(numero-1,posicion,usuario,socket);
return true;
}

};

#endif