#include "EventManagement.h"
#include "scenario.h"
#include "general.h"

#include <iostream>
using namespace std;



#define W2_MOV_RIGHT ALLEGRO_KEY_RIGHT
#define W2_MOV_LEFT ALLEGRO_KEY_LEFT
#define W2_MOV_JUMP ALLEGRO_KEY_UP

#define W1_MOV_RIGHT ALLEGRO_KEY_D
#define W1_MOV_LEFT ALLEGRO_KEY_A
#define W1_MOV_JUMP ALLEGRO_KEY_W

EventManagement::EventManagement(ALLEGRO_DISPLAY * display)
{
	this->errorLoading = false;
	bool beingheld1 = false;
	bool beingheld2 = false;


	this->drawingTimer = al_create_timer(1.0 / FPS);
	if (!drawingTimer) {
		std::cout << "ERROR: Failed to create timer." << std::endl;
		errorLoading = true;
	}
	this->movingTimerWorm1 = al_create_timer(1.0 / MOVE_FPS);
	this->movingTimerWorm2 = al_create_timer(1.0 / MOVE_FPS);
	
	if (!errorLoading && !movingTimerWorm1) {
		std::cout << "ERROR: Failed to create timer." << std::endl;
		errorLoading = true;
	}

	if (!errorLoading && !movingTimerWorm1) {
		std::cout << "ERROR: Failed to create timer." << std::endl;
		errorLoading = true;
	}

	event_queue = al_create_event_queue();
	if (!errorLoading && !event_queue) {
		std::cout << "ERROR: failed to create event_queue!" << std::endl;
		al_destroy_timer(drawingTimer);
		al_destroy_timer(movingTimerWorm1);
		al_destroy_timer(movingTimerWorm2);
		errorLoading = true;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(drawingTimer));
	al_register_event_source(event_queue, al_get_timer_event_source(movingTimerWorm1));
	al_register_event_source(event_queue, al_get_timer_event_source(movingTimerWorm2));

	al_start_timer(drawingTimer);

	this->move1 = false;
	this->move2 = false;
	this->redraw = false;
	this->gameFinished = false;
}


EventManagement::~EventManagement()
{
	al_destroy_timer(drawingTimer);
	al_destroy_timer(movingTimerWorm1);
	al_destroy_timer(movingTimerWorm2);
	al_destroy_event_queue(event_queue);
}

