
#include <iostream>
#include <string>
#include <stdlib.h> 

//#include <GLFW/glfw3.h>

// // GLEW - OpenGL Extension Wrangler - http://glew.sourceforge.net/
// // NOTE: include before SDL.h
#include <GL/glew.h>

// SDL - Simple DirectMedia Layer - https://www.libsdl.org/
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"

// // - OpenGL Mathematics - https://glm.g-truc.net/
 #define GLM_FORCE_RADIANS // force glm to use radians
// // NOTE: must do before including GLM headers
// // NOTE: GLSL uses radians, so will do the same, for consistency
 #include <glm/glm.hpp>
 #include <glm/gtc/matrix_transform.hpp>
 #include <glm/gtc/type_ptr.hpp>


#include <CoreWindow.h>

#include "Sprite.h"
#include "MainWorld.h"
#include "PersistantData.h"
#include "ShaderProg.h"

using namespace glm;

using namespace std;

void APIENTRY glDebugOutput(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar *message,
	const void *userParam)
{
	// ignore non-significant error/warning codes
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

	std::cout << "---------------" << std::endl;
	std::cout << "Debug message (" << id << "): " << message << std::endl;

	switch (source)
	{
	case GL_DEBUG_SOURCE_API:             std::cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   std::cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: std::cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     std::cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     std::cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           std::cout << "Source: Other"; break;
	} std::cout << std::endl;

	switch (type)
	{
	case GL_DEBUG_TYPE_ERROR:               std::cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: std::cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  std::cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         std::cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         std::cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              std::cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          std::cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           std::cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               std::cout << "Type: Other"; break;
	} std::cout << std::endl;

	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:         std::cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       std::cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          std::cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: std::cout << "Severity: notification"; break;
	} std::cout << std::endl;
	std::cout << std::endl;
}







int createWin(int w, int h, SDL_Window* &win)
{

 win = SDL_CreateWindow("OpenGL Workshop",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,w, h, SDL_WINDOW_OPENGL |SDL_WINDOW_RESIZABLE);

	SDL_assert(win);

	return 0;
}



