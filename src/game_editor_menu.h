#ifndef GAME_EDITOR_MENU_W1W81OOB
#define GAME_EDITOR_MENU_W1W81OOB

#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

using namespace std;

enum SlotElementType
{
	SlotElementTypeNone,
	SlotElementTypeBlock,
	SlotElementTypeSemiBlock
};

struct SlotElement
{
	int type;
	int subtype;
};

class Game_editor_menu
{
	private:
		sf::RenderWindow* screen;
		void draw_helper();
		bool asFocus;

		/****************
		 *  Sprites		*
		 ****************/
		
		sf::Texture barImage;
		sf::Texture textureBlockImage;

		void image_loading();

		/****************
		 *    Slot      *
		 ***************/
		 int nbSlot;
		 int selectedSlot; 
		 vector<SlotElement> slotContent;
		 void spriteAssignBlockTexture(sf::Sprite& spr, int textureIndex);

	public:
		void setScreen(sf::RenderWindow* Screen);
		void draw();
		Game_editor_menu();
		~Game_editor_menu();
		void give_focus();
		bool isFocusOn();
		SlotElement getCurrentSlotElement();

		void increaseSelectedSlot();
		void decreaseSelectedSlot();
};



#endif /* end of include guard: GAME_EDITOR_MENU_W1W81OOB */
