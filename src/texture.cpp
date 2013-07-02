#include "texture.h"
#include <SFML/Graphics.hpp>
using namespace sf;

// texture location
const char* texture_location[]=
{
	"img/text_grass.jpg"
};

GLuint texture_id[texture_enum_max];
GLuint get_texture_id(int i)
{
	return texture_id[i];
};

// a class to load texture before the main
class Texture_loader
{
	public:
		Texture_loader();
		~Texture_loader();
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


Texture_loader t;
