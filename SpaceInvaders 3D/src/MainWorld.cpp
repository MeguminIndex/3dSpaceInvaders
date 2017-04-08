
#include "SDL.h"
#include "MainWorld.h"



using namespace std;

MainWorld::MainWorld()
{
	SDL_LogDebug(SDL_LOG_PRIORITY_DEBUG, "MainWorld Constructed");

	bullets.reserve(10);
	enermieBullets.reserve(10);

}

void MainWorld::setUpEnermies(float entitieSize)
{
	float sizeM = 0.2f;
	float x = -1.8f + sizeM + 0.05, y= 1.5 - (sizeM + 0.01f);
	
	for (float i = y; i > 1;)
	{
		for (float j = x; j < 2-sizeM;)
		{
		
			Sprite tmpSprite;
			tmpSprite.sizeH = 1 * sizeM;
			tmpSprite.health = 1;
			tmpSprite.modelMatrix = glm::translate(tmpSprite.modelMatrix, glm::vec3(j, i, 0.0f));
			tmpSprite.modelMatrix = glm::scale(tmpSprite.modelMatrix, glm::vec3(sizeM));
			tmpSprite.points = 1;

			tmpSprite.lastShot += chrono::milliseconds(int(tmpSprite.cooldownValue*(i*j)) + 8000);
			tmpSprite.direction = movementInput::Left;//mobs start off going left
			
			enermieSp.push_back(tmpSprite);

			j += sizeM + 0.04f;
			
		}

		//x = -1.65;
		i -= sizeM +0.04f;



	}


}

MainWorld::~MainWorld()
{
	SDL_LogDebug(SDL_LOG_PRIORITY_DEBUG, "MainWorld De-Constructed");


}



