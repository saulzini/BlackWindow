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
    // found
    if ( 
        std::find(FileFormats::imageFormats.begin(), FileFormats::imageFormats.end(), filePath) != FileFormats::imageFormats.end()
    ) 
    {
        App->scene->SwapTexture(filePath);
        SDL_free(filePath); // Free dropped_filedir memory
        return;
    }

    App->scene->AddModel(filePath);
    SDL_free(filePath); // Free dropped_filedir memory
}
