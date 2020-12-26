#include "Model.h"
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include "Globals.h"
#include "Core/Vertex.h"
#include "Assimp/vector3.h"
#include "Assimp/vector2.h"
#include "Core/Importers/Texture/TextureLoader.h"
#include "BoundingBox.h"
#include "Core/Components/ComponentMesh.h"
#include "Core/Components/ComponentTransform.h"
#include "Math/Quat.h"
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

	BoundingBox boundingBox(scene);
	boundingBox.GetBoundingBox(&boundBoxMin,&boundBoxMax);
	CalculateBoxDimensions();

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
	ComponentTransform* componentTransform = (ComponentTransform*)root->AddComponent(ComponentTypes::TRANSFORM);
	componentTransform->SetPosition(translation);
	componentTransform->SetRotation(rotationRadians);
	componentTransform->SetScale(scale);
	

	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		// Adding default components when loading
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		ComponentMesh *componentMesh = (ComponentMesh*) root->AddComponent(ComponentTypes::MESH);
		componentMesh->SetShader(program);
		componentMesh->SetMesh( ProcessMesh(mesh, scene) );
	}
	// process all the node's meshes (if any)
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		root->AddChildren( ProcessNode(root,node->mChildren[i], scene) );
	}

	return root;
}

Mesh ModelImporter::Model::ProcessMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		aiVector3t<float> vector;
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
			aiVector2t<float> vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;

			vertex.TexCoords = vec;
		}
		else
		{
			vertex.TexCoords = aiVector2t<float>(0, 0);
		}
		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		//		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

		//	textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, indices, textures);
}

std::vector<Texture> ModelImporter::Model::LoadMaterialTextures(aiMaterial *mat, aiTextureType type,const std::string& typeName)
{
	std::vector<Texture> textures;

	for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		std::unordered_map<std::string,Texture>::const_iterator found = texturesLoaded.find(str.C_Str());
		// not found in hash
		if (found == texturesLoaded.end()){
			Texture texture;
			texture.id = TextureImporter::TextureLoader::LoadTexture2D(str.C_Str(),directory.c_str());
			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			texturesLoaded.insert( std::make_pair(str.C_Str(), texture ) ); 
		}
		else{
			textures.push_back(found->second);
		}
	}
	return textures;
}


// void ModelImporter::Model::ApplyTextureToModel(unsigned int id, const char *path) 
// {
// 	for (GLuint i = 0; i < texturesLoaded.size(); i++)
// 	{	
// 		texturesLoaded[i].id = id;
// 		texturesLoaded[i].path == path;
// 	}

// 	for (GLuint i = 0; i < meshes.size(); i++)
// 	{	
// 		meshes[i].ChangeTextures(texturesLoaded);
// 	}
// }

void ModelImporter::Model::CalculateBoxDimensions() 
{
	dimensions.x = abs(abs(boundBoxMax.x) - abs(boundBoxMin.x));
	dimensions.y = abs(abs(boundBoxMax.y) - abs(boundBoxMin.y));
	dimensions.z = abs(abs(boundBoxMax.z) - abs(boundBoxMin.z));
}
