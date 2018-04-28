/*
* File:   FSM_Server.cpp
* Author: Grupo 1
*
* Created on March 24, 2018
*/

/*******************************************************************************
                     INCLUDED HEADERS 
******************************************************************************/
#include "FSM_Server.h"
#include <time.h>
#include <string.h>		
#include <stdio.h>
#include <stdlib.h>
#include "curses.h"		//Libreria grafica
#include "FrontEnd.h"
#include"Windows.h"		//Sleep seguro
#include "event_queue.h"

/*******************************************************************************
						ENUMS, STRUCTS Y TYPEDEFS
******************************************************************************/
//typedef enum {CLEAR,FETCH}mode_t;//typedef for the function get_ev


 /*******************************************************************************
					  CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define PRINT 0        //variable for debugging

#define PRINTF 0 //macro for debugging while using printf instead of PDCurse library
//Messages
#define MSJ_INTRO "Press the following keys to simulate an event:\nMOVE_RECEIVED:'b'\n MOVE_SENT: 'c'\nACK : 'd'\n" \
				  "TIME_OUT : 'e'\nTIME_OUT_2 : 'f\nQUIT : 'g'\nERROR : 'h'\nGARBAGE : 'i'\nRESET : 'j'\n I_AM_READY : 'k'\n"\
	              "INVALID_ACKCODE : 'm'\nEND_COMMUNICATION : 'n'\n"



/*******************************************************************************
         FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
******************************************************************************/
static edge_t* fsm(edge_t* ptr_actualstate , int event1);    //FSM Manager 

static event_t get_ev(mode_t mode);      //event handler   

static void clean_event_queue(void);// 

static void do_nothing(void);//Dummy for the debugging of the protocol structure

static void dummy_printf(mode_t mode, event_t actual_event);

/*******************************************************************************
               STATIC VARIABLES WITH FILE LEVEL SCOPE
 ******************************************************************************/
//Event queue for the FSM
//static event_t event_queue[SIZEEVENTQUEUE];

/*******************************************************************************
                             FSM STATES
******************************************************************************/

//Every state is represent by an array
extern edge_t Initiating_state[];
extern edge_t Waiting_for_ClientOK_state[];
extern edge_t Waiting_for_Event_state[];
extern edge_t Waiting_for_ACK_state[];
extern edge_t Sending_ERROR_state[];


/*******************************************************************************
                           STATES INITIATION
******************************************************************************/

//event_t * ptr_event = &event_queue[0]; //Initation of pointer


 edge_t Initiating_state[]=
{
	{ START_COMMUNICATION, Waiting_for_ClientOK_state, do_nothing},
	{ QUIT, Waiting_for_ACK_state, do_nothing },
	{ ERROR1, Sending_ERROR_state, do_nothing },
	{ END_OF_TABLE,Initiating_state, do_nothing }
};


 edge_t Waiting_for_ClientOK_state[]=
{
	{ I_AM_READY, Waiting_for_Event_state, do_nothing },
	{ QUIT, Waiting_for_ACK_state, do_nothing },
	{ ERROR1, Sending_ERROR_state, do_nothing },
	{ RESET, Initiating_state, do_nothing },
	{ END_OF_TABLE,Waiting_for_ClientOK_state, do_nothing }
};

 edge_t Waiting_for_Event_state[]=
{
	{ MOVE, Waiting_for_ACK_state, do_nothing },
	{ QUIT, Waiting_for_ACK_state, do_nothing },
    { ERROR1, Sending_ERROR_state, do_nothing },
	{ RESET, Initiating_state, do_nothing },
	{ END_OF_TABLE,Waiting_for_Event_state, do_nothing }
};

 edge_t Waiting_for_ACK_state[]=
{
	{ TIME_OUT, Waiting_for_ACK_state, do_nothing },
	{ VALID_ACKCODE, Waiting_for_Event_state, do_nothing },
	{ INVALID_ACKCODE, Sending_ERROR_state, do_nothing },
	{ TIME_OUT_2, Sending_ERROR_state, do_nothing },
	{ QUIT, Waiting_for_ACK_state, do_nothing },
	{ ERROR1, Sending_ERROR_state, do_nothing },
	{ RESET, Initiating_state, do_nothing },
	{ END_OF_TABLE,Waiting_for_ACK_state, do_nothing }
};


 edge_t Sending_ERROR_state[]=
{
	{ ERROR1, Sending_ERROR_state, do_nothing },
	{ RESET, Initiating_state, do_nothing },
	{ END_OF_TABLE,Sending_ERROR_state, do_nothing }
};




