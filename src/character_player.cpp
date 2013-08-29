#include "character_player.h"
#include "texture.h"

Character_player::Character_player()
{
	bowlAngle=0;
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
	float x_der=0;
	float y_der=0;
	list<floatCoord>::iterator it;
	float l=1.0;
	for(it=bowlPosition.begin();it!=bowlPosition.end();)
	{
		float xb=it->x;
		float yb=it->y;
		++it;
		if (it!=bowlPosition.end())
		{
			float xe=it->x;
			float ye=it->y;
			x_der+=(xe-xb)*l;
			y_der+=(ye-yb)*l;
			l*=0.6;
		}
	}
	float force=x_der*x_der+y_der*y_der;
	if (force>1) force=1;
	bowlAngle+=force*(cos(bowlAngle)*y_der-sin(bowlAngle)*x_der);
	
	camera.setPosition(X+3*cos(bowlAngle),Y+3*sin(bowlAngle),Z+3);
	camera.lookAt(X,Y,Z);
}

float Character_player::getBowlAngle()
{
	return bowlAngle;
}
