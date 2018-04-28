#pragma once
#include "general.h"

typedef int(*pCallback) (char * key, char * value, void * userdata);

int parseCallback(char *key, char *value, void *userData);
