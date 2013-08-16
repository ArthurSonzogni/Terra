#include "camera.h"
#include <cmath>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include <cmath>
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
	x+=increment*sin(angle_xy);
	y+=-increment*cos(angle_xy);
}
void Camera::angle_xy_change(float increment)
{
	angle_xy+=increment;
}
void Camera::angle_xz_change(float increment)
{
	angle_xz+=increment;
	if (angle_xz<-1.56) angle_xz=-1.56;
	if (angle_xz>+1.56) angle_xz=+1.56;
}

glm::mat4 Camera::get_view()
{
	float viewx,viewy,viewz;
	viewx=cos(angle_xy)*cos(angle_xz);
	viewy=sin(angle_xy)*cos(angle_xz);
	viewz=sin(angle_xz);
	gluLookAt(x,y,z,x+viewx,y+viewy,z+viewz,0,0,1);
	return glm::lookAt(
			glm::vec3(x,y,z),
			glm::vec3(x+viewx,y+viewy,z+viewz),
			glm::vec3(0,0,1)
	);

	/*
	glRotatef(angle_xz*RADTODEG,0.f,1.f,0.f);
	glRotatef(-angle_xy*RADTODEG,0.f,0.f,1.f);
	glTranslatef(-x,-y,-z);
	*/
}

Ray Camera::get_ray()
{
	Ray ray;
	ray.set_position(x,y,z);
	ray.set_direction(
		cos(angle_xy)*cos(angle_xz),
		sin(angle_xy)*cos(angle_xz),
		sin(angle_xz)
	);
	return ray;
}

void Camera::setPosition(float X, float Y, float Z)
{
	x=X;
	y=Y;
	z=Z;
}

void Camera::lookAt(float X, float Y, float Z)
{
	float dx=X-x;
	float dy=Y-y;
	float dz=Z-z;
	float norme1=sqrt(dx*dx+dy*dy+dz*dz);
	float norme2=sqrt(dx*dx+dy*dy);
	float scalareProduct=(dx*dx+dy*dy)/(norme1*norme2);
	angle_xy=atan2(dy,dx);
	angle_xz=acos(scalareProduct);
	if (dz<0.0) angle_xz=-angle_xz;
}
