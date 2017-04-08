#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"
#include "Sprite.h"
#include "SDL_image.h"

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <chrono>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

enum class movementInput { Up, Down, Left, Right, None };


class Sprite
{

public:
	Sprite();
	~Sprite();
	
	bool checkcollision(const float otherX, const float otherY,  float otherWidth,  float otherHeight);
	void createBullet(vector<Sprite> &list, float direction);
	
	bool render=true;

	int health;
	
	float sizeH;
	
	int points=0;

	glm::mat4 modelMatrix;
	glm::mat4 rotationMatrix;

	bool dead;

	int cooldownValue = 1000;

	

	chrono::high_resolution_clock::time_point lastShot = chrono::high_resolution_clock::now();

	chrono::high_resolution_clock::time_point deathTime;

	movementInput direction = movementInput::None;
};
#endif // !#include "Sprite.h"