#include "netData.h"
#include "Communication.h"
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>    

#define IPFILE "DireccionesIP.txt"
#define HELLO_PORT_STR "12345"
#define HELLO_PORT 12345


Communication::Communication(netData * net)
{

	IO_handler = new boost::asio::io_service();
	socket = new boost::asio::ip::tcp::socket(*IO_handler);
	acceptor = new boost::asio::ip::tcp::acceptor(*IO_handler,
	boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), HELLO_PORT));
	resolver = new boost::asio::ip::tcp::resolver(*IO_handler);	

	this->net = net;
	
	this->numberOfActiveMachines = net->getIPListSize(); //Ya cargadas todas las IPs, el tamaño del vector corresponde a la cantidad de sistemas involucrados.
	this->shouldInit = false;

}

Communication::~Communication()
{
	acceptor->close();
	socket->close();
	delete resolver;
	delete acceptor;
	delete socket;
	delete IO_handler;
	
}

/******************************
***********initCom*************
*******************************
initCom pide la secuencia de inicio al usuario, se fija si tiene que correr la animacion,
la corre en caso de tener que hacerlo, y llena el paquete YOU_GO con el count apropiado. 

INPUT:
	1) draw: Drawer para correr la animacion en caso de necesitarlo
OUTPUT:
	void.
*/
void Communication::initCom(Drawer & draw)
{

	askForInitSequence();	//pido los datos necesarios para iniciarla (secuencia en la que se correra la animación

	if (net->getOwnIP() == net->getIPAt(this->YOU_GO[MACHINES_ID]-1)) {		//me fijo si soy el primero qeu tiene que mandar el mensaje	
		draw.runAnimation(this->animation);		//corro la animacion en caso de tener que correr.
		this->YOU_GO[COUNT_ID] = 1;				//informo que mande el mensaje en el paquete YOU_GO. Estoy teniendo en cuenta que sendMessage() incrementa en uno siempre antes de mandar
	}
	else
		this->YOU_GO[COUNT_ID] = 0;				//informo que nadie mando el mensaje. Estoy teniendo en cuenta que sendMessage() incrementa en uno siempre antes de mandar

	int turno = this->YOU_GO[COUNT_ID];		
	const char* nextIP = this->net->getIPAt(this->YOU_GO[MACHINES_ID + turno] - 1);

	startConnectionForServer(nextIP);			//comienzo conexion con la maquina que no es la mia
	sendMessage();//mando el paquete YOU_GO a la maquina especifica.
		
}

/******************************
***********endCom*************
*******************************
endCom avisa que se cierra el programa y espera al ACK para finalizar

INPUT:
	void.
OUTPUT:
	void.
*/
void Communication::endCom() {
	//aqui se informaria a todas las computadoras que se ha terminado la conversacion. 
	//Esto seria posible si hubiese un protocolo de comunicacion que cubra esto.
}

