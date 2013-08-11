#ifndef CHARACTER_PLAYER_ONP4IO3A
#define CHARACTER_PLAYER_ONP4IO3A

#include "character.h"

class Character_player : public Character
{	
	public:
		Character_player();
		virtual void update_mouse_position(float x, float y);
		virtual void step();
		void setBowlPosition(int x, int y, int z);
	private:
		int bowlPosition_x;
		int bowlPosition_y;
		int bowlPosition_z;
};


#endif /* end of include guard: CHARACTER_PLAYER_ONP4IO3A */




