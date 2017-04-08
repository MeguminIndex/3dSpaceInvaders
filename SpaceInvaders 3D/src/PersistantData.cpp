#include "SDL.h"
#include "PersistantData.h"
#include <iostream>


using namespace std;


PersistantData::PersistantData()
{

}

void PersistantData::loadfont(string path)
{

	font = TTF_OpenFont(path.c_str(),50);


}

GLuint PersistantData::ReturnTexture(string file)
{

	SDL_Surface* image = IMG_Load(file.c_str());
	if (!image)
	{
		SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, "IMG_Load error: %s\n", IMG_GetError());

	}
	
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	//texture paramiters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//applying image to the texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image->w, image->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	SDL_FreeSurface(image);

	return texture;
}

PersistantData::~PersistantData()
{

}