#include "Model.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include "Globals.h"
#include "Core/Vertex.h"
#include "Core/Importers/Texture/TextureLoader.h"
#include "BoundingBox.h"
#include "Core/Components/ComponentMesh.h"
#include "Core/Components/ComponentMaterial.h"
#include "Core/Components/ComponentMeshRenderer.h"
#include "Core/Components/ComponentTransform.h"
#include "Math/Quat.h"
#include "Math/float2.h"
#include "Math/float3.h"
#include "Core/ResourcesManager/ResourcesManager.h"

// static variables
std::unordered_map<std::string, Texture> ResourcesManager::texturesLoaded;
std::unordered_map<unsigned int, Texture> ResourcesManager::texturesLoadedInt;

inline float4x4 aiMatrix4x4ToMathGeo(const aiMatrix4x4* from)
{
    float4x4 to;

    to[0][0] = (float)from->a1; to[0][1] = (float)from->b1;  to[0][2] = (float)from->c1; to[0][3] = (float)from->d1;
    to[1][0] = (float)from->a2; to[1][1] = (float)from->b2;  to[1][2] = (float)from->c2; to[1][3] = (float)from->d2;
    to[2][0] = (float)from->a3; to[2][1] = (float)from->b3;  to[2][2] = (float)from->c3; to[2][3] = (float)from->d3;
    to[3][0] = (float)from->a4; to[3][1] = (float)from->b4;  to[3][2] = (float)from->c4; to[3][3] = (float)from->d4;

    return to;
}

ModelImporter::Model::Model(const std::string &path="",unsigned int program=0)
{
	animationsCount = 0;
	meshesCount = 0;
	materialsCount = 0;
	camerasCount = 0;
	lightsCount = 0;
	texturesCount = 0;
	this->program = program;
	
	if (path == "")
	{
		return;
	}
	this->path = path;
	
}


GameObject* ModelImporter::Model::LoadModel()
{
	GameObject* root = nullptr;
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcessPreset_TargetRealtime_MaxQuality);
	// If the import failed, report it
	std::string buf("Loading model:");
	App->editor->consoleWindow->AddLog(buf.append(path.c_str()).c_str());

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		buf = "Error loading:";
		App->editor->consoleWindow->AddLog(buf.append(path.c_str()).append(importer.GetErrorString()).c_str());
		//LOG("Error loading %s: %s", path, importer.GetErrorString());
		return root;
	}
	directory = path.substr(0, path.find_last_of('\\'));
	root =ProcessNode(nullptr,scene->mRootNode, scene);
	
	//Assigning name to the load gameobject given the file name
	std::string name = path.substr(path.find_last_of('\\') + 1, path.length());
	name = name.substr(0, name.find_last_of('.'));
	root->SetName(name.c_str());

	buf = "End loading model:";
	App->editor->consoleWindow->AddLog(buf.append(path.c_str()).c_str());

	
	//Printing details about model
	animationsCount = (int)scene->mNumAnimations;
	meshesCount = (int)scene->mNumMeshes;
	materialsCount = (int)scene->mNumMaterials;
	camerasCount = (int)scene->mNumCameras;
	lightsCount = (int)scene->mNumLights;
	texturesCount = (int)scene->mNumTextures;

	buf = "Animations:" + std::to_string(animationsCount) + " ";
	buf += "Meshes:" + std::to_string(meshesCount) + " ";
	buf += "Materials:" + std::to_string(materialsCount)+ " ";
	buf += "Cameras:" + std::to_string(camerasCount) + " ";
	buf += "Lights:" + std::to_string(lightsCount)+ " ";
	buf += "Textures:" + std::to_string(texturesCount)+ " ";

	double factor(0.0);
	bool result = scene->mMetaData->Get("UnitScaleFactor", factor);
	buf += "Scale:"+std::to_string( result);
	App->editor->consoleWindow->AddLog(buf.c_str());

	return root;
}

GameObject* ModelImporter::Model::ProcessNode(GameObject *parent,aiNode *node, const aiScene *scene)
{
	std::string name = "GameObject";

	if (parent != nullptr) {
		name = node->mName.C_Str();
	}
	GameObject* root = new GameObject(parent,name.c_str(),program);
	float3 translation;
	Quat rotation;
	float3 scale(1.0f,1.0f,1.0f);
	aiMatrix4x4ToMathGeo(&node->mTransformation).Decompose(translation,rotation,scale);
	float3 rotationRadians = rotation.ToEulerXYZ().Abs();

	// It always needs a transform component
	ComponentTransform* componentTransform = static_cast<ComponentTransform *>(root->AddComponent(ComponentTypes::TRANSFORM));
	componentTransform->SetPosition(translation);
	componentTransform->SetRotation(rotationRadians);
	componentTransform->SetScale(scale);

	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// Adding default components when loading
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		ComponentMesh* componentMesh = static_cast<ComponentMesh *>(root->AddComponent(ComponentTypes::MESH));
		ComponentMaterial* componentMaterial = static_cast<ComponentMaterial *>(root->AddComponent(ComponentTypes::MATERIAL));

		ProcessedMesh processedMesh = ProcessMesh(mesh, scene);
		componentMesh->SetVertices(processedMesh.vertices);
		componentMesh->SetIndices(processedMesh.indices);

		componentMaterial->SetSpecularId(processedMesh.specularId);
		componentMaterial->setTextureId(processedMesh.textureId);

		root->AddComponent(ComponentTypes::MESHRENDERER);

	}
	root->CalculateMeshBoundingBox();
	// process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		root->AddChildren( ProcessNode(root,node->mChildren[i], scene) );
	}

	return root;
}

ProcessedMesh ModelImporter::Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	unsigned int textureId;
	unsigned int specularId;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		float3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;

		vertex.Position = vector;

		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;

		vertex.Normal = vector;

		if (mesh->mTextureCoords[0])
		{
			float2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = float2(0,0);
		}
		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		//assert(face.mNumIndices == 3);
		
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		textureId = LoadMaterialTexture(material, aiTextureType_DIFFUSE);
		specularId = LoadMaterialTexture(material, aiTextureType_SPECULAR);
	}

	return ProcessedMesh(vertices, indices, textureId,specularId);
}

unsigned int ModelImporter::Model::LoadMaterialTexture(aiMaterial *mat, aiTextureType type)
{
	unsigned int textureId = 0;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

    	textureId = TextureImporter::TextureLoader::GetTextureIdByPath(str.C_Str(), directory.c_str() );
	}
	return textureId;
}


void ModelImporter::Model::CalculateBoxDimensions() 
{
	dimensions.x = abs(abs(boundBoxMax.x) - abs(boundBoxMin.x));
	dimensions.y = abs(abs(boundBoxMax.y) - abs(boundBoxMin.y));
	dimensions.z = abs(abs(boundBoxMax.z) - abs(boundBoxMin.z));
}
