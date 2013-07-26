#include "game_editor_menu.h"
#include <iostream>
#include "texture.h"

using namespace sf;
using namespace std;

Game_editor_menu::Game_editor_menu()
{
	screen=NULL;
	asFocus=false;

	/****************
	 * Slot related *
	 ****************/

	nbSlot=5;
	selectedSlot=0;
	slotContent.reserve(nbSlot);
	for(int i=0;i<nbSlot;++i)
	{
		SlotElement s;
		s.type=SlotElementTypeNone;
		s.subtype=0;
		slotContent.push_back(s);
	}
	
	slotContent[0].type=SlotElementTypeBlock;
	slotContent[1].type=SlotElementTypeSemiBlock;
	slotContent[0].subtype=0;
	slotContent[1].subtype=1;
	
	//**************************//
	image_loading();
}

Game_editor_menu::~Game_editor_menu()
{
}

void Game_editor_menu::image_loading()
{
	barImage.loadFromFile("image/editorbar.png");
	textureBlockImage.loadFromFile("image/text_block.jpg");
}


void Game_editor_menu::setScreen(RenderWindow* Screen)
{
	screen=Screen;
}

void Game_editor_menu::draw()
{
	screen->pushGLStates();
	screen->resetGLStates();
	draw_helper();
	screen->popGLStates();
}

void Game_editor_menu::draw_helper()
{

	float xStart=screen->getSize().x/2-32-float(nbSlot)*32.0;
	float yStart=screen->getSize().y-64.0;
	
	static float yStartShift=64.0;
	yStartShift*=0.9;
	yStart+=yStartShift;
	
	sf::Sprite sprite;
	sf::Sprite spriteTexture;
	sprite.setTexture(barImage);
	spriteTexture.setTexture(textureBlockImage);
	spriteTexture.scale(64./246.0,64./256.0);
			

	/***********************
	 * Bar content drawing *
	 ***********************/
	
		
		// texture drawing
		spriteTexture.setPosition(xStart,yStart);
		spriteTexture.move(32,0);
		for(int i=0;i<nbSlot;++i)
		{
			switch (slotContent[i].type)
			{
				case SlotElementTypeNone:
				{
				} break;
				case SlotElementTypeSemiBlock:
				{
					spriteAssignBlockTexture(spriteTexture,slotContent[i].subtype);
					screen->draw(spriteTexture);
				} break;
				case SlotElementTypeBlock:
				{
					spriteAssignBlockTexture(spriteTexture,slotContent[i].subtype);
					screen->draw(spriteTexture);
				} break;
			}
			spriteTexture.move(64,0);
		}

		// icon drawing
		sprite.setPosition(xStart,yStart);
		sprite.move(64,32);
		for(int i=0;i<nbSlot;++i)
		{
			switch (slotContent[i].type)
			{
				case SlotElementTypeNone:
				{
				} break;
				case SlotElementTypeSemiBlock:
				{
					sprite.setTextureRect(sf::IntRect(32,64,32,32));
					screen->draw(sprite);
				} break;
				case SlotElementTypeBlock:
				{
					sprite.setTextureRect(sf::IntRect(0,64,32,32));
					screen->draw(sprite);
				} break;
			}
			sprite.move(64,0);
		}
		
	/***************
	 * Bar drawing *
	 ***************/
		sprite.setPosition(xStart,yStart);

		// left corner
		sprite.setTextureRect(sf::IntRect(0,0,32,64));
		screen->draw(sprite);
		// slots
		sprite.setTextureRect(sf::IntRect(32,0,64,64));
		sprite.move(32,0);
		for(int i=0;i<nbSlot;++i)
		{
			screen->draw(sprite);
			sprite.move(64,0);
		}
		// right corner
		sprite.setTextureRect(sf::IntRect(96,0,32,64));
		screen->draw(sprite);
}

void Game_editor_menu::give_focus()
{
	asFocus=true;
}

bool Game_editor_menu::isFocusOn()
{
	return asFocus;
}

void Game_editor_menu::spriteAssignBlockTexture(sf::Sprite& spr, int textureIndex)
{
	float position[2];
	texture_block_get_position(textureIndex, position);
	position[0]*=TEXT_WIDTH;
	position[1]*=TEXT_HEIGHT;
	spr.setTextureRect(sf::IntRect(
		position[0],
		position[1],
		TEXT_BLOCK_SIZE,
		TEXT_BLOCK_SIZE
		));
	
}

SlotElement Game_editor_menu::getCurrentSlotElement()
{
	return slotContent[selectedSlot];
}

void Game_editor_menu::increaseSelectedSlot()
{
	selectedSlot=(selectedSlot+1)%nbSlot;
}

void Game_editor_menu::decreaseSelectedSlot()
{
	selectedSlot=(selectedSlot-1)%nbSlot;
}
