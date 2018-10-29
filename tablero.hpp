#ifndef TABLERO_HPP
#define TABLERO_HPP

#include "jugador.hpp"
#include <sstream>

using namespace std;

class Tablero
{
	private:
		vector<vector <string> > _tablero;
		vector<vector <string> > _solucion;
		int _rows;
		int _cols;

	public:
		Tablero(int rows = 10, int cols = 10){
			_rows = rows; _cols = cols;
			int bombas = 0;

			_tablero.resize(_rows);

			for(int i = 0; i < _rows; i++)
				_tablero[i].resize(_cols);

			_solucion.resize(_rows);

			for(int i = 0; i < _rows; i++)
				_solucion[i].resize(_cols);

			for(int i = 0; i < _rows; i++){
				for(int j = 0; j < _cols; j++){
					_tablero[i][j] = "-";
				}
			}

			for(int i = 0; i < _rows; i++){
				for(int j = 0; j < _cols; j++){
					_solucion[i][j] = "0";
				}
			}

			
			do{
				int fila = rand()%10;
				int columna = rand()%10;
				if(_solucion[fila][columna] != "*"){
					_solucion[fila][columna] = "*";
					bombas++;
				}
			}while(bombas<10);




		}
		~Tablero(){};

		//Observadores

		string getPos(int i, int j){return _tablero[i][j];}

		int getRows(){return _rows;}

		int getCols(){return _cols;}

		//Modificadores

		void setPos(int i, int j, string c){_tablero[i][j] = c;}

		void setRows(int rows){_rows = rows;}

		void setCols(int cols){_cols = cols;}


		
};

#endif
