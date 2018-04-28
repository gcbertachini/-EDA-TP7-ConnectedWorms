#pragma once

#include <allegro5\allegro5.h>
#include <allegro5\allegro_image.h>
#include "position.h"
#include "general.h"
#define _USE_MATH_DEFINES
#include <cmath>
 
#define AMOUNT_OF_WALKING_IMAGES 15		//total amount of different bitmaps that involve the walking process for a worm.
#define AMOUNT_OF_JUMPING_IMAGES 10		//total amount of different bitmaps that involve the jumping process for a worm.

#define XMAXSTARTINGPOS 1150
#define XMINSTARTINGPOS 701
#define YSTARTINGPOS 616

class worm {
private:
	position pos;
	wormState_t estado;
	ALLEGRO_BITMAP * drawState;
	unsigned int tickCount;


	static ALLEGRO_BITMAP * walkImgs[AMOUNT_OF_WALKING_IMAGES];
	static ALLEGRO_BITMAP * jmpImgs[AMOUNT_OF_JUMPING_IMAGES];
	static ALLEGRO_BITMAP * quietImg;
	static double gravity;
	static double degree;

	unsigned int walkIndex;
	unsigned int jmpIndex;

	wormMoves_t facedSide;
	int frameNum;

	bool shouldMove;
	unsigned int ticks;
	bool loopIsOver;
	bool amMoving;

public:
	
	worm(wormEnum_t wormN, unsigned int wormQty);
	wormState_t getState();
	void setState(wormState_t state);
	static bool setStateImgs();
	void setPos(float x, float y);
	position getPos();

	ALLEGRO_BITMAP * getToDrawState(); 
	ALLEGRO_BITMAP * setNewDrawState(wormState_t newState);

	void refresh();
	void handleMovement(wormMoves_t direction);
	void startWalking(wormMoves_t direction);
	void startJumping();
	void jumpingTick();
	void walkingTick(wormMoves_t direction);

	wormMoves_t getFacedSide();
	void setFacedSide(wormMoves_t direction);

	bool getLoop();
	void setLoop(bool setValue);
	bool getMoving();
	void setMove(bool setValue);

	void tick(void);
	//resetea los ticks del movement para esperar 100ms ppero aparte le dice al worm que no se puede mover!!!
	void resetTicks();
};