/*****************************************
***********askForInitSequence*************
******************************************
askForInitSequence pide al usuario la secuencia de arranque para cuando la maquina funciona como server.
Hace chequeo de input.
INPUT:
	void.
OUTPUT:
	void.
*/
void Communication::askForInitSequence() {

	std::cout << "Debe inicializar." << std::endl;

	bool initCorrect = false;

	std::cout << "Indique la animación a enviar." << std::endl;
	std::cout << "A. Cat Running" << std::endl;
	std::cout << "B. Explosion 1 " << std::endl;
	std::cout << "C. Explosion 2 " << std::endl;
	std::cout << "D. Homer Dance " << std::endl;
	std::cout << "E. Super Mario " << std::endl;
	std::cout << "F. Sonic" << std::endl;

	while (!initCorrect) {

		std::string input = "";
		std::getline(std::cin, input, '\n');

		boost::to_upper(input);			//transformo todo a mayuscula para comparar
		//hago que el input sea correcto salvo que se demuestre lo contrario, 
		//para simmplificar escritura y lectura de codigo en la siguiente cadena de if else.
		initCorrect = true;
		if (!input.compare("A"))
			this->animation = CAT;
		else if (!input.compare("B"))
			this->animation = EXPLOSION1;
		else if (!input.compare("C"))
			this->animation = EXPLOSION2;
		else if (!input.compare("D"))
			this->animation = HOMER;
		else if (!input.compare("E"))
			this->animation = MARIO;
		else if (!input.compare("F"))
			this->animation = SONIC;
		else {
			initCorrect = false;
			std::cout << std::endl;
			if(input != "")
				std::cout << "INPUT INCORRECTO." << std::endl;
		}
	}
	int youGoIndex = 0;			//indice con el que se va a ir llenando el paquete YOU_GO para la secuencia
	this->YOU_GO[youGoIndex] = this->animation;		//pego la animacion al mensaje YOU_GO
	youGoIndex = MACHINES_ID;		//Continuare llenando YOU_GO. Comenzare llenando las maquinas.
	std::cout << std::endl;

	initCorrect = false;
	
	while (!initCorrect) {

		std::cout << "Indique la secuencia a traves de la cual se procedera a enviar la animacion." << std::endl;
		std::cout << "El input de la secuencia sera de numeros seguidos de espacios" << std::endl;
		std::string input = "";
		std::string rawInput;
		
		getline(std::cin, rawInput);				//tomo una linea
		std::stringstream ss(rawInput);				//la paso a un stream para poder parsearla
		while (std::getline(ss, rawInput, ' '))		//tomo una palabra/secuencia seguida de digitos sin separacion de espacios.
		{
			if( (rawInput == "\n" ) && ( (youGoIndex - MACHINES_ID) == this->numberOfActiveMachines) ) {
				break;
			}
			bool isNum = true;
			for (int i = 0; i < rawInput.length() && isNum; ++i)
				isNum = isdigit(rawInput[i]);					//voy revisando digito a digito de la palabra ingresada.
			
			if (isNum)			//en caso de que todos los de la palabra hayan sido numeros
			{
				int num = atoi(rawInput.c_str());		//convierto ese numero a int para poder hacer comparaciones y demas
				if ((num <= MAX_N_MAQUINAS) && (num >= 1)) {		//chequeo que el numero que representa este en el rango aceptado de maquinas
					char byteSecuencia = num;
					if ((youGoIndex - MACHINES_ID) < this->numberOfActiveMachines) {
						this->YOU_GO[youGoIndex] = num;
						youGoIndex++;					//paso a la proxima maquina en el mensaje YOU_GO
					}
					else {
						std::cout << "Ha introducido mas datos en la secuencia que maquinas activas!!" << std::endl;
						youGoIndex = MACHINES_ID;
						break;
					}

				}
				else{
					std::cout << "El numero introducido no dentro de los valores permitidos!" << std::endl;	
					youGoIndex = MACHINES_ID;
					break;
				}
			}
			else{
				std::cout << rawInput << "Ha ingresado algo que no es un número!" << std::endl;
				youGoIndex = MACHINES_ID;
				break;
			}
		}
		if(youGoIndex > MACHINES_ID)		//en caso de haber logrado llenar las maquinas y que no tuve que resetear el indice > no hubo ningun problema.
			initCorrect = true;
	}


}

/*****************************************
***********listenAndRespond*************
******************************************
listenAndRespond recibe mensaje (hace rol de cliente), luego corre la animacion en caso de tener que hacerlo.  
En caso de llegar al final de la secuencia, pide al usuario lo que debe hacer a continuacion

INPUT:
	1) draw: Drawer para correr la animacion en caso de necesitarlo
OUTPUT:
	void.
*/
void Communication::listenAndRespond(Drawer & draw) {

	startConnectionForClient();							//me pongo a escuchar
	receiveMessage();									//recivo el mensaje YOU_GO
	parseMessage();

	int turno = this->YOU_GO[COUNT_ID];

	if (this->net->getOwnIP() == this->net->getIPAt(this->YOU_GO[MACHINES_ID + turno - 1]-1)) {			//me fijo si tengo que correr la animacion

		draw.runAnimation(this->animation);

		if (this->YOU_GO[COUNT_ID] == this->net->getIPListSize()) {						//me fijo si llegue al final de la secuencia
			
			std::cout << "Debe ingresar la palabra 'iniciar' para comenzar una nueva secuencia." << std::endl;
			std::cout << "Tambien puede ingresar la palabra 'terminar' para cerrar la conexion y terminar el programa.s" << std::endl;
			bool InputCorrecto = false;

			while (!InputCorrecto) {
				string input;
				std::cin >> input;
				boost::algorithm::to_lower(input); //paso a minusculas para comparar.
				if (input == "iniciar") {
					this->net->setCurrentMode(SERVER);
					//this->initCom(draw);			//actuo como server.
					InputCorrecto = true;
				}
				else if (input == "terminar") {			//tengo que terminar el programa.
					net->setIfShouldEnd(true);			//termino el programa.
					this->endCom();
					InputCorrecto = true;
				}
				else 
					std::cout << "Debe ingreso una palabra incorrecta. Por favor ingrese una palabra valida" << std::endl;	
			}
		}		
		else {
			const char* nextIP = this->net->getIPAt(this->YOU_GO[MACHINES_ID + turno] - 1);			//consigo la IP de la maquina a la que le tengo qeu mandar el paquete.
			startConnectionForServer(nextIP);			//comienzo conexion la maquina que no sea la mia.
			sendMessage();								//mando el paquete YOU_GO a la maquina especifica.
			this->net->setCurrentMode(CLIENT);			//deje de ser server porque ya mande el mensaje
		}
	}
	

}


