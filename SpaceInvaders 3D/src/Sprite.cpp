
#include "Sprite.h"

using namespace std;

	Sprite::Sprite()
	{

		//corrects position onto camera
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 1.5f));
		dead = false;
		//modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f));
	}


	void Sprite::createBullet(vector<Sprite> &list, float direction)
	{
		Sprite TmpBullet;
		float scale = 0.2f;
		TmpBullet.modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, direction, 0.0f));
		//TmpBullet.modelMatrix = glm::scale(TmpBullet.modelMatrix, glm::vec3(scale));
		TmpBullet.sizeH = 1 * scale;
		list.push_back(TmpBullet);
	}

	bool Sprite::checkcollision(const float otherX, const float otherY,  float otherWidth,  float otherHeight)
	{
	
		float thisRight, thisBottom;
		float bottomOther, rightOther;

		thisRight = modelMatrix[3].x + sizeH;

		thisBottom = modelMatrix[3].y + sizeH;
		
		rightOther = otherX + otherWidth;

		bottomOther = otherY +otherHeight;

	//	cout << otherHeight << endl;

		//cout <<"ThisX: " << modelMatrix[3].x << "BulletX: " <<otherX<<endl;

		//if state ments return false when this sprite is otside the rect of the sprite being checked against
		if (thisBottom <= otherY)
		{
			return false;//if bottom of this is above the top of the other sprite
		}
	

		if (modelMatrix[3].y >= bottomOther)
		{
			return false;// if top of this is below the bottom of the other sprite
		}
		
		if (thisRight <= otherX)
		{
			return false;//if the right side of this is left of the other sprites left side
		}
	

		if (modelMatrix[3].x >= rightOther)
		{
			return false;//if this sprite is right of the righ side othe the other sprite
		}
		

		//gets here if all of the statements are false

		return true;// returns true signifying collision has happened
		

	}

	Sprite::~Sprite()
	{
	
		
		//SDL_Log("Sprite destroyed");

	}