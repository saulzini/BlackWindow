#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GL/glew.h"
#include "Texture.h"
#include "Mesh.h"
GLint TextureFromFile(const char* path, std::string directory);
class Model
{
public:
	Model(const char* path) {
		if (path == "") {
			return;
		}
		this->LoadModel(path);
	};


	void Draw(unsigned int shader);
	
	void LoadModel(const char* path);
	void ProcessNode(aiNode* node, const aiScene* scene);

	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;

};

