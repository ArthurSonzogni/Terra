#include "texture.h"
#include <SFML/Graphics.hpp>

using namespace sf;

// texture location
const char* texture_location[]=
{
	"image/text_block.jpg",
	"image/text_ball.jpg"
};

GLuint texture_id[texture_enum_max];
GLuint get_texture_id(int i)
{
	return texture_id[i];
};




Texture_loader::Texture_loader()
{
	int i;
	GLuint texture = 0;
	for(i=0;i<texture_enum_max;++i)
	{
        Image image;
        if (!image.loadFromFile(texture_location[i])) return;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getSize().x, image.getSize().y, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		texture_id[i]=texture;
	}
}

Texture_loader::~Texture_loader()
{
	int i;
	for(i=0;i<texture_enum_max;++i)
	{
    	glDeleteTextures(1, texture_id+i);
	}
}


void texture_block_get_position(int id, float* position)
{
	position[0]=float(id%TEXT_X_N)*TEXT_X_INC;
	position[1]=float(id%TEXT_Y_N)*TEXT_Y_INC;
}
