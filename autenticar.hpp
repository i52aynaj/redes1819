#ifndef AUTENTICAR_HPP_
#define AUTENTICAR_HPP_
//Ficheros que incluye
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstring>

using namespace std;

class Autenticar{

private:

	string usuario_;		//Nombre de usuario del jugador.
	string contrasena_;		//Contraseña del usuario del jugador.
	int puntuacion_;		//Puntuacion del usuario
	bool conectado_;		//Estado que indica si el jugador esta logueado en el sistema o no.
	bool jugando_;			//Estado que indica si el jugador esta en una partida o no.
	bool esperando_;		//Estado del jugador cuando selecciona partida_grupo (controla que no pueda usar los otros comandos)
	bool turno_;			//Necesario para la partida multijugador. True->Es el turno del jugador. False-> NO
	int partida_;			//Partida en la que esta el usuario.
	int socket_;			//Necesario para comprobar errores al asigar el socket al usuario.


public:
	//Observadores

	string getUsuario(){return usuario_;};
	string getContrasena(){return contrasena_;};
	bool getConectado(){return conectado_;};
	bool getJugando(){return jugando_;};
	bool getTurno(){return turno_;};
	int getPuntuacion(){return puntuacion_;};
	int getPartida(){return partida_;};
	int getSocket(){return socket_;};

	//Modificadores

	void setSocket(int socket){socket_=socket;};
	void setPartida(int num){partida_=num;};
	void setUsuario(string usuario){usuario_=usuario;};
	void setContrasena(string contrasena){contrasena_=contrasena;};
	void conectado(){conectado_=true;};
	void desconectado(){conectado_=false;};
	void setTurnoTrue(){turno_=true;};
	void setTurnoFalse(){turno_=false;};
	void jugando(bool jugando){jugando_=jugando;};
	void esperando(){jugando_=false;};
	void setPuntuacion(int num){puntuacion_=puntuacion_+num;};
	void resetearPuntuacion(){puntuacion_=0;};
	void restarPuntuacion(int num){puntuacion_=puntuacion_-num;};
	void vaciarUsuario(){usuario_="";};


	void registrar(string usuario, string contrasena) //Escribe nuevos usuarios en el fichero datos.txt
	{					

		ofstream f;
		f.open ("datos.txt", ios::app);

		if(f.is_open() == true){

			f<<usuario+";";
			f<<contrasena+";";
			f<<"0"<<endl;

			f.close();
		}


	}

					

	bool autenticarUsuario(string usuario) //Comprueba si existe el usuario en datos.txt
	{					

		string nick;
		string password;
		int puntuacion;
		string s;
		int flag = 0;
		ifstream f;
		f.open("datos.txt", ios::in);

		if(f.is_open() == true){

		
			while(getline(f,s,';')){
				nick=s;

				getline(f,s,';');
				password=s;
				
				getline(f,s,'\n');
				puntuacion=atoi(s.c_str());

				if(nick == usuario){

					this->setUsuario(usuario);


					flag = 1;
					return true;
				}

			}

		f.close();

		if (flag == 0)
			return false;
		

		
		}			

}


bool autenticarContrasena(string contrasena, string usuario) //Comprueba si existe el usuario en datos.txt

{					

		string nick;
		string password;
		int puntuacion;
		string s;
		int flag = 0;
		ifstream f;
		f.open("datos.txt", ios::in);

		if(f.is_open() == true){

		
			while(getline(f,s,';')){
				nick=s;

				getline(f,s,';');
				password=s;
				
				getline(f,s,'\n');
				puntuacion=atoi(s.c_str());

				if(contrasena == password && nick == usuario){

					this->setUsuario(usuario);
					this->setContrasena(contrasena);
					this->setPuntuacion(puntuacion);
					this->conectado();
					this->esperando();

					flag = 1;
					return true;
				}

			}

		f.close();

		if (flag == 0)
			return false;
		

		
		}			

}

	//Constructor
	Autenticar(string usuario = "", int socket = -1, string contrasena = "", int puntuacion = 0, int intentos = 0, int partida = 0, bool conectado = false, bool jugando = false, bool turno = false, bool multijugador = false, bool esperando = false){

		usuario_=usuario;
		socket_=socket;
		contrasena_=contrasena;
		puntuacion_=puntuacion;
		partida_=partida;
		conectado_=conectado;
		jugando_=jugando;
		turno_=turno;
		esperando_=esperando;


	}
	~Autenticar(){}

};


#endif