void input(MainWorld* &world,PersistantData* &persData,vec3 &colourValueGLMVector,bool &running)
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{

		switch (e.type)
		{
		case SDL_WINDOWEVENT:
		{
			switch (e.window.event)
			{
			case SDL_WINDOWEVENT_RESIZED:
				SDL_LogDebug(SDL_LOG_CATEGORY_SYSTEM, "Window Resized");
				persData->windowResized = true;
				break;


			default:
				break;
			}

		}
		case SDL_KEYDOWN:
		//	if (!e.key.repeat)
		//	/{
				switch (e.key.keysym.scancode)
				{
				case SDL_SCANCODE_ESCAPE:
					running = false;
					break;

				case SDL_SCANCODE_F11:
					if (persData->fullscreen == false)
					{
						SDL_SetWindowFullscreen(persData->window, SDL_WINDOW_FULLSCREEN_DESKTOP);
						persData->fullscreen = true;
					}
					else
					{
						SDL_SetWindowFullscreen(persData->window, 0);
						persData->fullscreen = false;
					}
					break;

			

				case SDL_SCANCODE_A:
					world->player.direction = movementInput::Left;
					break;

			
				case SDL_SCANCODE_D:
					world->player.direction = movementInput::Right;
					break;

				case SDL_SCANCODE_F:

					world->playerFire = true;
					break;



				case SDL_SCANCODE_UP:
					world->camPosition += world->camUp*world->camSpeed;
					break;
				case SDL_SCANCODE_DOWN:
					world->camPosition -= world->camUp*world->camSpeed;
					break;
				case SDL_SCANCODE_RIGHT:
					world->camAngle = glm::radians(0.5f);
					world->cameraRotationMatrix = glm::rotate(world->cameraRotationMatrix, world->camAngle, glm::vec3(0.0f, 1.0f, 0.0f));
					break;
				case SDL_SCANCODE_LEFT:
					
					world->camAngle = glm::radians(-0.5f);
					world->cameraRotationMatrix = glm::rotate(world->cameraRotationMatrix, world->camAngle, glm::vec3(0.0f, 1.0f, 0.0f));
					break;
				case SDL_SCANCODE_W:
					world->camPosition += world->camLook * world->camSpeed;
					break;
				case SDL_SCANCODE_S:
					world->camPosition -= world->camLook * world->camSpeed;
					break;
				default:
					break;


				}

			//}
			break;

		case SDL_KEYUP:
			switch (e.key.keysym.scancode)
			{
		

			case SDL_SCANCODE_W:			
			case SDL_SCANCODE_A:		
			case SDL_SCANCODE_S:
			case SDL_SCANCODE_D:
				world->player.direction = movementInput::None;
				break;

			}
			break;



		}



		if (e.type == SDL_QUIT)
		{
			SDL_Log("Application Quit Requested");
			running = false;

		}

	}
}
void render(GLuint VAO, GLuint EBO,MainWorld* world,PersistantData* persData, vec3 &colourValueGLMVector, ShaderProg* shaderProgram)
{

	

	int numb = 3*12;
	// pre-render	
	glEnable(GL_BLEND);
	

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glClearColor(colourValueGLMVector.r, colourValueGLMVector.g, colourValueGLMVector.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	world->viewMatrix = glm::lookAt(world->camPosition, world->camPosition + world->camLook, world->camUp);

	// render
	glUseProgram(shaderProgram->shader);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


	GLint viewLocation = glGetUniformLocation(shaderProgram->shader, "viewMat");
	GLint projectionLocation = glGetUniformLocation(shaderProgram->shader, "projectionMat");
	//set the uniforms in the shader


	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(world->viewMatrix*world->cameraRotationMatrix));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(world->projectionMatrix));

	glBindTexture(GL_TEXTURE_2D, persData->backgroundTexture);//binds texture
	GLint modelLocation = glGetUniformLocation(shaderProgram->shader, "modelMat");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(world->background.modelMatrix*world->background.rotationMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	for (const auto &enermy : world->enermieSp)
	{
		if (enermy.render == true)
		{
			glBindTexture(GL_TEXTURE_2D, persData->enermieTexture);//binds texture
			GLint modelLocation2 = glGetUniformLocation(shaderProgram->shader, "modelMat");

			glUniformMatrix4fv(modelLocation2, 1, GL_FALSE, glm::value_ptr(enermy.modelMatrix*enermy.rotationMatrix));
			glDrawElements(GL_TRIANGLES, numb, GL_UNSIGNED_INT, 0);
		}

	}

	for (const auto &bullet : world->bullets)
	{

		if (bullet.dead == false)
		{

			glBindTexture(GL_TEXTURE_2D, persData->bulletTexture);//binds texture
			GLint modelLocation2 = glGetUniformLocation(shaderProgram->shader, "modelMat");

			glUniformMatrix4fv(modelLocation2, 1, GL_FALSE, glm::value_ptr(bullet.modelMatrix*bullet.rotationMatrix));
			glDrawElements(GL_TRIANGLES, numb, GL_UNSIGNED_INT, 0);
		}
	}

	for (const auto &bullet : world->enermieBullets)
	{

		if (bullet.dead == false)
		{

			glBindTexture(GL_TEXTURE_2D, persData->bulletTexture);//binds texture
			GLint modelLocation2 = glGetUniformLocation(shaderProgram->shader, "modelMat");

			glUniformMatrix4fv(modelLocation2, 1, GL_FALSE, glm::value_ptr(bullet.modelMatrix*bullet.rotationMatrix));
			glDrawElements(GL_TRIANGLES, numb, GL_UNSIGNED_INT, 0);
		}
	}
	
	for (const auto &blockade : world->barriers)
	{

		if (blockade.dead == false)
		{


			if(blockade.health >= 3)
			glBindTexture(GL_TEXTURE_2D, persData->barrierTexture);//binds texture
			else if(blockade.health == 2)
			glBindTexture(GL_TEXTURE_2D, persData->barrierTexture2);//binds texture
			else
			glBindTexture(GL_TEXTURE_2D, persData->barrierTexture3);//binds texture

			GLint modelLocation2 = glGetUniformLocation(shaderProgram->shader, "modelMat");

			glUniformMatrix4fv(modelLocation2, 1, GL_FALSE, glm::value_ptr(blockade.modelMatrix*blockade.rotationMatrix));

			/*GLint colorUniLocation = glGetUniformLocation(shaderProgram, "colorUni");

			vec3 color = {1.0f, 0.0f, 0.0f};

			glUniform3f(colorUniLocation, (GLfloat)color.r, (GLfloat)color.g, (GLfloat)color.b);*/

			glDrawElements(GL_TRIANGLES, numb, GL_UNSIGNED_INT, 0);
		}
	}

	glBindTexture(GL_TEXTURE_2D, persData->boundryTexture);//binds texture
	GLint boundryModelLocation = glGetUniformLocation(shaderProgram->shader, "modelMat");

	glUniformMatrix4fv(boundryModelLocation, 1, GL_FALSE, glm::value_ptr(world->sideWall.modelMatrix*world->sideWall.rotationMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

#pragma region text Rendering
	
	
	SDL_Color color;
	color.r = 255;
	color.b = 255;
	color.g = 255;
	string lives = "Lives: " + to_string(world->player.health);
	if (persData->gameOver == true)
	{
		lives = "GAME OVER!!!";

	}
	else if (persData-> gameWon == true)
	{
		lives = "You Win!";

	}
	SDL_Surface *texSurf = TTF_RenderUTF8_Blended(persData->font,lives.c_str(), color); //< Doesn't work
	GLuint fontTexture;
	glGenTextures(1, &fontTexture);
	glBindTexture(GL_TEXTURE_2D, fontTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSurf->w, texSurf->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, texSurf->pixels);


	glBindTexture(GL_TEXTURE_2D, fontTexture);//binds texture
	GLint modelLocationFont = glGetUniformLocation(persData->shaderPlayer->shader, "modelMat");
	glUniformMatrix4fv(modelLocationFont, 1, GL_FALSE, glm::value_ptr(world->LivesText.modelMatrix*world->LivesText.rotationMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


  
	glDeleteTextures(1, &fontTexture);

	//score text
	string score ="Score: " + to_string(world->player.points);

	texSurf = TTF_RenderUTF8_Blended(persData->font, score.c_str(), color);
	glGenTextures(1, &fontTexture);
	glBindTexture(GL_TEXTURE_2D, fontTexture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texSurf->w, texSurf->h, 0, GL_BGRA, GL_UNSIGNED_BYTE, texSurf->pixels);
	glBindTexture(GL_TEXTURE_2D, fontTexture);//binds texture
	GLint modelLocationScore = glGetUniformLocation(persData->shaderPlayer->shader, "modelMat");
	glUniformMatrix4fv(modelLocationFont, 1, GL_FALSE, glm::value_ptr(world->scoreText.modelMatrix*world->scoreText.rotationMatrix));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


	SDL_FreeSurface(texSurf);
	glDeleteTextures(1, &fontTexture);

#pragma endregion
	glUseProgram(persData->shaderPlayer->shader);
	viewLocation = glGetUniformLocation(persData->shaderPlayer->shader, "view");
	projectionLocation = glGetUniformLocation(persData->shaderPlayer->shader, "projection");
	//set the uniforms in the shader


	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(world->viewMatrix*world->cameraRotationMatrix));
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(world->projectionMatrix));

	//draw player
	if (world->player.dead == false)
	{

		GLint modelLocation = glGetUniformLocation(persData->shaderPlayer->shader, "modelMat");
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(world->player.modelMatrix*world->player.rotationMatrix));
		world->player.Draw(persData->shaderPlayer);
	}

	// post-render
	SDL_GL_SwapWindow(persData->window);

	
}

void update(MainWorld* &world, PersistantData* &persData, chrono::duration<double> t)
{
	if (persData->windowResized == true)
	{
		int w, h;
		SDL_GL_GetDrawableSize(persData->window, &w, &h);
		glViewport(0, 0, w, h);
		persData->windowResized = false;
	}

//	world->LivesText.modelMatrix = glm::translate(world->viewMatrix, glm::vec3(-1.5f, 0.0f, 5.0f));
////	world->LivesText.modelMatrix = glm::scale(world->LivesText.modelMatrix, glm::vec3(0.4f));
//
//	world->scoreText.modelMatrix = glm::translate(world->viewMatrix, glm::vec3(-1.5f, 0.6f, 5.0f));
//	//world->scoreText.modelMatrix = glm::scale(world->scoreText.modelMatrix, glm::vec3(0.4f));



	if (persData->gameOver == false && persData->gameWon == false)
	{

		float playerMoveSpeed = 4.0* t.count();
		float backgroundMovement = 0.05 * t.count();
		float mobMoveSpeedm = world->mobSpeed * t.count(); // need to have the space invaders speed up over time 

		world->mobSpeed += (0.1f * t.count());

		
		if (world->player.dead == false)
		{

			if (world->player.direction == movementInput::Left &&  world->player.modelMatrix[3].x > -3.0f + world->player.sizeH)
			{
				world->player.modelMatrix = glm::translate(world->player.modelMatrix, glm::vec3(-playerMoveSpeed, 0.00f, 0.0f));
				world->background.modelMatrix = glm::translate(world->background.modelMatrix, glm::vec3(backgroundMovement, 0.00f, 0.0f));


			}
			else if (world->player.direction == movementInput::Right  &&  world->player.modelMatrix[3].x < 3 - (0.1f))
			{
				world->player.modelMatrix = glm::translate(world->player.modelMatrix, glm::vec3(playerMoveSpeed, 0.00f, 0.0f));
				world->background.modelMatrix = glm::translate(world->background.modelMatrix, glm::vec3(-backgroundMovement, 0.00f, 0.0f));

			}

			if (world->playerFire == true && world->player.lastShot <= chrono::high_resolution_clock::now())
			{

				world->player.createBullet(world->bullets, 1.0f);
				world->player.lastShot += chrono::milliseconds(world->player.cooldownValue);


			}
			world->playerFire = false;

		//	cout << "player x pos: " << world->player.modelMatrix[3].x << endl;
		}
		

		float rotate = 0.1f *t.count();



		float bulletSpeed = 15 * t.count();
		for (auto &bullet : world->bullets)
		{
			bullet.modelMatrix = glm::translate(bullet.modelMatrix, glm::vec3(0.0f, bulletSpeed, 0.0f));
			bullet.rotationMatrix = glm::rotate(bullet.rotationMatrix, glm::radians(0.8f), glm::vec3(0.0f, 0.0f, 1.0f));

		}




		bool dropDown = false;
		//check non of them will hit edge

		for (auto &enermy : world->enermieSp)
		{
			if (enermy.dead == false)
			{


				glm::mat4 tmpmodel;
				switch (enermy.direction)
				{
				case movementInput::Right:
					//		SDL_Log("Right");
					tmpmodel = glm::translate(enermy.modelMatrix, glm::vec3(mobMoveSpeedm, 0.00f, 0.0f));
					break;
				case movementInput::Left:
					//		SDL_Log("Left");
					tmpmodel = glm::translate(enermy.modelMatrix, glm::vec3(-mobMoveSpeedm, 0.00f, 0.0f));
				}

				if (tmpmodel[3].x > 3 - 0.1f && enermy.direction == movementInput::Right || tmpmodel[3].x < -3 + enermy.sizeH && enermy.direction == movementInput::Left)
				{
					dropDown = true;
					break;
				}


			}
		
		}






		int nSDelay = 1;
		//update each enermy sprite position here  
		persData->gameWon = true;

		for (auto &enermy : world->enermieSp)
		{

			if (enermy.dead == false)
			{
				//if the enermy should drop down
				persData->gameWon = false;
				if (dropDown == true)
				{
					float mobMoveSpeedTmp;
					float ammount = 0.25;
					enermy.modelMatrix = glm::translate(enermy.modelMatrix, glm::vec3(0.0f, -0.1f, 0.0f));
					switch (enermy.direction)
					{


					case movementInput::Right:
						enermy.direction = movementInput::Left;
						mobMoveSpeedTmp = 0 - ammount;
						break;
					case movementInput::Left:
						enermy.direction = movementInput::Right;
						mobMoveSpeedTmp = ammount;
						break;


					}

					enermy.modelMatrix = glm::translate(enermy.modelMatrix, glm::vec3(0.0f, 0.00f, 0.0f));


					if (enermy.modelMatrix[3].y < 0.8)
					{
						persData->gameOver = true;
						world->player.dead = true;
						cout << "Aliens got too far, GameOver" << endl;
					}
				}





				switch (enermy.direction)
				{
				case movementInput::Right:
					//		SDL_Log("Right");
					enermy.modelMatrix = glm::translate(enermy.modelMatrix, glm::vec3(mobMoveSpeedm, 0.00f, 0.0f));


					break;
				case movementInput::Left:
					//		SDL_Log("Left");
					enermy.modelMatrix = glm::translate(enermy.modelMatrix, glm::vec3(-mobMoveSpeedm, 0.00f, 0.0f));
					break;
				}


			//	enermy.rotationMatrix = glm::rotate(enermy.rotationMatrix, glm::radians(0.8f), glm::vec3(0.0f, 0.0f, 1.0f));

				srand(time(NULL));
				int rng = rand() % 3 + 1;

				if (enermy.lastShot < chrono::high_resolution_clock::now())
				{

					enermy.createBullet(world->enermieBullets, -1.0f);

					enermy.lastShot += chrono::milliseconds(enermy.cooldownValue*(rng*nSDelay));
				}

				nSDelay++;

				
				for (auto &bullet : world->bullets)
				{
					if (bullet.dead == false)
					{

						if (enermy.checkcollision(bullet.modelMatrix[3].x, bullet.modelMatrix[3].y, bullet.sizeH, bullet.sizeH) == true)
						{

							//cout << "Collision Happened with bullet and mob" << endl;

							bullet.dead = true;

							enermy.health -= 1;
							if (enermy.health <= 0)
							{
								world->player.points += 1;
								cout << "Player Points: " << world->player.points <<endl;
								enermy.dead = true;
								enermy.deathTime = chrono::high_resolution_clock::now() + chrono::milliseconds(3000);
							}
						}
						
					}
				}


			}
			else if(enermy.deathTime >= chrono::high_resolution_clock::now())
			{
				enermy.rotationMatrix = glm::rotate(enermy.rotationMatrix, glm::radians(2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else
			{
				enermy.render = false;
			}
			

		}



		for (auto &bullet : world->enermieBullets)
		{
			if (bullet.dead == false)
			{
				bullet.modelMatrix = glm::translate(bullet.modelMatrix, glm::vec3(0.0f, -bulletSpeed, 0.0f));

				if (world->player.dead == false)
				{


					if (bullet.checkcollision(world->player.modelMatrix[3].x, world->player.modelMatrix[3].y, world->player.sizeH, world->player.sizeH) == true)
					{

						//cout << "Collision Happened with bullet and mob" << endl;

						bullet.dead = true;

						world->player.health -= 1;
						cout << "Player took dmg current lives is: " << world->player.health <<endl;
						if (world->player.health <= 0)
						{
							world->player.dead = true;
							persData->gameOver = true;
							cout << "Player Died: GAME OVER" << endl;
						}
					}
				}

			}
		}


		for (auto &blockade : world->barriers)
		{
			if (blockade.dead == false)
			{

				for (auto &bullet : world->bullets)
				{
					if (bullet.dead == false)
					{

						if (blockade.checkcollision(bullet.modelMatrix[3].x, bullet.modelMatrix[3].y, bullet.sizeH, bullet.sizeH) == true)
						{

							//cout << "Collision Happened with bullet and mob" << endl;

							bullet.dead = true;

							blockade.health -= 1;
							if (blockade.health <= 0)
							{
								blockade.dead = true;

							}
						}

					}
				}

				for (auto &bullet : world->enermieBullets)
				{
					if (bullet.dead == false)
					{

						if (blockade.checkcollision(bullet.modelMatrix[3].x, bullet.modelMatrix[3].y, bullet.sizeH, bullet.sizeH) == true)
						{

							//cout << "Collision Happened with bullet and mob" << endl;

							bullet.dead = true;

							blockade.health -= 1;
							if (blockade.health <= 0)
							{
								blockade.dead = true;

							}
						}

					}
				}
			}
		}

		//delete the dead bullets

		//look at maby capping a fixed size and reusing entries rather than endlessly adding then removeing which cases an entire reshuffle
		for (auto i = std::begin(world->bullets); i != std::end(world->bullets);)
		{

			if ((*i).dead == true)
			{

				i = world->bullets.erase(i);
			}
			else
			{
				++i;
			}


		}

		for (auto i = std::begin(world->enermieBullets); i != std::end(world->enermieBullets);)
		{

			if ((*i).dead == true)
			{

				i = world->enermieBullets.erase(i);
			}
			else
			{
				++i;
			}


		}


		//reset dropdown
		//if(dropDown == true)
		//	dropDown = false;


		


	}
	else
	{
		if (persData->jusEnded == true)
		{
			world->LivesText.modelMatrix = glm::translate(world->LivesText.modelMatrix, glm::vec3(5.0f, -1.0f, 0.0f));
			world->LivesText.modelMatrix = glm::scale(world->LivesText.modelMatrix, glm::vec3(2.0f));

			persData->jusEnded = false;
		}
	}
}

int initialise(SDL_Window *win,SDL_GLContext &glcontext, GLint width, GLint height)
{
	// Set all the required options for OGL
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_PROFILE_CORE);

	glcontext = SDL_GL_CreateContext(win);

	glewExperimental = GL_TRUE;
	//initialize GLEW
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "Glew init error %s\n", glewGetErrorString(glewError));
		return 1;
	}
	SDL_Log("glewInit initialised OK!\n");

	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
	{
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}


	glViewport(0, 0, width, height);


	// SDL_Image initialise
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
	if (!(IMG_Init(imgFlags) & imgFlags)) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_image init error: %s\n",
			IMG_GetError());
		return 1;
	}
	SDL_Log("SDL_image initialised OK!\n");

	// SDL_ttf initialise
	if (TTF_Init() == -1) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_ttf init error: %s\n",
			SDL_GetError());
		return 1;
	}
	SDL_Log("SDL_ttf initialised OK!\n");

	// SDL_mixer initialise
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_mixer init error: %s\n   PERHAPS NO "
			"HEADPHONES/SPEAKERS CONNECTED\n",
			Mix_GetError());
		return 1;
	}
	SDL_Log("SDL_mixer initialised OK!\n");

	return 0;
}

