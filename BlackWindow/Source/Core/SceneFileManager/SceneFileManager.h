#include "json/json.h"
#include <iostream>
#include <fstream>
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include <future>         // std::async, std::future
class SceneFileManager
{
public:
    SceneFileManager(){}
    ~SceneFileManager(){}


    static void Save(){
        App->editor->consoleWindow->AddLog("Saving scene ....");
        std::async(std::launch::async, [] { 
            App->scene->SaveScene(); 
            App->editor->consoleWindow->AddLog("Scene saved");
        });
       
    }

    static void ExportFile(const char *name, const Json::Value &jsonNode){
        Json::StyledStreamWriter writer;
        std::ofstream outputFile(name);
        writer.write(outputFile, jsonNode);
    }

	static void LoadFromFile(const char *name){
        App->editor->consoleWindow->AddLog("Loading scene...");
        Json::Value root;
        std::ifstream file(name);
        if(file.fail()){
            App->editor->consoleWindow->AddLog("Scene file not exists");
            return;
        }
        file >> root;
        std::async(std::launch::async, [ root ] { 
            App->scene->Load(root);
            App->editor->consoleWindow->AddLog("Scene loaded");
        });

        // result.get();
        // App->scene->Load(root);
    }

};
