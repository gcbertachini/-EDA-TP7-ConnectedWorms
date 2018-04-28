#include <iostream>
#include "allegroFunctions.h"
#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\bitmap.h>
#include <allegro5\allegro_primitives.h>
#include "general.h"

using namespace std;



bool allegroInit(void) {

	bool abort = false;

	/*Si hay un error, ponemos abort en true de modo tal que la primer condición "abort == false"
	resulte falsa y no compruebe la segunda (Que es la inicialización correcta), aprovechando 
	el hecho de que C++ es lazy para no continuar inicializando tras un error, además de devolver abort e indicar el error. */

	if (!al_init()) {

		cout << "ERROR: Could not initialize Allegro." << endl;
		abort = true;

	}

	if ((abort == false) && !(al_init_primitives_addon())) {

		cout << "ERROR: Could not initialize Allegro Primitives." << endl;
		abort = true;

	}

	if ((abort == false) && !(al_install_keyboard())) {

		cout << "ERROR: Could not install keyboard." << endl;
		al_shutdown_primitives_addon();
		abort = true;

	}

	if ((abort == false) && !(al_init_image_addon())) {

		cout << "ERROR: Could not initialize image addons." << endl;
		al_shutdown_primitives_addon();
		al_uninstall_system();
		abort = true;

	}
	

	return abort;

}

void un_init_allegro(void) {

	al_shutdown_image_addon();
	al_shutdown_primitives_addon();
	al_uninstall_system();

}

void allegroShutdown(scenario& stage)
{

	al_shutdown_primitives_addon();
	al_shutdown_image_addon();
	al_uninstall_system();

}