int main(int argc, char *argv[]) {

	PersistantData* persData = new PersistantData();
	
#pragma region initialise
	// SDL initialise
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_Init Error: %s\n",
			SDL_GetError());
		return 1;
	}
	SDL_Log("SDL initialised OK!\n");


	SDL_DisplayMode currentMode;


	SDL_GetDesktopDisplayMode(0, &currentMode);//set to 0 since only want the first display could use a viariable and for loop to get multiple.


	
	// Window Creation
	GLint width = currentMode.w/2, height = currentMode.h/2;
	//SDL_Window *win = nullptr;
	// win = SDL_CreateWindow("Hello World", 100, 100, 800, 600, 0);

	createWin(width, height,persData->window);
	
	if (persData->window == nullptr) {
		SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION,
			"SDL_CreateWindow init error: %s\n", SDL_GetError());
		return 1;
	}
	
	
	SDL_SetWindowPosition(persData->window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);//centers window (incase not ddefult centered)

	SDL_SetWindowTitle(persData->window,"Wesley Kyron Richard Beck CGP2012M-1617  15576489");

	SDL_GLContext glcontext = SDL_GL_CreateContext(persData->window);

	int code = initialise(persData->window, glcontext, width, height);
	if (code != 0)
	{
		return 1;
	}