/******************************************************************************
*******************************************************************************
					GLOBAL FUNCTION DEFINITIONS
*******************************************************************************

******************************************************************************/
/**************************************************************
					MAIN_FSM
**************************************************************/
int  main(void)
{

	switch_t running = ON;  //Control variable for the execution of the program

	event_queue ev_queue;	// Creation of event_queue object
	ev_queue.clean_queue();
	edge_t *actual_state = Initiating_state; //Creation of a pointer and initiation to the Initiating_state
	event_t actual_event = END_OF_TABLE;
	WINDOW * winTest = NULL;                     //Variable en d�nde se guarda la terminal (Window) en donde voy a trabajar.
	winTest = initscr();
	if (!winTest)
	{
		printf("Falla al crear ventana\n");
	}

	start_color();

	//Declaro los "color pairs" siempre primero foreground y despu�s background.
	init_pair(1, COLOR_BLUE, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_BLACK);

	//Esta opci�n hace que siempre que se llame a una funci�n que escribe se refresque la pantalla.
	immedok(winTest, TRUE);

	//Elijo el primer set de colores (letras azules con fondo negro) y escribo un texto de prueba.
	color_set(3, NULL);


	//Con las dos opciones de abajo elijo que el getch()sea no bloqueante (nodelay TRUE) y que no
	//muestre los caracteres cuando el usuario los escribe (noecho).
	nodelay(winTest, TRUE);
	noecho();

#if PRINT
	int i;
	for (i = 0; i<SIZEEVENTQUEUE; i++)
	{
		printf("%d\t", event_queue[i]);
		event_queue[i] = 0;
	}
#endif
#if PRINTF 
	puts(MSJ_INTRO);
#endif
	event_t old_event = END_OF_TABLE;
	while (running)  //check of the control variable to keep looping
	{
		while ((running)) //check if there is something in the queue or if the user decided to shut off the simulation
		{
			imprimir_simulacion(ESPERANDO,actual_event, old_event, actual_state,winTest, ev_queue); //ESPERANDO, CON EVENTO ANTERIOR
			//dummy_printf(FETCH, 0);//Simulo la llegada de un evento

			//actual_event = get_ev(FETCH);
			actual_event = ev_queue.read_queue();
#if PRINT
			printf("\nLa cola de eventos despues de tomar el evento:\n");
			for (i = 0; i<SIZEEVENTQUEUE; i++)
				printf("%d\t", event_queue[i]);
			puts("\n");
#endif
#if PRINTF 
			dummy_printf(CLEAR, actual_event); //Me fijo que llego
#endif
			
			imprimir_simulacion(PROCESANDO,actual_event, old_event, actual_state, winTest, ev_queue); //Procesando, con EVENTO ACTUAL
			Sleep(500);
			actual_state = fsm(actual_state, actual_event); //The FSM returns the next state
			old_event = actual_event;
			

#if PRINTF 

			{ //Print actual state
				if (actual_state == Initiating_state)
					puts("Estado actual:Initiating_state");

				if (actual_state == Waiting_for_ClientOK_state)
					puts("Estado actual:Waiting_for_ClientOK_state");

				if (actual_state == Finishing_configuration)
					puts("Estado actual:Finishing_configuration");

				if (actual_state == Looping_state)
						puts("Estado actual:Looping_state");

				if (actual_state == Waiting_to_send_ACK_state)
					puts("Estado actual:Waiting_to_send_ACK_state");

				if (actual_state == Waiting_for_ACK_state)
					puts("Estado actual:Waiting_for_ACK_state");

				if (actual_state == Resending_MOVE)
					puts("Estado actual:Resending_MOVE");

				if (actual_state == Analyzing_ACK)
					puts("Estado actual:Analyzing_ACK");

				if (actual_state == Sending_ERROR)
					puts("Estado actual:Sending_ERROR");


			}

			
#endif
			if ((old_event == ERROR1)&&(actual_state==Sending_ERROR)) //The loop ends when the ERRO1 event arrives, flaging that the simulation is shutting off
				running = OFF;
		}

		
	}
	Sleep(1500);
	endwin();
	return EXIT_SUCCESS;
}


