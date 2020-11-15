#include "Texture2D.h"
#include <GL/glew.h>
#include <IL/il.h> 
Texture2D::~Texture2D()
{
   // ilBindImage(0);
   // ilDeleteImage(imageID);
}

ILuint Texture2D::LoadTexture(const std::string& filename, bool generateMipMaps)
{
    ILboolean success;
    ILuint imageID;

    // generate an image name
    ilGenImages(1, &imageID);
    // bind it
    ilBindImage(imageID);
    // match image origin to OpenGL’s
    ilEnable(IL_ORIGIN_SET);
    ilOriginFunc(IL_ORIGIN_LOWER_LEFT);
    // load  the image
    success = ilLoadImage((ILstring)filename.c_str());
    // check to see if everything went OK
    if (!success) {
        ilDeleteImages(1, &imageID); //
        return 0;
    }
    /*
    printf("Width: %d, Height %d, Bytes per Pixel %d",
        ilGetInteger(IL_IMAGE_WIDTH),
        ilGetInteger(IL_IMAGE_HEIGHT),
        ilGetInteger(IL_IMAGE_BYTES_PER_PIXEL));

    std::string s;
    switch (ilGetInteger(IL_IMAGE_FORMAT)) {
    case IL_COLOR_INDEX: s = "IL_COLOR_INDEX"; break;
    case IL_ALPHA: s = "IL_ALPHA"; break;
    case IL_RGB: s = "IL_RGB"; break;
    case IL_RGBA: s = "IL_RGBA"; break;
    case IL_BGR: s = "IL_BGR"; break;
    case IL_BGRA: s = "IL_BGRA"; break;
    case IL_LUMINANCE: s = "IL_LUMINANCE"; break;
    case  IL_LUMINANCE_ALPHA: s = "IL_LUMINANCE_ALPHA"; break;
    }
    printf(" Format %s", s.c_str());

    switch (ilGetInteger(IL_IMAGE_TYPE)) {
    case IL_BYTE: s = "IL_BYTE"; break;
    case IL_UNSIGNED_BYTE: s = "IL_UNSIGNED_BYTE"; break;
    case IL_SHORT: s = "IL_SHORT"; break;
    case IL_UNSIGNED_SHORT: s = "IL_UNSIGNED_SHORT"; break;
    case IL_INT: s = "IL_INT"; break;
    case IL_UNSIGNED_INT: s = "IL_UNSIGNED_INT"; break;
    case IL_FLOAT: s = "IL_FLOAT"; break;
    case IL_DOUBLE: s = "IL_DOUBLE"; break;
    case IL_HALF: s = "IL_HALF"; break;
    }
    printf(" Data type:  %s", s.c_str());*/

    /* Convert image to RGBA with unsigned byte data type */
    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    glGenTextures(1, &imageID); // creating image in opengl
    glBindTexture(GL_TEXTURE_2D,imageID); //Bind texture

    glTexImage2D(
        GL_TEXTURE_2D, 
        0, 
        ilGetInteger(IL_IMAGE_BPP), 
        ilGetInteger(IL_IMAGE_WIDTH),
        ilGetInteger(IL_IMAGE_HEIGHT), 
        0,
        ilGetInteger(IL_IMAGE_FORMAT), 
        GL_UNSIGNED_BYTE, 
        ilGetData()
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //linear interpolation for magnification filter
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //linear interpolation for minifiying filter

    ilDeleteImage(imageID); //release from il since we have it in opengl

    return imageID;
}
