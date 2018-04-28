#pragma once

#include <string>
#include <vector>
#include <fstream>

#define IPFILE "DireccionesIP.txt"

typedef enum{CLIENT, SERVER} mode;

using namespace std;

typedef struct {

	string myIP; //La direccion IP de la computadora actual.
	vector<string> IPList; //La lista de IPs de todas las máquinas involucradas
	vector<unsigned int> orderList; //El orden en el cual solicitamos que se ejecute la animación entre máquinas.
	unsigned int orderNumber; //Posición de la IP de esta máquina en la lista de IPs.

	mode currentMode; //Modo actual de esta computadora
	
	bool shouldEnd; //¿Debería terminar la ejecución del programa?

}netData;


void netConfig(netData data);
static void loadIPs(vector<string>& stringVector);

