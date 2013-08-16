#ifndef CHARACTER_PLAYER_ONP4IO3A
#define CHARACTER_PLAYER_ONP4IO3A

#include "character.h"

struct floatCoord
{
	float x,y,z;
};

class Character_player : public Character
{	
	public:
		Character_player();
		virtual void update_mouse_position(float x, float y);
		virtual void step();
		void setBowlPosition(float x, float y, float z);
	private:
		list<floatCoord> bowlPosition;
		float bowlAngle;
};


#endif /* end of include guard: CHARACTER_PLAYER_ONP4IO3A */