#pragma endregion

	glEnable(GL_DEPTH_TEST);
  

	vec3 colourValueGLMVector = vec3(0.20f, 0.29f, 0.33f);


  GLfloat vertices[] = {
	  // Positions          // Colors           // Texture Coords
	  //front face
	  0.0f,  1.0f, 0.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top Right
	  0.0f, -0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
	  -1.0f, -0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
	  -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,  // Top Left 	 
	  //back face
	  0.0f,  1.0f, -0.5f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, // Top Right
	  0.0f, -0.0f, -0.5f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
	  -1.0f, -0.0f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
	  -1.0f,  1.0f, -0.5f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,  // Top Left 	 





  };

	


  GLuint indices[] = {  // Note that we start from 0!
	  //front face
	  0, 1, 3, // First Triangle
	  1, 2, 3,  // Second Triangle
	  //backface
	  4, 5, 7, // First Triangle
	  5, 6, 7,  // Second Triangle
	  //right face
	  0,4,5,
	  0,1,5,
	  //left face
	  3,7,6,
	  3,2,6,
	  //bottom face
	  2,1,5,
	  2,6,5,
	  //top face
	  3,0,4,
	  3,7,4

	 
  };

#pragma region GLBuffers/Shaders ect..
  //try finding reading from a seperate file

  //uniform vec3 colorUni;
 // const GLchar* vertexShaderSource = R"(
	//	#version 430 core
 // 		layout (location = 0) in vec3 position;
	//	layout (location = 1) in vec3 color;
	//	layout (location = 2) in vec2 texCoord;

	//	out vec3 ourColor;
	//	out vec2 TexCoord;
	//	
	//	uniform mat4 modelMat;
	//	uniform mat4 viewMat;
	//	uniform mat4 projectionMat;

	//	

	//	void main()
	//	{
	//		gl_Position = projectionMat * viewMat * modelMat * vec4(position.x, position.y, position.z, 1.0);
	//		ourColor = color;
	//		TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
	//	}
 // )";

 // const GLchar* fragmentShaderSource = R"(
	//#version 430 core
	//in vec3 ourColor;
	//in vec2 TexCoord;

	//out vec4 color;
	//


 //   uniform sampler2D ourTexture;	

	//void main()
	//{
	//  color = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f) ;
	//}

	//)";

 // // Vertex shader
 // GLuint vertexShader;
 // vertexShader = glCreateShader(GL_VERTEX_SHADER);
 // glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
 // glCompileShader(vertexShader);

 // //check for errors
 // GLint success;//variable to store success state
 // GLchar infoLog[512];//stores error msg
 // glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//gets succes status
 // if (!success)
 // {
	//  glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	//  std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILD\n" << infoLog << std::endl;


 // }

 // // Fragment shader
 // GLuint fragmentShader;
 // fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
 // glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
 // glCompileShader(fragmentShader);

 // // check for errors
 // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//gets succes status
 // if (!success)
 // {
	//  glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	//  std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILD\n" << infoLog << std::endl;


 // }

 // // Link Shaders
 // GLuint shaderProgram;
 // shaderProgram = glCreateProgram();//creating program and returns id of it
	//								//attaching and linking shaders
 // glAttachShader(shaderProgram, vertexShader);
 // glAttachShader(shaderProgram, fragmentShader);
 // glLinkProgram(shaderProgram);

 // //check if it failed/succeded
 // glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
 // if (!success)//if fails
 // {
	//  // glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
 // }


 // glDeleteShader(vertexShader);//deltes the shader object now that we dont need it
 // glDeleteShader(fragmentShader);
 // //  glDeleteShader(fragmentShader2);

  GLuint VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  //OpenGL Buffers
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  //Bind Vertex Array Object
  glBindVertexArray(VAO);

  // Copy our vertices array in a buffer for OpenGL to use
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // Position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
  glEnableVertexAttribArray(0);
  // Color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);

  //texture attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);

  //Element Buffer object
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


  //unbind array buf
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  //unbind element array buff
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  //Unbind the VAO
  glBindVertexArray(0);

