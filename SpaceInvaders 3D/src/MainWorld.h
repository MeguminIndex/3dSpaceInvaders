#ifndef MAINWORLD_H
#define MAINWORLD_H

#include "SDL.h"
#include "MainWorld.h"
#include "Sprite.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <iostream>

using namespace std;


class MainWorld
{
public:
	//this class stores alot of the data used in my game.
	

	
	MainWorld();
	void setUpEnermies(float entitieSize);

	~MainWorld();


	
	bool playerFire;
	bool rorate = false;

	float mobSpeed = 0.4f;
	
	vector<Sprite> enermieSp;
	vector<Sprite> bullets;
	vector<Sprite> enermieBullets;
	vector<Sprite> barriers;

	Sprite sideWall;

	Sprite background;

	Sprite player;
	

	Sprite test;

	Sprite LivesText;
	Sprite scoreText;

	glm::mat4 viewMatrix;

	glm::mat4 projectionMatrix;

	glm::mat4 cameraRotationMatrix;

	//set up camera variables
	//Position = 3 units along the positive z axis
	glm::vec3 camPosition = glm::vec3(0.0f, 0.0f, 3.0f);
	//Look at from that position = (0,0,-1)
	glm::vec3 camLook = glm::vec3(0.0f, 0.0f, -1.0f);
	//set the Up axis fro the camera
	glm::vec3 camUp = glm::vec3(0.0f, 1.0f, 0.0f);


	bool increaseLightDist = false;
	bool decreaseLightDist = false;
	GLfloat lightDist = 0.0f;
	//angle of the camera
	GLfloat camAngle = 0.5f;

	//value for the camera speed of movement
	GLfloat camSpeed = 0.1f;






};


#endif