#include "character_player.h"
#include "texture.h"

Character_player::Character_player()
{
}

void Character_player::update_mouse_position(float x, float y)
{
	camera.angle_xy_change(-x*DEGTORAD/10);
	camera.angle_xz_change(y*DEGTORAD/10);
}

void Character_player::step()
{

}

void Character_player::setBowlPosition(int x, int y, int z)
{

}