#pragma endregion

  
  persData->loadfont("assets\\cour.ttf");

#pragma region MainShaderSource
  const GLchar* vertexShaderSource1 = R"(
		#version 430 core
  		layout (location = 0) in vec3 position;
		layout (location = 1) in vec3 color;
		layout (location = 2) in vec2 texCoord;

		out vec3 ourColor;
		out vec2 TexCoord;
		
		uniform mat4 modelMat;
		uniform mat4 viewMat;
		uniform mat4 projectionMat;

		

		void main()
		{
			gl_Position = projectionMat * viewMat * modelMat * vec4(position.x, position.y, position.z, 1.0);
			ourColor = color;
			TexCoord = vec2(texCoord.x, 1.0f - texCoord.y);
		}
  )";

  const GLchar* fragmentShaderSource1 = R"(
	#version 430 core
	in vec3 ourColor;
	in vec2 TexCoord;

	out vec4 color;
	


    uniform sampler2D ourTexture;	

	void main()
	{
	  color = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0f) ;
	}

	)";
#pragma endregion

#pragma region PlayerShaderSource
  const GLchar* playerVertexShaderSource = R"(
		#version 430 core
  		layout (location = 0) in vec3 position;
		layout (location = 1) in vec3 normal;
		layout (location = 2) in vec2 texCoords;

		out vec2 TexCoords;

		uniform mat4 modelMat;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
		  gl_Position = projection * view * modelMat * vec4(position.x, position.y, position.z, 1.0f);
		  TexCoords = texCoords;
		}
  )";

  const GLchar* playerFragmentShaderSource = R"(
	#version 430 core
	in vec2 TexCoords;

	out vec4 color;

	uniform sampler2D texture_diffuse1;

	void main()
	{    
		color = vec4(texture(texture_diffuse1, TexCoords)) * vec4(1,1,1,1);
	}

	)";
