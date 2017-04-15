#ifndef SHADERPROG_H
#define SHADERPROG_H


#include "ShaderProg.h"
#include <iostream>

#include <GL/glew.h>

#define GLM_FORCE_RADIANS // force glm to use radians
// // NOTE: must do before including GLM headers
// // NOTE: GLSL uses radians, so will do the same, for consistency
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class ShaderProg
{

public: 
	ShaderProg();

	GLuint shader;


private:
	const GLchar* vertexShaderSource = R"(
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

	const GLchar* fragmentShaderSource = R"(
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




};







#endif