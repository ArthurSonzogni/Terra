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
	SlotElementTypeSemiBlock,
	SlotElementTypeStartPoint,
	SlotElementTypeEndPoint
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
		 void draw_board(int x1, int y1, int width, int height, int alpha, int filled);

		 /**********
		  * Mouse  *
		  **********/
		 bool mouseClic;
		 int mouseX;
		 int mouseY;
	public:
		void setScreen(sf::RenderWindow* Screen);
		void draw();
		Game_editor_menu();
		~Game_editor_menu();
		void give_focus();
		void take_focus();
		bool isFocusOn();
		SlotElement getCurrentSlotElement();

		void increaseSelectedSlot();
		void decreaseSelectedSlot();
		
		void getMouseInfo(bool clic, int x, int y);
};



#endif /* end of include guard: GAME_EDITOR_MENU_W1W81OOB */
