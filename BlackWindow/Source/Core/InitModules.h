#pragma once
#include <IL/ilut.h>
class InitModules
{
public:
    static void init(){
    	// init DevIL. This needs to be done only once per application
        ilInit();
	    iluInit();
    }
};
