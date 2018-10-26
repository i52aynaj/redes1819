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
		bool _conectado;
		bool _validado;

	public:

		//Constructor

		Jugador(bool conectado = false, bool validado = false){_conectado = conectado; _validado = validado;}

		//Observadores
		int getIdentifier(){return _identificador;}

		string getUser(){return _user;}

		string getPassword(){return _password;}

		bool isConnected(){return _conectado;}

		bool isValidated(){return _validado;}

		//Modificadores
		void setIdentifier(int identificador){_identificador = identificador;}

		void setUser(string user){_user = user;}

		void setPassword(string password){_password = password;}

		void validate(){_validado = true;}

		void connect(){_conectado = true;}



		bool validateUser(string usuario){

			ifstream myfile;
			string name;
			myfile.open("usuarios.txt");

			if(myfile.is_open()){
				while(getline(myfile, name, ' ')){
					if(usuario == name){
						myfile.close();
						setUser(usuario);
						validate();
						return true;
					}
					getline(myfile, name,'\n');
				}
			}

			myfile.close();
			return false;

		}

		void logIn(string password){

			ifstream myfile;
			string pass;
			myfile.open("usuarios.txt");

			if(myfile.is_open()){
				while(getline(myfile, pass, ' ')){
					getline(myfile, pass,'\n');
					if(password == pass){
						setPassword(password);
						connect();
						myfile.close();
						return;
					}
				}
			}

			myfile.close();
			return;
		}

		void signIn(string usuario, string password){
			fstream myfile;
			string name, pass;
			myfile.open("usuarios.txt");

			if(myfile.is_open()){
				while(getline(myfile, name, ' ')){
					if(usuario == name){
						cout << "El nombre de usuario ya existe." << endl;
						myfile.close();
						return;
					}
					getline(myfile, name, '\n');
				}
				myfile << usuario << ' ' << password << endl;
			}

			myfile.close();

		}
};

#endif