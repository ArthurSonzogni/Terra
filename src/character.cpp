#include "character.h"

Character::Character():
	camera(),
	grid(NULL)
{
	
}

void Character::set_grid(Grid* G)
{
	grid=G;
}
void Character::mouse_click()
{

}
void Character::update_mouse_position(int x, int y)
{
	cout<<x<<"-----"<<y<<endl;
}

void Character::move_forward()
{
	camera.move_forward(0.3);
}

void Character::move_backward()
{
	camera.move_forward(-0.3);
}

void Character::move_left()
{
	camera.move_laterally(-0.3);
}

void Character::move_right()
{
	camera.move_laterally(0.3);
}


void Character::get_view()
{
	camera.get_view();
}
