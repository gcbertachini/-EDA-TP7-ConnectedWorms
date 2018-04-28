#pragma once

#include <allegro5/allegro.h>
#include "scenario.h"

class EventManagement
{
public:

	EventManagement(ALLEGRO_DISPLAY * display);
	~EventManagement();
	void receiveEvent(scenario& stage);			//checks for events and updates its info accordingly
	void handleEvent(scenario& stage); //applies changes to the scenario object so that the worms and stage will reflect the user input.
	void finishGame(void);			//informs the event manager the game has finished.
	bool gameIsFinished(void);		//returns true if the game has finished
	bool shouldRedraw(void);
	bool errorLoading;

private:

	wormEnum_t keyPressedWorm;
	wormMoves_t keyPressedWorm1;
	wormMoves_t keyPressedWorm2;
	wormState_t lastStateWorm1;
	wormState_t lastStateWorm2;

	bool gameFinished;			//boolean that indicates whether the game has finished or not. 
	bool move1;                  //boolean that indicates when actions regarding the movingTimer should be taken.
	bool move2;
	bool drawWorm1;
	bool drawWorm2;
	bool redraw;                  //boolean that indicates when actions regarding the drawingTimer should be taken.
	bool beingheld1;
	bool beingheld2;


	ALLEGRO_EVENT_QUEUE * event_queue;
	ALLEGRO_TIMER * drawingTimer;
	ALLEGRO_TIMER * movingTimerWorm1;
	ALLEGRO_TIMER * movingTimerWorm2;
	ALLEGRO_EVENT ev;


};