#pragma endregion


  persData->shader = new ShaderProg(vertexShaderSource1, fragmentShaderSource1);

  persData->shaderPlayer = new ShaderProg(playerVertexShaderSource, playerFragmentShaderSource);

  MainWorld* world = new MainWorld();

  //player
 world->player = Sprite();
 world->player.sizeH = 0.2f;
 world->player.modelMatrix = glm::translate(world->player.modelMatrix, glm::vec3(0.0f, -1.3f, 0.0f));
 world->player.modelMatrix = glm::scale(world->player.modelMatrix, glm::vec3(0.1f));
 world->player.health = 3;
// world->player.loadModel("assets/blockycharacters/Models/advancedCharacter.fbx");
 world->player.loadModel("assets/Nanosuit2/nanosuit2.obj");
// world->player.cooldownValue = 500;

//background
 world->background = Sprite();
 world->background.modelMatrix = glm::translate(world->background.modelMatrix, glm::vec3(15.0f, -6.0f, -3.0f));
 world->background.modelMatrix = glm::scale(world->background.modelMatrix, glm::vec3(38.0f,24.0f,1.0f));
 
 // world->background.rotationMatrix = glm::rotate(world->background.rotationMatrix, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
 //barriers
 for (float i=-2; i< 3;)
 {
	 Sprite TempBarrier;
	 TempBarrier.health = 3;
	 TempBarrier.modelMatrix = glm::translate(TempBarrier.modelMatrix, glm::vec3(i, -1.0f, 0.0f));
	 TempBarrier.modelMatrix = glm::scale(TempBarrier.modelMatrix, glm::vec3(0.2f));
	 TempBarrier.sizeH = 0.2f;
	 world->barriers.push_back(TempBarrier);
	 i += 0.5f;
 }

 //side walls
 

 world->sideWall.modelMatrix = glm::translate(world->sideWall.modelMatrix, glm::vec3(10.5f, -5.6f, -2.7f));

 world->sideWall.modelMatrix = glm::scale(world->sideWall.modelMatrix, glm::vec3(21.5f, 14.0f, 1.0f));



  persData->playerTexture = persData->ReturnTexture("assets\\player.png");
  persData->enermieTexture = persData->ReturnTexture("assets\\enermy2.png");
  persData->bulletTexture = persData->ReturnTexture("assets\\bullet.png");
  persData->backgroundTexture = persData->ReturnTexture("assets\\background.png");
  persData->barrierTexture = persData->ReturnTexture("assets\\blocksmall.png");
  persData->barrierTexture2 = persData->ReturnTexture("assets\\blocksmall2.png");
  persData->barrierTexture3 = persData->ReturnTexture("assets\\blocksmall3.png");

  persData->boundryTexture = persData->ReturnTexture("assets\\boundry.png");



  world->setUpEnermies(4);

  world->LivesText.modelMatrix = glm::translate(world->LivesText.modelMatrix, glm::vec3(-1.5f, 0.0f, 0.0f));
  world->LivesText.modelMatrix = glm::scale(world->LivesText.modelMatrix, glm::vec3(0.4f));

  world->scoreText.modelMatrix = glm::translate(world->scoreText.modelMatrix, glm::vec3(-1.5f, 0.6f, 0.0f));
  world->scoreText.modelMatrix = glm::scale(world->scoreText.modelMatrix, glm::vec3(0.4f));
 

  
  SDL_GetWindowSize(persData->window, &width, &height);

	//left, right, bottom, top, near clip plane, far clip plane
 // projectionMatrix = glm::ortho(0.0f, 4.0f, 0.0f, 3.0f, -1.0f, 100.0f);

  world->projectionMatrix = glm::perspective(glm::radians(100.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

  world->viewMatrix = glm::translate(world->viewMatrix, glm::vec3(0.0f, 0.0f, 0.0f));


 

  world->test.loadModel("assets\\cube.obj");
  world->test.modelMatrix = glm::translate(world->test.modelMatrix, glm::vec3(0.0f, -1.75f, 0.0f)); // Translate it down a bit so it's at the center of the scene
  world->test.modelMatrix = glm::scale(world->test.modelMatrix, glm::vec3(0.2f, 0.2f, 0.2f));	// It's a bit too big for our scene, so scale it down
 

  SDL_Log("Game Loop Started");

  bool running = true;


  chrono::milliseconds skipTicks(1000 / 60);

  chrono::high_resolution_clock::time_point next_Game_Tick = chrono::high_resolution_clock::now();

  chrono::duration<double> lastdt = chrono::milliseconds(0);
  while (running == true)
  {
	 
	  chrono::high_resolution_clock::time_point t = chrono::high_resolution_clock::now();
	
	  //input
	  input(world,persData,colourValueGLMVector,running);

	  //updating 
	  update(world,persData,lastdt);

	  if (chrono::high_resolution_clock::now() > next_Game_Tick)
	  {
		  //rendering
		

		  render(VAO,EBO,world,persData,colourValueGLMVector, persData->shader);

		  next_Game_Tick += skipTicks;
	  }


	  chrono::high_resolution_clock::time_point dt = chrono::high_resolution_clock::now();
	  lastdt = chrono::duration_cast<chrono::duration<double>>(dt - t);



  }
  // Exit Game loop


  //de-allocate resorces
  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  SDL_Quit();

  //Program Exits
  return 0;
}
