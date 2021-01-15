#include "DropperHandler.h"
#include <string>
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "FileFormats.h"
#include <algorithm>

void DropperHandler::DropFileIntoWindow(char *filePath)
{
    // Looking through vector of extensions
    for (std::vector<std::string>::iterator it = FileFormats::imageFormats.begin(); it != FileFormats::imageFormats.end(); ++it)
    {
        std::string filePathString = filePath;
        std::size_t index = filePathString.find((*it));
        if (index !=std::string::npos )
        {
            // found
            App->scene->AddTexture(filePath);
            SDL_free(filePath); // Free dropped_filedir memory
            return;
        }
    }

    App->scene->AddModel(filePath);
    SDL_free(filePath); // Free dropped_filedir memory
}
