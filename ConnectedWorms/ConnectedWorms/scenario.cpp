#pragma once
#include <allegro5\allegro.h>
#include <iostream>
#include "scenario.h"


#define SCENARIO "Scenario.png"
#define BACKGROUND "mountains.png"

scenario::scenario() : worm1(WORM1, WQTY), worm2(WORM2, WQTY) {

	this->errorLoading = this->setBackgroundAndMainStage();

	if (!this->errorLoading)
		this->errorLoading = this->worm1.setStateImgs();			//static method for all worms.

	this->worm1.refresh();
	this->worm2.refresh();
} 

scenario::~scenario() {

	al_destroy_bitmap(this->mainStage);
	al_destroy_bitmap(this->background);

}

position scenario::getWormPos(wormEnum_t n) {

	position wormpos;
	if (n == WORM1) {
		wormpos = worm1.getPos();
	}
	else {
		wormpos = worm2.getPos();
	}
	return wormpos;
}

bool scenario::setBackgroundAndMainStage(void) {
	bool abort = false;

	this->mainStage = NULL;
	this->background = NULL;

	if (!(this->mainStage = al_load_bitmap(SCENARIO))) {
		std::cout << "ERROR: Could not initialize bitmaps." << std::endl;
		abort = true;

	}
	if (!(this->background = al_load_bitmap(BACKGROUND))) {
		std::cout << "ERROR: Could not initialize bitmaps." << std::endl;
		abort = true;
	}
	
	return abort;
}

void scenario::draw(void)
{

	al_draw_scaled_bitmap(this->background, 0, 0, al_get_bitmap_width(this->background), al_get_bitmap_height(this->background), 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0); //Dibujo el fondo
	al_draw_bitmap(this->mainStage, 0, 0, 0); //Dibujo el escenario 

	
	//draw de los worms en el escenario.


	al_draw_bitmap(this->worm1.getToDrawState(), this->worm1.getPos().coordX, this->worm1.getPos().coordY, this->worm1.getFacedSide());
	al_draw_bitmap(this->worm2.getToDrawState(), this->worm2.getPos().coordX, this->worm2.getPos().coordY, this->worm2.getFacedSide());
}

void scenario::handleWormMovement(wormEnum_t worm, wormMoves_t direction) {
	
	if (worm == WORM1) {
		this->handleWormMovement(worm1, direction);

	}
	else if (worm == WORM2) {
		this->handleWormMovement(worm2, direction);
	}
	
}
void scenario::handleWormMovement(worm& worm, wormMoves_t direction) {

	if ( direction != NO_MOV ) {

		float currentX = worm.getPos().coordX;		
		float currentY = worm.getPos().coordY;		


		switch (direction) {
		case RIGHT:

			if (worm.getState() == WALKING_RIGHT) {

				worm.walkingTick(RIGHT);
			}
			else if (worm.getState() == QUIET) {
				worm.startWalking(direction);
				worm.setFacedSide(direction);
			}
			
			break;

		case LEFT:
			if (worm.getState() == WALKING_LEFT) {

				worm.walkingTick(LEFT);
			}
			else if (worm.getState() == QUIET) {
				worm.startWalking(direction);
				worm.setFacedSide(direction);
			}
			
			break;

		case UP:
			if (worm.getState() == JUMPING || worm.getMoving())
				worm.jumpingTick();
			else if (worm.getState() == QUIET) 
				worm.startJumping();
			
			break;
			
		}
	}
	
}


wormState_t scenario::getWormState(wormEnum_t wormN) {

	wormState_t wormState = QUIET;

	if (wormN == WORM1) {
		wormState = this->worm1.getState();
	}
	else if (wormN == WORM2) {
		wormState = this->worm1.getState();
	}
	return wormState;
}




void scenario::resetTicksFor(wormEnum_t wormN) {
	if (wormN == WORM1) {
		worm1.resetTicks();
	}
	else if (wormN == WORM2) {
		worm2.resetTicks();
	}
}
void scenario::tickFor(wormEnum_t wormN) {
	if (wormN == WORM1) {
		worm1.tick();
	}
	else if (wormN == WORM2) {
		worm2.tick();
	}
}

bool scenario::getLoopState(wormEnum_t wormN) {
	
	bool loopState = 0;
	if (wormN == WORM1) {
		loopState = this->worm1.getLoop();
	}
	else if (wormN == WORM2) {
		loopState = this->worm2.getLoop();
	}

	return loopState;
}



void scenario::setLoopState(wormEnum_t wormN, bool setValue) {

	bool loopState = 0;

	if (wormN == WORM1) {
		this->worm1.setLoop(setValue);
	}
	else if (wormN == WORM2) {
		this->worm2.setLoop(setValue);
	}

}


void scenario::setWormState(wormEnum_t wormN, wormState_t state) {
	if (wormN == WORM1) {
		this->worm1.setState(state);
	}
	else if (wormN == WORM2) {
		this->worm2.setState(state);
	}
}

void scenario::setMoving(wormEnum_t wormN, bool setValue) {
	if (wormN == WORM1) {
		this->worm1.setMove(setValue);
	}
	else if (wormN == WORM2) {
		this->worm2.setMove(setValue);
	}
}