//recibe y procesa un evento por teclado. 
void EventManagement::receiveEvent(scenario& stage) {

	if (al_get_next_event(event_queue, &ev)) //Toma un evento de la cola, en caso de que esta no este vacia.
	{

		if (beingheld1) {
			this->move1 = true;
			stage.setLoopState(WORM1, false);

		if ((lastStateWorm1 = stage.getWormState(WORM1)) != QUIET) {
				stage.setWormState(WORM1, lastStateWorm1);
			}
		}

		if (beingheld2){ // Si mantengo presionado, debo seguir caminando. 
			this->move2 = true;
			stage.setLoopState(WORM2, false);

			/*if ((lastStateWorm2 = stage.getWormState(WORM1)) != QUIET) {
				stage.setWormState(WORM2, lastStateWorm2);
			}*/

		}

		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			this->finishGame();
			return;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			if (!beingheld1) {

				switch (ev.keyboard.keycode) {

				case W1_MOV_RIGHT:
					if (this->move1 ==false) {
						this->keyPressedWorm1 = RIGHT;
						this->keyPressedWorm = WORM1;
						al_start_timer(movingTimerWorm1);
						this->beingheld1 = true;
					}
					break;
				case W1_MOV_LEFT:
					if (this->move1 == false) {
						this->keyPressedWorm1 = LEFT;
						this->keyPressedWorm = WORM1;
						al_start_timer(movingTimerWorm1);
						this->beingheld1 = true;
					}
					break;
				case W1_MOV_JUMP:
					if (this->move1 == false) {
						this->keyPressedWorm1 = UP;
						this->keyPressedWorm = WORM1;
						al_start_timer(movingTimerWorm1);
						this->beingheld1 = true;
					}
					break;
				}
			}

			

			if (!beingheld2) {
				switch (ev.keyboard.keycode) {
					case W2_MOV_RIGHT:
						if (this->move2 == false) {
							this->keyPressedWorm2 = RIGHT;
							this->keyPressedWorm = WORM2;
							al_start_timer(movingTimerWorm2);
							this->beingheld2 = true;
						}
						break;
					case W2_MOV_LEFT:
						if (this->move2 == false) {
							this->keyPressedWorm2 = LEFT;
							this->keyPressedWorm = WORM2;
							al_start_timer(movingTimerWorm2);
							this->beingheld2 = true;
						}
						break;
					case W2_MOV_JUMP:
						if (this->move2 == false) {
							this->keyPressedWorm2 = UP;
							this->keyPressedWorm = WORM2;
							al_start_timer(movingTimerWorm2);
							this->beingheld2 = true;
						}
						break;
					}
				}
			}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode)
			{
			case W1_MOV_RIGHT:
				if (this->keyPressedWorm1 == RIGHT) { //Tomo acción solo si la tecla que levanté es la que en principio estaba presionando.
					this->keyPressedWorm = WORM1;
					al_stop_timer(movingTimerWorm1);
					this->beingheld1 = false;
				}
				break;
			case W1_MOV_LEFT:
				if (this->keyPressedWorm1 == LEFT) { //Tomo acción solo si la tecla que levanté es la que en principio estaba presionando.
					this->keyPressedWorm = WORM1;
					al_stop_timer(movingTimerWorm1);
					this->beingheld1 = false;
				}
				break;
			case W1_MOV_JUMP:
				if (this->keyPressedWorm1 == UP) { //Tomo acción solo si la tecla que levanté es la que en principio estaba presionando.
					this->keyPressedWorm = WORM1;
					al_stop_timer(movingTimerWorm1);
					this->beingheld1 = false;
				}
				break;
			case W2_MOV_RIGHT:
				if (this->keyPressedWorm2 == RIGHT) { //Tomo acción solo si la tecla que levanté es la que en principio estaba presionando.
					this->keyPressedWorm = WORM2;
					al_stop_timer(movingTimerWorm2);
					this->beingheld2 = false;

				}
			case W2_MOV_LEFT:
				if (this->keyPressedWorm2 == LEFT) { //Tomo acción solo si la tecla que levanté es la que en principio estaba presionando.
					this->keyPressedWorm = WORM2;
					al_stop_timer(movingTimerWorm2);
					this->beingheld2 = false;

				}
			case W2_MOV_JUMP:
				if (this->keyPressedWorm2 == UP) { //Tomo acción solo si la tecla que levanté es la que en principio estaba presionando.
					this->keyPressedWorm = WORM2;
					al_stop_timer(movingTimerWorm2);
					this->beingheld2 = false;

				}
				break;
			}

		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {

			if (ev.timer.source == (this->movingTimerWorm1)){
				this->move1 = true;
				al_stop_timer(this->movingTimerWorm1);
			}
			else if (ev.timer.source == this->movingTimerWorm2) {
				this->move2 = true;
				al_stop_timer(this->movingTimerWorm2);
			}
			else if (ev.timer.source == this->drawingTimer) {

				this->redraw = true;

				//printf("WORM1: %d WORM2: %d \n", stage.getLoopState(WORM1), stage.getLoopState(WORM2));
				if (this->move1 == true && !stage.getLoopState(WORM1)) {
					this->drawWorm1 = true;
				}
				else
				{

					if (stage.getLoopState(WORM1)) {
						this->keyPressedWorm1 = NO_MOV;
						stage.setLoopState(WORM1, false);
					}

					this->move1 = false;
					stage.setLoopState(WORM1, false);

					//stage.setWormState(WORM2, QUIET);
					//printf("move1 en falso \n");
				}

				if (this->move2 == true && !stage.getLoopState(WORM2)) {
					this->drawWorm2 = true;
				}
				else
				{
					//printf("WORM2 NO CAMINA \n");
					this->move2 = false;

					if (stage.getLoopState(WORM2)) {
						this->keyPressedWorm2 = NO_MOV;
						stage.setLoopState(WORM2, false);
					}

					//stage.setWormState(WORM2, QUIET);
					//printf("move2 en falso \n");
				}
			}
		}
	}

}

void EventManagement::handleEvent(scenario& stage) {

	if (this->drawWorm1) {
		if (this->keyPressedWorm1 == NO_MOV) {			//caso en que se solto la tecla o esta suelta ya.
				stage.resetTicksFor(WORM1);			//RESETEA EL TIEMPO DE ESPERA PERO ADEMAS DICE QUE NO SE PUEDE MOVER EL WORM!!!
			}
		else if ((this->keyPressedWorm1 == RIGHT) || (this->keyPressedWorm1== LEFT))
		{
			stage.tickFor(WORM1);
			stage.handleWormMovement(WORM1, this->keyPressedWorm1);
		}
		else if (this->keyPressedWorm1 == UP)
		{
			stage.tickFor(WORM1);
			stage.handleWormMovement(WORM1, this->keyPressedWorm1);
		}
		this->drawWorm1 = false; //En falso para que solo se dibuje una vez por frame.
	}

	if (this->drawWorm2) {
		if (this->keyPressedWorm2 == NO_MOV) {			//caso en que se solto la tecla o esta suelta ya.
			stage.resetTicksFor(WORM2);			//RESETEA EL TIEMPO DE ESPERA PERO ADEMAS DICE QUE NO SE PUEDE MOVER EL WORM!!!
		}
		else if ((this->keyPressedWorm2 == RIGHT) || (this->keyPressedWorm2 == LEFT))
		{
			stage.tickFor(WORM2);
			stage.handleWormMovement(WORM2, this->keyPressedWorm2);
		}
		else if (this->keyPressedWorm2 == UP)
		{
			stage.tickFor(WORM2);
			stage.handleWormMovement(WORM2, this->keyPressedWorm2);
		}
		this->drawWorm2 = false; //En falso para que solo se dibuje una vez por frame.
	}

}

void EventManagement::finishGame(void) {
	this->gameFinished = true;
}
bool EventManagement::gameIsFinished(void) {
	return this->gameFinished;
}

bool EventManagement::shouldRedraw(void) {
	return this->redraw;
}
