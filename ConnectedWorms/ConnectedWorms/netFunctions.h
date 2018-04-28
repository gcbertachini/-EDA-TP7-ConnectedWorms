#pragma once

#include <string>
#include <vector>
#include <fstream>

#define IPFILE "DireccionesIP.txt"

typedef enum{CLIENT, SERVER} mode;

using namespace std;

typedef struct {

	string myIP; //La direccion IP de la computadora actual.
	vector<string> IPList; //La lista de IPs de todas las m�quinas involucradas
	vector<unsigned int> orderList; //El orden en el cual solicitamos que se ejecute la animaci�n entre m�quinas.
	unsigned int orderNumber; //Posici�n de la IP de esta m�quina en la lista de IPs.

	mode currentMode; //Modo actual de esta computadora
	
	bool shouldEnd; //�Deber�a terminar la ejecuci�n del programa?

}netData;


void netConfig(netData data);
static void loadIPs(vector<string>& stringVector);

