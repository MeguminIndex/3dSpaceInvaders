#ifndef GAMESETUP_H
#define GAMESETUP_H

#include "gameSetUp.h"
#include "SDL.h"
#include "SDL_mixer.h"


#include "PersistantData.h"
#include "MainWorld.h"
//#include "fontManager.h"


#include <iostream>
class gameSetUp
{
public:
	gameSetUp();
void setGameUp(PersistantData* persData, MainWorld* world);
	~gameSetUp();
};

#endif