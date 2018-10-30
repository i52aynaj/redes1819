#ifndef TABLERO_HPP
#define TABLERO_HPP

#include "jugador.hpp"
#include <sstream>
#include <cctype>

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

			_tablero.resize(_rows);

			for(int i = 0; i < _rows; i++)
				_tablero[i].resize(_cols);

			_solucion.resize(_rows);

			for(int i = 0; i < _rows; i++)
				_solucion[i].resize(_cols);

			for(int i = 0; i < 10; i++){
				for(int j = 0; j < 10; j++){
					_tablero[i][j] = "-";
				}
			}

			/*for(int i = 0; i < 10; i++){
				cout << i << " ";
				for(int j = 0; j < 10; j++){
					cout << _tablero[i][j] << " ";
				}
				cout << endl;
			}*/


			for(int i = 0; i < _rows; i++){
				for(int j = 0; j < _cols; j++){
					_solucion[i][j] = "0";
				}
			}

			
			for (int mina=0;mina < 10; mina++){
            //Busca una posición aleatoria donde no haya otra bomba
            int f,c;
            do{
                f=rand()%10;
                c=rand()%10;
            }while(_solucion[f][c] == "*");
            //Pone la bomba
            _solucion[f][c]="*";
            //Recorre el contorno de la bomba e incrementa los contadores
            for (int f2=max(0, f-1);f2 < min(_rows,f+2);f2++){
                for (int c2=max(0,c-1);c2 < min(_rows,c+2);c2++){
                    if (_solucion[f2][c2]!= "*"){ //Si no es bomba
                    	string pos= getSol(f2, c2);
                    	char * csr = &pos[0u];
                    	int aux = atoi(csr) + 1;
                        _solucion[f2][c2] = to_string(aux);  //Incrementa el contador
                    }
                }
            }
        }


		}
		~Tablero(){};

		//Observadores

		string getPos(int i, int j){return _tablero[i][j];}
		string getSol(int i, int j){return _solucion[i][j];}

		int getRows(){return _rows;}

		int getCols(){return _cols;}

		//Modificadores

		void setPos(int i, int j, char c){_tablero[i][j] = c;}

		void setRows(int rows){_rows = rows;}

		void setCols(int cols){_cols = cols;}

		bool comprobarcasilla(char *letras, int numero)
		{
			int posicion,i=0;
			char letra;
			while (letras[i])
			{
				letra = letras[i];
				putchar (tolower(letra));
				i++;
			}
			

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

			


		}


		
};

#endif
