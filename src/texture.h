#ifndef TEXTURE_LO7KW3MU
#define TEXTURE_LO7KW3MU

#include <SFML/OpenGL.hpp>

#define TEXT_BLOCK_SIZE (256.0)
#define TEXT_X_N	(4)
#define TEXT_Y_N	(4)
#define TEXT_WIDTH (TEXT_BLOCK_SIZE*TEXT_X_N)
#define TEXT_HEIGHT (TEXT_BLOCK_SIZE*TEXT_Y_N)
#define TEXT_X_INC	(TEXT_BLOCK_SIZE/TEXT_WIDTH)
#define TEXT_Y_INC	(TEXT_BLOCK_SIZE/TEXT_HEIGHT)
#define TEXT_X_INC2	((TEXT_BLOCK_SIZE-1)/TEXT_WIDTH)
#define TEXT_Y_INC2	((TEXT_BLOCK_SIZE-1)/TEXT_HEIGHT)

enum texture_enum
{
	texture_block,
	texture_ball,
	texture_enum_max
};

enum texture_block_identifiant
{
	texture_block_grass,
	texture_block_gravel,
	texture_block_ghost,
	texture_block_rock,
	texture_block_enum_max
};
GLuint get_texture_block();

GLuint get_texture_id(int i);
void texture_block_get_position(int id, float* position);

// a class to load texture
class Texture_loader
{
	public:
		Texture_loader();
		~Texture_loader();
};

#endif /* end of include guard: TEXTURE_LO7KW3MU */
