#include "ShaderProg.h"


ShaderProg::ShaderProg()
{
	

	// Vertex shader
	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//check for errors
	GLint success;//variable to store success state
	GLchar infoLog[512];//stores error msg
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);//gets succes status
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILD\n" << infoLog << std::endl;


	}

	// Fragment shader
	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	// check for errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);//gets succes status
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILD\n" << infoLog << std::endl;


	}

	// Link Shaders
	
	shader = glCreateProgram();//creating program and returns id of it
									  //attaching and linking shaders
	glAttachShader(shader, vertexShader);
	glAttachShader(shader, fragmentShader);
	glLinkProgram(shader);

	//check if it failed/succeded
	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (!success)//if fails
	{
		// glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	}


	glDeleteShader(vertexShader);//deltes the shader object now that we dont need it
	glDeleteShader(fragmentShader);
}