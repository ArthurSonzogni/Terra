#include "camera.h"
#include <cmath>

camera::camera():
	x(0),
	y(0),
	z(0),
	angle_xy(0),
	angle_xz(0)
{
}
void camera::move_forward(float increment)
{
	x+=increment*cos(angle_xy)*cos(angle_xz);
	y+=increment*sin(angle_xy)*cos(angle_xz);
	z+=increment*sin(angle_xz);
}

void camera::move_laterally(float increment)
{
	x+=increment*sin(angle_xy)*cos(angle_xz);
	y+=increment*cos(angle_xy)*cos(angle_xz);
	z+=increment*sin(angle_xz);
}
void camera::angle_xy_change(float increment)
{
	angle_xy+=increment;
}
void camera::angle_xz_change(float increment)
{
	angle_xy+=increment;
}
