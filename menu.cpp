#include <stdio.h>
#include <cstdlib>
#include "FuncionesAuxiliares.cpp"


int main()
{
int opcion;
srand(time(NULL));
	do
	{
	opcion=menu();

		switch(opcion)
		{
			case 0:
			std::cout<<"Saliendo.\n";
			break;

			case 1:
			metodoburbuja();
			break;

			case 2:
			metodoordenacion();
			break;
		}
	}
	while (opcion!=0);	
	

return 0;
}