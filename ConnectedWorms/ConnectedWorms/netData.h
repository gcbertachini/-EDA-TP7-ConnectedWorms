#pragma once

#include <string>
#include <vector>
#include <fstream>

#define OPTION_QTY 1
#define PARAM_QTY 2

typedef enum { CLIENT, SERVER } mode;
#define IPFILE "DireccionesIP.txt"

#define NOIP "No IP"
#define IP "ip"

#define LISTEN "escuchar"
#define START "iniciar"

class netData {
public:
		netData() {

			loadIPs();

			options[0] = IP;

			params[0] = LISTEN;
			params[1] = START;

			this->end = false;
			myIP = NOIP;

			currentMode = CLIENT;
		}

		//------Setters------//
		void setOwnIP(unsigned int i);
		void setIfShouldEnd(bool shouldI);
		void setCurrentMode(mode newMode);


		//------Getters------//

		std::string getOwnIP();
		const char * getOptions(unsigned int i); //Devuelve una opción en una posición dada del  arreglo de opciones.
		const char * getParams(unsigned int i); //Devuelve una opción en una posición dada del  arreglo de parametros.
		unsigned int getIPListSize(); //Devuelve el tamaño del vector.
		const char * getIPAt(int i); //Devuelve la IP en una posición dada del vector.
		bool getIfShouldEnd(); //Devuelve si el loop del main debe finalizar
		mode getCurrentMode(); //Devuelve el modo actual de la maquina (Cliente o server)

private:
		std::string myIP; //La direccion IP de la computadora actual.
		std::vector<std::string> IPList; //La lista de IPs de todas las máquinas involucradas
		std::vector<unsigned int> orderList; //El orden en el cual solicitamos que se ejecute la animación entre máquinas.
		unsigned int orderNumber; //Posición de la IP de esta máquina en la lista de IPs.

		const char * options[OPTION_QTY];
		const char * params[PARAM_QTY];

		mode currentMode; //Modo actual de esta computadora
		bool end; //¿Debería terminar la ejecución del programa?

		void loadIPs();

};