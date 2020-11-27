#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GL/glew.h"
#include "Texture.h"
#include "Mesh.h"
#include "TextureLoader.h"
#include <string>
#include "Math/float3.h"
class Model
{
public:
	Model(std::string path);

	void Draw(unsigned int shader);

	void LoadModel(std::string path);
	void ProcessNode(aiNode *node, const aiScene *scene);

	Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

	std::vector<Texture> GetTexturesLoaded() const
	{
		return texturesLoaded;
	};

	void ApplyTextureToModel(unsigned int id, const char *path);

	// TODO::Improve this
	float3 GetDimensions() const{
		return float3(dimensions.x,dimensions.y,dimensions.z);
	}
	

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texturesLoaded;
	// TextureLoader *textureLoader;


	aiVector3D boundBoxMin;
	aiVector3D boundBoxMax;

	// storing dimensions in the 3 axis
	aiVector3D dimensions;
	void CalculateBoxDimensions();
};
