#include "netData.h"



void netData::loadIPs() {

	std::ifstream dirFile;
	dirFile.open(IPFILE); //Abrimos el archivo .txt con las direcciones IP...

	std::string auxString; //...Y creamos un string auxiliar para poder trabajarlas.

	while (dirFile.good()) { // ¿Aún hay direcciones que agarrar?
		getline(dirFile, auxString); //Si las hay, la ponemos en el string auxiliar...
		this->IPList.push_back(auxString); //... Y la ponemos al final del vector de strings.
	}

}

//-----------------SETTERS-------------------//
void netData::setOwnIP(unsigned int i) {
	this->myIP = this->IPList.at(i);
}

void netData::setIfShouldEnd(bool shouldI) {
	
	this->end = shouldI;
}

void netData::setCurrentMode(mode newMode) {
	this->currentMode = newMode;
}

//----------------GETTERS-------------------//
std::string netData::getOwnIP() {
	return this->myIP;
}

const char* netData::getOptions(unsigned int i){ //Devuelve una opción en una posición dada del  arreglo de opciones.
	return this->options[i];
}

const char * netData::getParams(unsigned int i) {
	return this->params[i];
}

unsigned int netData::getIPListSize() { //Devuelve el tamaño del vector.
	return this->IPList.size();
}

const char * netData::getIPAt(int i) { //Devuelve la IP en una posición dada del vector.
	return this->IPList.at(i).c_str();
}

bool netData::getIfShouldEnd() {
	return this->end;
}

mode netData::getCurrentMode() {
	return this->currentMode;
}

