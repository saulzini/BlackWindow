#pragma once
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "GL/glew.h"
#include "Core/Importers/Texture/TextureLoader.h"
#include <string>
#include "Math/float3.h"
#include "Core/Texture.h"
#include "Core/GameObject/GameObject.h"
#include <unordered_map> 
#include "Math/float4x4.h"
#include "Core/Importers/Model/ProcessedMesh.h"
namespace ModelImporter
{

	class Model
	{
	private:
		std::string directory;
		std::string path;

		float3 boundBoxMin;
		float3 boundBoxMax;

		// storing dimensions in the 3 axis
		float3 dimensions;

		int animationsCount;
		int meshesCount;
		int materialsCount;
		int camerasCount;
		int lightsCount;
		int texturesCount;
		unsigned int program;
	public:
		Model(const std::string &path,unsigned int program);

		GameObject* LoadModel();
		GameObject* ProcessNode(GameObject *parent,aiNode *node, const aiScene *scene);

		ProcessedMesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

		unsigned int LoadMaterialTexture(aiMaterial *mat, aiTextureType type);


		float3 GetDimensions(){
			return dimensions;
		}
		
	private:
		void CalculateBoxDimensions();
	};
} // namespace ModelImporter
