#ifndef CHARACTER_FREE_VIEW_WN6U9WNX
#define CHARACTER_FREE_VIEW_WN6U9WNX

#include "character.h"
#include "game_editor_menu.h"


class Character_free_view : public Character
{	
	public:
		Character_free_view();
		virtual void update_mouse_position(int x, int y);
		virtual void mouse_click();
		virtual void step();
		void setBlockType(SlotElement& s);
	private:
		SlotElement slotElement;
};


#endif /* end of include guard: CHARACTER_FREE_VIEW_WN6U9WNX */

