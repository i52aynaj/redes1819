#ifndef FUNCIONES_HPP
#define FUNCIONES_HPP

#include <cstdio>
#include <cstdlib>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

bool isRegistered(string usuario){
	ifstream myfile;
	string name;
	myfile.open("usuarios.txt");

	if(myfile.is_open()){
		while(getline(myfile, name, ' ')){
			if(usuario == name){
				myfile.close();
				return true;
			}
			getline(myfile, name,'\n');
		}
	}

	myfile.close();
	return false;
}

bool comprobarPass(string usuario, string password){
	ifstream myfile;
	string name, pass;
	myfile.open("usuarios.txt");

	if(myfile.is_open()){
		while(getline(myfile, name, ' ')){
			getline(myfile, pass,'\n');
			if(usuario == name and pass == password){
					myfile.close();
					return true;
			}

		}
	}

	myfile.close();
	return false;
}

void signIn(string usuario, string password){
	ofstream myfile;
	myfile.open("usuarios.txt", ios::app);

	if(isRegistered(usuario)){
		cout << "El nombre de usuario ya existe." << endl;
	}else{
		if(myfile.is_open()){
			myfile << usuario << ' ' << password << endl;
		}
	}

	myfile.close();

}

#endif