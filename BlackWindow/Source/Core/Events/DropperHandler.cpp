#include "DropperHandler.h"
#include <string>
#include "Application.h"
#include "ModuleScene.h"
#include "ModuleWindow.h"
#include "SDL.h"


void DropperHandler::DropFileIntoWindow(char *filePath) 
{
	char * found = strstr (filePath,"fbx");
    if (found == NULL){ // force to check if image
        App->scene->SwapTexture(filePath);
        // TODO::Validate through a list of extensions
        // SDL_ShowSimpleMessageBox(
        //     SDL_MESSAGEBOX_INFORMATION,
        //     "File extension not accepted for the moment",
        //     filePath,
        //     App->window->window
        // );
        SDL_free(filePath);    // Free dropped_filedir memory
        return;
    }

    App->scene->SwapModel(filePath);
}
