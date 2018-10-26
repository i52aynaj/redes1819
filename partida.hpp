#ifndef PARTIDA_H
#define PARTIDA_H

#include "jugador.hpp"
#include "tablero.hpp"

using namespace std;

class Partida
{
	private:
		Jugador _jugador1;
		Jugador _jugador2;
		Tablero _tablero;

	public:
		Partida();
		~Partida();

};

#endif