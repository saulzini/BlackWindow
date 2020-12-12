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
namespace ModelImporter
{

	class Model
	{
	public:
		Model(std::string path);

		void LoadModel(std::string path);
		void ProcessNode(aiNode *node, const aiScene *scene);

		Mesh ProcessMesh(aiMesh *mesh, const aiScene *scene);

		std::vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, const std::string &typeName);

	private:
		std::vector<Mesh> meshes;
		std::string directory;
		std::vector<Texture> texturesLoaded;

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
		void CalculateBoxDimensions();
	};
} // namespace ModelImporter
