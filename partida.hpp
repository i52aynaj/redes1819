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
		};

		~Partida(){};

		//Observadores

		Jugador getJugador1(){return _jugador1;}
		Jugador getJugador2(){return _jugador2;}
		Tablero getTablero(){return _tablero;}

		//Modificadores
		
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
			send(p1, buffer, strlen(buffer), 0);
			send(p2, buffer, strlen(buffer), 0);
		}

};

#endif