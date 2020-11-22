#pragma once

#include <string>
#include <GL/glew.h>
#include <IL/il.h> 
#include "Application.h"
#include "ModuleEditor.h"
#include "ConsoleWindow.h"

class TextureLoader
{
public:
	TextureLoader(){};
	
	/// <summary>
	/// Method that returns 0 if an error has ocurred if not it returns the opengl id
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="generateMipMaps"></param>
	/// <returns></returns>
	static unsigned int LoadTexture2D(const std::string &filename, bool generateMipMaps = true)
	{

		ILboolean success;
		ILuint imageID;

		// generate an image name
		ilGenImages(1, &imageID);
		// bind it
		ilBindImage(imageID);
		// match image origin to OpenGL�s
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

		//Adding to application log
		std::string buf("Begin texture:");
		App->editor->consoleWindow->AddLog(buf.append(filename.c_str()).c_str());
		// load  the image
		success = ilLoadImage((ILstring)filename.c_str());
		// check to see if everything went OK
		if (!success)
		{
			ilDeleteImages(1, &imageID); //deleting image from il
			return 0;
		}
		buf = "Loaded texture:";
		App->editor->consoleWindow->AddLog(buf.append(filename.c_str()).c_str());

		/* Convert image to RGBA with unsigned byte data type */
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

		//Open gl section
		glGenTextures(1, &imageID);			   // creating image in opengl
		glBindTexture(GL_TEXTURE_2D, imageID); //Bind texture
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			ilGetInteger(IL_IMAGE_BPP),
			ilGetInteger(IL_IMAGE_WIDTH),
			ilGetInteger(IL_IMAGE_HEIGHT),
			0,
			ilGetInteger(IL_IMAGE_FORMAT),
			GL_UNSIGNED_BYTE,
			ilGetData());

		if (generateMipMaps)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //linear interpolation for magnification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //linear interpolation for minifiying filter

		ilDeleteImage(imageID); //release from il since we have it in opengl

		return imageID;
	}
};
