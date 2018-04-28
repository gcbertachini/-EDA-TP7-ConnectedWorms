#pragma once
#include <allegro5\allegro.h>
#include "worm.h"
#include "general.h"


class scenario {

private:
	worm worm1; //El escenario debe contener dos worms.
	worm worm2;
	ALLEGRO_BITMAP * mainStage;
	ALLEGRO_BITMAP * background;
	bool setBackgroundAndMainStage();
	void handleWormMovement(worm & worm, wormMoves_t direction);

	
public:
	void draw(void);
	scenario();
	virtual ~scenario();
	bool  errorLoading;
	position getWormPos(wormEnum_t n);
	wormState_t getWormState(wormEnum_t wormN);
	void setWormState(wormEnum_t wormN, wormState_t state);
	void handleWormMovement(wormEnum_t worm, wormMoves_t direction);
	bool getLoopState(wormEnum_t wormN);
	void setLoopState(wormEnum_t wormN, bool setValue);


	void resetTicksFor(wormEnum_t wormN);
	void tickFor(wormEnum_t wormN);
	void setMoving(wormEnum_t wormN, bool setValue);

};