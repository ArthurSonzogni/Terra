#include "character_free_view.h"

void Character_free_view::update_mouse_position(int x, int y)
{
	camera.angle_xy_change(-x*DEGTORAD/10);
	camera.angle_xz_change(y*DEGTORAD/10);
}
