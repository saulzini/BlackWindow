#include "json/json.h"
#include <iostream>
#include <fstream>
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include <future>         // std::async, std::future
#include <thread>

class SceneFileManager
{
public:
    SceneFileManager(){}
    ~SceneFileManager(){}

    static void SaveScene() {
        App->editor->consoleWindow->AddLog("Saving scene ....");
        App->scene->SaveScene("saves/scene.blackwindow");
        App->editor->consoleWindow->AddLog("Scene saved");
    }

    static void SaveForPlay() {
        App->editor->consoleWindow->AddLog("Saving scene for play ....");
        App->scene->SaveScene("saves/scene-play.blackwindow");
        App->editor->consoleWindow->AddLog("Scene saved for play");
    }

    static void Save(){
        std::thread(SaveScene).detach();
    }

    static void SaveForPlayThread(){
        std::thread(SaveForPlay).detach();
    }

    static void ExportFile(const char *name, const Json::Value &jsonNode){
        Json::StyledStreamWriter writer;
        std::ofstream outputFile(name);
        writer.write(outputFile, jsonNode);
    }

    static void LoadScene() {
        LoadFromFile("saves/scene.blackwindow");
    }

    static void LoadForPlay() {
        LoadFromFile("saves/scene-play.blackwindow");
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
        App->scene->Load(root);
        App->editor->consoleWindow->AddLog("Scene loaded");

    }

};
