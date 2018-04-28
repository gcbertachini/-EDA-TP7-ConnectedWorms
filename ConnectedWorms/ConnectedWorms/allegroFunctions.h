#pragma once
#include "scenario.h"

bool allegroInit(void); //Funci�n encargada de inicializar Allegro en el programa. Devuelve false si no hay error, true si lo hay.
void un_init_allegro(void); //Funci�n encargada de desinstalar Allegro en el programa en caso de error.
void allegroShutdown(scenario& stage); //Funci�n encargada de destruir Allegro al finalizar el programa.