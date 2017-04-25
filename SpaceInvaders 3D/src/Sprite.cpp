
#include "Sprite.h"

using namespace std;

	Sprite::Sprite()
	{

		//corrects position onto camera
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, 1.5f));
		dead = false;
		//modelMatrix = glm::scale(modelMatrix, glm::vec3(10.0f));
	}

	Sprite::Sprite(float x,float y,float z)
	{
		modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
		dead = false;
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






	void Sprite::Draw(ShaderProg* shader)
	{
		for (GLuint i = 0; i < this->meshes.size(); i++)
			this->meshes[i].Draw(shader);

	}



	void  Sprite::loadModel(string path)
	{
		// Read file via ASSIMP
		Assimp::Importer importer;

		//aiProcess_Triangulate makes all faces triangles if not already
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}
		// Retrieve the directory path of the filepath
		directory = path.substr(0, path.find_last_of('\\'));

		// go through  ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);
	}


	void Sprite::processNode(aiNode* node, const aiScene* scene)
	{
		// Process all the node's meshes in the scene object
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// Then do the same for each of its children
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}



	}


	Mesh Sprite::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		//the mesh data
		vector<Vertex> vertices;
		vector<GLuint> indices;
		vector<Texture> textures;

		//for each of the mesh verticies do this
		for (GLuint i =0; i< mesh->mNumVertices; i++)
		{
			Vertex thisVertex;// this will stores the vertext data of this mesh

			glm::vec3 tmpVector;

			// Grabs the positions from the mesh and store into my tmp vec3
			tmpVector.x = mesh->mVertices[i].x;
			tmpVector.y = mesh->mVertices[i].y;
			tmpVector.z = mesh->mVertices[i].z;
			thisVertex.Position = tmpVector;//setting the position in my vertext struct
			// grabbing infomation of mesh normals
			
			tmpVector.x = mesh->mNormals[i].x;
			tmpVector.y = mesh->mNormals[i].y;
			tmpVector.z = mesh->mNormals[i].z;
			thisVertex.Normal = tmpVector;//updating my vertex struct 

			//checking if mesh has texture cords
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 tmpTexVec;//create a vec2 to store texture cords in temp
				//grab x and y cord
				tmpTexVec.x = mesh->mTextureCoords[0][i].x;
				tmpTexVec.y = mesh->mTextureCoords[0][i].y;
				thisVertex.TexCoords = tmpTexVec;//update vertex struct with texture cords
			}
			else//theres no texture cords
			{
				thisVertex.TexCoords = glm::vec2(0.0f,0.0f);
				
			}

			vertices.push_back(thisVertex);

		}

		//iterate each face in the model
		for (GLuint i =0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			//for each indices in the face
			for (GLuint j = 0; j < face.mNumIndices; j++)
			{
				//push the indicies into the variable created eariler
				indices.push_back(face.mIndices[j]);
			}


		}


		//check if mesh has material
		if (mesh->mMaterialIndex >=0)
		{
			//get material object
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

			vector<Texture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			vector<Texture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		}


		// Return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

	vector<Texture> Sprite::loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			GLboolean skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip)
			{   // If texture hasn't been loaded already, load it
				Texture texture;
				texture.id = LoadTexture(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // Add to loaded textures
			}
		}
		
		cout << directory << endl;

		return textures;
	}

	GLint Sprite::LoadTexture(const char* path, string directory)
	{

		string filename = string(path);
		//filename = "assets//" + filename;
		filename = directory + '/' + filename;
		cout << "File Name " + filename << endl;
		SDL_Surface* image = IMG_Load(filename.c_str());
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

	Sprite::~Sprite()
	{
	
		
		//SDL_Log("Sprite destroyed");

	}