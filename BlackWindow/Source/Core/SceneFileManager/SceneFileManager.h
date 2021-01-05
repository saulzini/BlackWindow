#include "json/json.h"
#include <iostream>
#include <fstream>
#include "Application.h"
#include "ModuleScene.h"
class SceneFileManager
{
public:
    SceneFileManager(){}
    ~SceneFileManager(){}

    static void ExportFile(const char *name, const Json::Value &jsonNode){
        Json::StyledStreamWriter writer;
        std::ofstream outputFile(name);
        writer.write(outputFile, jsonNode);
    }

	static void LoadFromFile(const char *name){
        Json::Value root;
        std::ifstream file(name);
        file >> root;
        // Json::StyledStreamWriter writer;
        // std::ofstream outputFile(name);
        // writer.write(outputFile, jsonNode);
        App->scene->Load(root);
       
    }

};
