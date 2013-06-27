#include "ray.h"
#include <cmath>

inline float max(float x,float y)
{
	return (x>y)?x:y;
}

inline float min(float x,float y)
{
	return (x>y)?x:y;
}

Ray::Ray():
	x(0),
	y(0),
	z(0),
	dx(0),
	dy(0),
	dz(0)
{
	
}
void Ray::set_position(float X,float Y,float Z)
{
	x=X;
	y=Y;
	z=Z;
}

void Ray::set_direction(float DX, float DY, float DZ)
{
	dx=DX;
	dy=DY;
	dz=DZ;
}

void Ray::increment()
{
	float x_rel,y_rel,z_rel;
	double integer_part;
	x_rel=modf(p_x,&integer_part);
	y_rel=modf(p_y,&integer_part);
	z_rel=modf(p_z,&integer_part);
	float tx=-1;
	float tX=-1;
	float ty=-1;
	float tY=-1;
	float tz=-1;
	float tZ=-1;
	
	if (dx!=0.0) tx=(0.0-x_rel)/dx;
	if (dx!=0.0) tX=(1.0-x_rel)/dx;
	if (dy!=0.0) ty=(0.0-y_rel)/dx;
	if (dy!=0.0) tY=(1.0-y_rel)/dx;
	if (dz!=0.0) tz=(0.0-z_rel)/dx;
	if (dz!=0.0) tZ=(1.0-z_rel)/dx;
	
	float t=0.0;
	t=max(t,tx);
	t=max(t,tX);
	t=max(t,ty);
	t=max(t,tY);
	t=max(t,tz);
	t=max(t,tZ);

	if (tx>0.0) t=min(t,tx);
	if (tX>0.0) t=min(t,tX);
	if (ty>0.0) t=min(t,ty);
	if (tY>0.0) t=min(t,tY);
	if (tz>0.0) t=min(t,tz);
	if (tZ>0.0) t=min(t,tZ);

	// avoiding being on the frontiere
	t*=1.01;

	p_x+=t*dx;
	p_y+=t*dy;
	p_z+=t*dz;
}

void Ray::get_current_position(int* X,int* Y,int *Z)
{
	*X=p_x;
	*Y=p_y;
	*Z=p_z;
}
