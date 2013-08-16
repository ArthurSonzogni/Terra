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

void Character_player::setBowlPosition(float X, float Y, float Z)
{
	if (bowlPosition.size()>=10)
		bowlPosition.pop_back();
	floatCoord position={X,Y,Z};
	bowlPosition.push_front(position);
	
	// compute the camera position
	
	camera.setPosition(X-3,Y-3,Z+3);
	camera.lookAt(X,Y,Z);
}
