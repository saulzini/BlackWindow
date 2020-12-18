#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GL/glew.h"
#include "Core/Mesh.h"
#include "Core/Importers/Texture/TextureLoader.h"
#include <string>
#include "Math/float3.h"
#include "Core/Texture.h"
#include "Core/GameObject/GameObject.h"
#include <unordered_map> 
namespace ModelImporter
{

	class Model
	{
	public:
		Model(std::string path,unsigned int program);

		GameObject* LoadModel();
		GameObject* ProcessNode(GameObject *parent,aiNode *node, const aiScene *scene);

		Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

		std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);

		std::vector<Mesh> GetMeshes(){
			return meshes;
		}
		
	private:
		std::vector<Mesh> meshes;
		std::string directory;
		std::string path;
		// std::vector<Texture> texturesLoaded;
		std::unordered_map<std::string, Texture> texturesLoaded; 

		aiVector3D boundBoxMin;
		aiVector3D boundBoxMax;

		// storing dimensions in the 3 axis
		aiVector3D dimensions;

		int animationsCount;
		int meshesCount;
		int materialsCount;
		int camerasCount;
		int lightsCount;
		int texturesCount;
		unsigned int program;
		void CalculateBoxDimensions();
	};
} // namespace ModelImporter