/**************************************************************
					FSM MANAGER
**************************************************************/
/*
*This function manage the FSM.
*
*INPUT:
*It receives the actual state of the fsm and the event that is hold in the edge 
*that contains the action routine.
*
*OUTPUT:
*The program returns a pointer to the next state.
*/
edge_t* fsm(edge_t* ptr2actual_state, int event1)
{

	while ((ptr2actual_state->event != event1) && (ptr2actual_state->event != END_OF_TABLE))
	{	//Search for the received event, it increase the pointer until the detection
		ptr2actual_state++;
	}

	//Runs the functions realted to that event
	(*ptr2actual_state->fun_trans)();

	return (ptr2actual_state->nextstate);
}


/**************************************************************
						GET_EV
**************************************************************/
/*
*Function that manage the extraction of events from the queue. 
*
*INPUT:
*Mode: If call in FETCH mode you obtain the next tracked event. In CLEAR mode, the queue is cleared.
*
*OUTPUT:
*The program returns the event extracted.
*/
/*static event_t get_ev(mode_t mode)
{
	static int index = 0;        //index for event reading
	event_t extracted_event = END_OF_TABLE;
	//getchar();
	if (mode) 
	{
		//if (event_queue[index])   //if there is an event in the queue, it extracted the event and increment the index
		{
			extracted_event = event_qu[index++];    //if it can not found one, it retruns 0 that equals an END_OF_TABLE 
											  //if the latter happens the fsm does nothing and waits for the next event
		}
		

	}
	else //CLEAN mode
	{
		index = 0;
		clean_event_queue();
		ptr_event = &event_queue[0];
	}
	//puts("Se extrajo un evento mediante getev()\n");
	return extracted_event; //Devuelvo el evento extraido
}
*/



/**************************************************************
				   CLEAN_EVENT_QUEUE
*************************************************************
static void clean_event_queue(void)
{
	for (uint i = 0; i < SIZEEVENTQUEUE; i++)
		event_queue[i] = END_OF_TABLE;
;
}*/
 
static void do_nothing(void)
{

}

/**************************************************************
                    DUMMY_PRINTF
**************************************************************/
static void dummy_printf(mode_t mode, event_t actual_event)
{/*
	if (mode == FETCH)
	{	
		puts("Ingrese un evento:\n");
		fflush(NULL);
		char u;
		scanf("%c", &u);

		switch (u)
		{
		case('b'):
			*ptr_event++ = MOVE_RECEIVED;
			break;
		case('c'):
			*ptr_event++ = MOVE_SENT;
			break;
		case('d'):
			*ptr_event++ = ACK;
			break;
		case('e'):
			*ptr_event++ = TIME_OUT;
			break;
		case('f'):
			*ptr_event++ = TIME_OUT_2;
			break;
		case('g'):
			*ptr_event++ = QUIT;
			break;
		case('h'):
			*ptr_event++ = ERROR;
			break;
		case('j'):
			*ptr_event++ = RESET;
			break;
		case('k'):
			*ptr_event++ = I_AM_READY;
			break;
		case('m'):
			*ptr_event++ = INVALID_ACKCODE;
			break;
		case('n'):
			*ptr_event++ = END_COMMUNICATION;
			break;
		case('o'):
			*ptr_event++ = VALID_ACKCODE;
			break;

		case('i'):default:
			*ptr_event++ = GARBAGE;
			break;

		}
	}

	else
	{		
		switch (actual_event)
		{
		case(MOVE_RECEIVED):
			puts("Se recibi� el evento MOVE RECEIVED");
			break;
		case(MOVE_SENT):
			puts("Se recibi� el evento MOVE SENT");
			break;
		case(ACK):
			puts("Se recibi� el evento ACK");
			break;
		case(TIME_OUT):
			puts("Se recibi� el evento TIME_OUT");
			break;
		case(TIME_OUT_2):
			puts("Se recibi� el evento TIME_OUT_2");
			break;
		case(QUIT):
			puts("Se recibi� el evento QUIT");
			break;
		case(ERROR):
			puts("Se recibi� el evento ERROR");
			break;
		case(RESET):
			puts("Se recibi� el evento RESET");
			break;
		case(I_AM_READY):
			puts("Se recibi� el evento I_AM_READY");
			break;
		case(INVALID_ACKCODE):
			puts("Se recibi� el evento INVALID_ACKCODE");
			break;
		case(VALID_ACKCODE):
			puts("Se recibi� el evento VALID_ACKCODE");
			break;
		case(END_COMMUNICATION):
			puts("Se recibi� el evento END_COMMUNICATION");
			break;
		case(END_OF_TABLE):
			puts("Se recibi� el evento END_OF_TABLE");
			break;
		default:
			puts("Se recibi� el evento GARBAGE");
			break;
		}
	}*/
}