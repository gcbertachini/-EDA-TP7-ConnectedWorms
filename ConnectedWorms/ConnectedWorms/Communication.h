#pragma once
#include <vector>
#include <iostream>
#include "General.h"
#include "netData.h"
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/timer/timer.hpp>
#include "Drawer.h"

#define ANIMATION_ID 0					//posicion en el mensaje YOU_GO donde ira a parar la animacion.
#define COUNT_ID 1						//posicion en el mensaje YOU_GO donde ira a parar el COUNT
#define MACHINES_ID (COUNT_ID+1)			//posicion en el mensaje YOU_GO de la maquina 1 (de la primera maquina). Es la prox a COUNT_ID

class Communication
{
public:
	Communication(netData * net);
	~Communication();

	void initCom(Drawer & draw);
	void listenAndRespond(Drawer & draw);
	void endCom();

private:

	//parametros de boost para el manejo de IO por internet.
	boost::asio::io_service* IO_handler;
	boost::asio::ip::tcp::socket* socket;
	boost::asio::ip::tcp::resolver* resolver;
	boost::asio::ip::tcp::resolver::iterator endpoint;
	boost::asio::ip::tcp::acceptor* acceptor;


	char YOU_GO[MAX_N_MAQUINAS + 2];		//todas las maquinas mas 1 byte para count y 1 byte para animacion. Paquete de datos para mandar entre máquinas
	bool shouldInit;						//indica si el comunicador tiene que actuar como server o no.
	unsigned int numberOfActiveMachines;	//numeros de maquinas en el .txt de direcciones. 
	animations animation;					// id para la animacion qeu el usuario debera correr.
	char myMachineSequenceNumber;			

	netData * net;							//información del manejo de red.

	void askForInitSequence();
	void startConnectionForServer(const char * host);
	void startConnectionForClient();
	void sendMessage();
	void receiveMessage();
	void parseMessage();


	void renewServerItems();
	void renewClientItems();

	void setShouldInitialize(bool should);		
	bool shouldInitialize();
};

