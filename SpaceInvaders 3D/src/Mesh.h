#ifndef MESH_H
#define MESH_H

#include "Mesh.h"
#include "ShaderProg.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <sstream>
#include <vector>


using namespace std;

struct Vertex {

	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;

};

struct Texture {
	GLuint id;
	string type;
	aiString path;
};

class Mesh
{
public:

	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;


	Mesh(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> textures);
	void Draw(ShaderProg* shaderProg);
	
private:
	/*  Render data  */
	GLuint VAO, VBO, EBO;
	/*  Functions    */
	void setupMesh();

};

#endif