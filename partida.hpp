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

		void setJugador1(Jugador jugador){_jugador1 = jugador;}
		void setJugador2(Jugador jugador){_jugador2 = jugador;}
		void setId(int id){_id = id;}
		void iniciar(){_iniciada = true;}

};

#endif