/*****************************************
***********startConnectionForServer****************
******************************************
startConnectionForServer inicia la conexión con maquina siendo server.

INPUT:
	void.
OUTPUT:
	void.
*/
void Communication::startConnectionForServer(const char * host) {
	this->net->setCurrentMode(SERVER);
	renewServerItems();
	try{

		endpoint = resolver->resolve(boost::asio::ip::tcp::resolver::query(host, HELLO_PORT_STR));

		boost::asio::connect(*socket, endpoint);

		socket->non_blocking(true);
	}
	catch (exception & e) {
		std::cout << "Error al intentar conectar" << std::endl;
		net->setIfShouldEnd(true);
	}
	
}

/*****************************************
***********startConnectionForClient****************
******************************************
startConnectionForClient inicia la conexión con maquina siendo cliente.

INPUT:
void.
OUTPUT:
void.
*/
void Communication::startConnectionForClient() {
	

	this->net->setCurrentMode(CLIENT);
	renewClientItems();

	acceptor->accept(*socket);

	socket->non_blocking(true);
	

}
/*****************************************
***********sendMessage********************
******************************************
sendMessage INCREMENTA EL COUNT y luego envia el mensaje a una sola maquina en espefico.

INPUT:
	void.
OUTPUT:
	void.
*/
void Communication::sendMessage() {

	this->YOU_GO[COUNT_ID]++;			//antes de mandar siempre verifico si me 

	size_t len;
	boost::system::error_code error;
	
	do
	{
		len = socket->write_some(boost::asio::buffer(this->YOU_GO, strlen(this->YOU_GO)), error);
	} 
	while ((error.value() == WSAEWOULDBLOCK));

	if (error)
		std::cout << "Error while trying to send message. " << error.message() << std::endl;
}

/*****************************************
***********receiveMessage*****************
******************************************
sendMessage INCREMENTA EL COUNT y luego envia el mensaje a una sola maquina en espefico.

INPUT:
void.
OUTPUT:
void.
*/
void Communication::receiveMessage() {

	boost::system::error_code error;	

	char buf[MAX_N_MAQUINAS + 3];		// por donde recibire el input. numero de maquinas mas 2 bytes por count y por animations

	size_t len = 0;

	do
	{
		len = socket->read_some(boost::asio::buffer(buf), error);			//leo el input que me envia la otra maquina
		if (!error)							
			buf[len] = '\0';							//esto deberia ser cambiado! deberia terminarlo en la cantidad de bytes tomados o no terminarlo directamente.
				
	} while (error.value() == WSAEWOULDBLOCK);

	if (!error) 
		if (len >= (this->numberOfActiveMachines + 2)) 				//copio la cantidad de caracteres que necesito.
			for (int i = 0; i< (this->numberOfActiveMachines + 2); i++) 
				this->YOU_GO[i] = buf[i];							//cargo el paquete YOU_GO
	else
		std::cout << "Error while trying to connect to server " << error.message() << std::endl;
}

/***************************************
***********parseMessage*****************
****************************************
parseMessage procesa el mensaje YOU_GO y su informacion.

INPUT:
	void.
OUTPUT:
	void.
*/
void Communication::parseMessage() {
	this->animation = (animations) this->YOU_GO[ANIMATION_ID];
}

bool Communication::shouldInitialize() {
	return this->shouldInit;
}
void Communication::setShouldInitialize(bool should) {
	this->shouldInit = should;
}


void Communication::renewServerItems() {

	delete socket;
	delete IO_handler;
	delete resolver;

	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	IO_handler = new boost::asio::io_service();
	socket = new boost::asio::ip::tcp::socket(*IO_handler);
	resolver = new boost::asio::ip::tcp::resolver(*IO_handler);
}
void Communication::renewClientItems() {

	delete acceptor;
	delete socket;
	delete IO_handler;
	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	IO_handler = new boost::asio::io_service();
	socket = new boost::asio::ip::tcp::socket(*IO_handler);
	acceptor = new boost::asio::ip::tcp::acceptor(*IO_handler,
		boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), HELLO_PORT));
}