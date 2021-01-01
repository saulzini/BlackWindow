#include "json/json.h"
#include <iostream>
#include <fstream>
class SceneFileManager
{
public:
    SceneFileManager(){}
    ~SceneFileManager(){}

    void ExportFile(const char *name, const Json::Value &jsonNode){
        Json::StyledStreamWriter writer;
        std::ofstream outputFile(name);
        writer.write(outputFile, jsonNode);
    }

};
