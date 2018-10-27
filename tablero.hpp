#ifndef TABLERO_HPP
#define TABLERO_HPP

#include "jugador.hpp"

using namespace std;

class Tablero
{
	private:
		vector<vector <string> > _tablero;
		int _rows;
		int _cols;

	public:
		Tablero(int rows = 10, int cols = 10){
			_rows = rows; _cols = cols;

			_tablero.resize(_rows);

			for(int i = 0; i < _rows; i++)
				_tablero[i].resize(_cols);

			for(int i = 0; i < _rows; i++){
				for(int j = 0; j < _cols; j++){
					_tablero[i][j] = "-";
				}
			}
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
