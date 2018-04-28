#include "allegroFunctions.h"
#include <iostream>
#include "EventManagement.h"
#include "scenario.h"
#include "scenario.cpp"
#include "general.h"

using namespace std;

int main(void) 
{
	
	srand(time(NULL));

	ALLEGRO_DISPLAY * display = NULL;

	if ((!allegroInit()) && (display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT))){

		EventManagement eventHandler(display);
		scenario stage;


		if (stage.errorLoading || eventHandler.errorLoading) {
			eventHandler.finishGame();			//redundante pero nunca esta de mas.
			un_init_allegro();
			cout << "Allegro Error while creating scenario. Press any key to end the program." << endl; //Caso de error
			getchar();
		}
		else {
			while (!eventHandler.gameIsFinished()) {
				
				eventHandler.receiveEvent(stage);
				eventHandler.handleEvent(stage);
				if (!eventHandler.gameIsFinished() && eventHandler.shouldRedraw()) {
					stage.draw();
					al_flip_display();
				}
			}
			
			getchar();
			allegroShutdown(stage);
		}
	}
	else {
		cout << "Allegro initialization has failed. Press any key to end the program." << endl; //Caso de error
		getchar();
	}

	al_destroy_display(display);
	return 0;
}