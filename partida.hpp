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
		Partida(){
			_iniciada = false;
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

};

#endif