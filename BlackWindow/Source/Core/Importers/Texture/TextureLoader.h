#pragma once

#include <string>
#include <GL/glew.h>
#include <IL/il.h>
#include "Application.h"
#include "ModuleEditor.h"
#include "UIWindow/ConsoleWindow.h"
#include <IL/ilut.h>
#include <IL/ilu.h>
#include "Core/FileManager.h"
enum class Strategy
{
	Normal,
	SameFolder,
	Textures,
	White
};
namespace TextureImporter
{

	class TextureLoader
	{
	public:
		TextureLoader(){};

		/// <summary>
		/// Method that returns 0 if an error has ocurred if not it returns the opengl id
		/// </summary>
		/// <param name="path"></param>
		/// <param name="generateMipMaps"></param>
		/// <returns></returns>

		static unsigned int FindTexture(const std::string &path, const std::string &directory, const std::string &filename)
		{
			ILboolean success = false;
			unsigned int imageId;

			// generate an image name
			ilGenImages(1, &imageId);
			// bind it
			ilBindImage(imageId);
			// match image origin to OpenGLs
			ilEnable(IL_ORIGIN_SET);
			ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

			//Looping given the strategies defined
			std::vector<Strategy> types{
				Strategy::Normal,
				Strategy::SameFolder,
				Strategy::Textures,
				Strategy::White,
			};

			for (std::vector<Strategy>::iterator it = types.begin(); it != types.end() && !success; ++it)
			{
				std::string buf("Begin loading texture ");
				std::string resultPath("");
				std::size_t found = std::string::npos;
				switch (*it)
				{
				case Strategy::Normal:
					buf.append(std::string("normal strategy:").c_str());
					resultPath = filename.c_str();
					break;
				case Strategy::SameFolder:
					buf.append(std::string("same folder strategy:").c_str());
					resultPath = directory.c_str();
					found = path.rfind('\\');
					resultPath.append("\\");

					if (found != std::string::npos)
					{
						resultPath.append(path.substr(found, path.length()).c_str());
					}
					else
					{
						resultPath.append(path.c_str());
					}
					break;
				case Strategy::Textures:
					buf.append(std::string("textures strategy: ").c_str());
					resultPath = "..\\Assets\\Textures\\";
					resultPath.append(path.c_str());
					break;
				default:
					buf.append(std::string("white strategy: ").c_str());
					resultPath = "..\\Assets\\Textures\\White.png";
					break;
				}

				App->editor->consoleWindow->AddLog(buf.append(resultPath.c_str()).c_str());

				//Reading file
				FileManager fileTexture;

				if (fileTexture.ReadFile(resultPath.c_str()))
				{
					success = ilLoadL(IL_TYPE_UNKNOWN, fileTexture.GetFile(), fileTexture.GetSize());

					// check to see if everything went OK
					if (!success)
					{
						//Check for error
						ILenum Error;
						while ((Error = ilGetError()) != IL_NO_ERROR)
						{
							std::string buf = "Error Loading texture: ";
							App->editor->consoleWindow->AddLog(buf.append(iluErrorString(Error)).c_str());
						}
					}
				}
				else
				{
					buf = "Error reading texture file";
					App->editor->consoleWindow->AddLog(buf.c_str());
				}
			}

			if (!success)
			{
				ilDeleteImages(1, &imageId); //deleting image from il
				return 0;
			}

			return imageId;
		}

		static unsigned int LoadTexture2D(const std::string &path, const std::string &directory, bool generateMipMaps = true)
		{
			std::string filename = directory + '\\' + path;

			unsigned int imageId = FindTexture(path, directory, filename);
			if (imageId == 0)
			{
				return imageId;
			}

			std::string buf = "Loaded texture:";
			App->editor->consoleWindow->AddLog(buf.append(filename.c_str()).c_str());

			/* Convert image to RGBA with unsigned byte data type */
			ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

			//Open gl section
			glGenTextures(1, &imageId);			   // creating image in opengl
			glBindTexture(GL_TEXTURE_2D, imageId); //Bind texture
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

			ilDeleteImage(imageId); //release from il since we have it in opengl

			return imageId;
		}
	};
} // namespace TextureImporter
