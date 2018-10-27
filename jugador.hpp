#ifndef JUGADOR_HPP
#define JUGADOR_HPP

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

class Jugador{

	private:
		int _identificador;
		string _user;
		string _password;
		bool _validado;

	public:

		//Constructor

		Jugador(bool validado = false){_validado = validado;}

		//Observadores
		int getIdentifier(){return _identificador;}

		string getUser(){return _user;}

		string getPassword(){return _password;}

		bool isValidated(){return _validado;}

		//Modificadores
		void setIdentifier(int identificador){_identificador = identificador;}

		void setUser(string user){_user = user;}

		void setPassword(string password){_password = password;}

		void validate(){_validado = true;}



		void logIn(string usuario, string password){

			
						setUser(usuario);
						setPassword(password);
						validate();
		}


};

#endif