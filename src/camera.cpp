#include "camera.h"
#include <cmath>
#include <SFML/OpenGL.hpp>
#include <iostream>
using namespace std;

Camera::Camera():
	x(0),
	y(0),
	z(0),
	angle_xy(0),
	angle_xz(0)
{
}

void Camera::move_forward(float increment)
{
	x+=increment*cos(angle_xy)*cos(angle_xz);
	y+=increment*sin(angle_xy)*cos(angle_xz);
	z+=increment*sin(angle_xz);
}

void Camera::move_laterally(float increment)
{
	x+=increment*sin(angle_xy)*cos(angle_xz);
	y+=-increment*cos(angle_xy)*cos(angle_xz);
	z+=increment*sin(angle_xz);
}
void Camera::angle_xy_change(float increment)
{
	angle_xy+=increment;
}
void Camera::angle_xz_change(float increment)
{
	angle_xz+=increment;
}

void Camera::get_view()
{
	cout<<x<<" "<<y<<" "<<z<<" "<<angle_xy<<" "<<angle_xz<<endl;
	glLoadIdentity();
	gluLookAt(0,0,0,1,0,0,0,0,1);
	glRotatef(angle_xz*RADTODEG,0.f,1.f,0.f);
	glRotatef(-angle_xy*RADTODEG,0.f,0.f,1.f);
	glTranslatef(-x,-y,-z);
}
