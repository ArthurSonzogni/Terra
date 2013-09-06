
#include <GL/glew.h>
#include "texture.h"
#include <SFML/Graphics.hpp>

using namespace sf;

// texture location
const char* texture_location[]=
{
	"image/text_block.jpg",
	"image/text_ball.jpg",
	"image/skybox/Nidor_Bottom.jpg",
	"image/skybox/Nidor_North.jpg",
	"image/skybox/Nidor_South.jpg",
	"image/skybox/Nidor_Top.jpg",
	"image/skybox/Nidor_West.jpg",
	"image/skybox/Nidor_East.jpg"
};

GLuint texture_id[texture_enum_max];
GLuint get_texture_id(int i)
{
	return texture_id[i];
};


/*
GLsizei texture_block_size = 256;
GLsizei texture_block_layer = 2;
GLuint	texture_block = 0;
GLuint get_texture_block() { return texture_block;}
void load_texture_block()
{
	glGenTextures(1,&texture_block);
	glBindTexture(GL_TEXTURE_2D_ARRAY,texture_block);
	//allocate the storage
	glTexStorage3D(
			GL_TEXTURE_2D_ARRAY,
			1,
			GL_RGBA8,
			texture_block_size,
			texture_block_size,
			texture_block_layer
			);
	//Loading the image
	Image image_block;
	if (!image_block.loadFromFile("image/text_block.jpg")) return;
	//upload pixel data
	glTexSubImage3D(GL_TEXTURE_2D_ARRAY,0,0,0,0,
			texture_block_size,
			texture_block_size,
			texture_block_layer,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			image_block.getPixelsPtr()
			);
	// set texture parameter
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	
	// generate mipmap
	for(int i=0;i<8;++i)
	glTexImage3D(GL_TEXTURE_2D_ARRAY, i, GL_RGBA8,
			texture_block_size,
			texture_block_size,
			texture_block_layer,
			0,
			);
}
*/


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
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 5);
		texture_id[i]=texture;
		glBindTexture(GL_TEXTURE_2D,GL_NONE);
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

// TODO memorisation for optimisation
void texture_block_get_position(int id, float* position)
{
	position[0]=float(id%TEXT_X_N)*TEXT_X_INC;
	position[1]=float(id/TEXT_X_N)*TEXT_Y_INC;

	// correction
	//position[0]+=0.5/TEXT_BLOCK_SIZE;
	//position[1]+=0.5/TEXT_BLOCK_SIZE;
}
