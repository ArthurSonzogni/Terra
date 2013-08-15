#include "game_editor_menu.h"
#include <iostream>
#include "texture.h"
#include <cmath>

using namespace sf;
using namespace std;

Game_editor_menu::Game_editor_menu()
{
	screen=NULL;
	asFocus=false;

	/****************
	 * Slot related *
	 ****************/

	nbSlot=11;
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
	slotContent[0].subtype=5;
	slotContent[1].subtype=5;
	
	 /**********
	  * Mouse  *
	  **********/
	 bool mouseClic=false;
	 int mouseX=0;
	 int mouseY=0;
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
	textureBlockImage.setSmooth(true);
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
				case SlotElementTypeStartPoint:
				{
					sprite.setTextureRect(sf::IntRect(64,64,32,32));
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
			if (mouseClic and isFocusOn() and
					sprite.getGlobalBounds().contains(mouseX,mouseY))
				selectedSlot=i;
			sprite.move(64,0);
		}
		// right corner
		sprite.setTextureRect(sf::IntRect(96,0,32,64));
		screen->draw(sprite);

	/********************************
	 *  Highlight the selected slot *
	 ********************************/
	
	sf::RectangleShape highlight;
	highlight.setSize(Vector2f(64,64));
	highlight.move(xStart+32+selectedSlot*64,yStart);
	highlight.setFillColor(sf::Color(255,255,255,128));
	screen->draw(highlight);
	
	if (isFocusOn())
	{
		// draw item
		draw_board(xStart+64*selectedSlot+32,yStart-64*2,64,64*2,128,true);
		
		// first item : full block
		sprite.setTextureRect(sf::IntRect(0,64,32,32));
		sprite.setPosition(xStart+64*selectedSlot+48,yStart-64*2+16);
		screen->draw(sprite);
		if (mouseClic and sprite.getGlobalBounds().contains(mouseX,mouseY))
			slotContent[selectedSlot].type=SlotElementTypeBlock;

		// second item : semi block
		sprite.setTextureRect(sf::IntRect(32,64,32,32));
		sprite.setPosition(xStart+64*selectedSlot+48,yStart-64*2+48);
		screen->draw(sprite);
		if (mouseClic and sprite.getGlobalBounds().contains(mouseX,mouseY))
			slotContent[selectedSlot].type=SlotElementTypeSemiBlock;
		
		// Third item : StartPoint
		sprite.setTextureRect(sf::IntRect(64,64,32,32));
		sprite.setPosition(xStart+64*selectedSlot+48,yStart-64*2+80);
		screen->draw(sprite);
		if (mouseClic and sprite.getGlobalBounds().contains(mouseX,mouseY))
			slotContent[selectedSlot].type=SlotElementTypeStartPoint;

		//draw_board(10,10,screen->getSize().x-20,screen->getSize().y-64*3-10,128,true);
		// texture
		int nbTexture=TEXT_X_N*TEXT_Y_N;
		int textureBoardX=screen->getSize().x-20-20;
		int textureBoardY=screen->getSize().y-20-64*3-10-20;
		float TextureSize=sqrt(textureBoardX*textureBoardY/nbTexture)*0.8;
		spriteTexture.setScale(TextureSize/256.0,TextureSize/256.0);
		int modulo=textureBoardX/TextureSize;
		for(int i=0;i<nbTexture;++i)
		{
			spriteAssignBlockTexture(spriteTexture,i);
			spriteTexture.setPosition(10+32+(i%modulo)*TextureSize,10+32+(i/modulo)*TextureSize);
			screen->draw(spriteTexture);

			// border
			sprite.setPosition(10+32+(i%modulo)*TextureSize,10+32+(i/modulo)*TextureSize);
			if (slotContent[selectedSlot].subtype==i)
				sprite.setTextureRect(sf::IntRect(0,128,64,64));
			else
				sprite.setTextureRect(sf::IntRect(64,128,64,64));

			sprite.setScale(TextureSize/64.0,TextureSize/64.0);
			screen->draw(sprite);

			if (mouseClic and sprite.getGlobalBounds().contains(mouseX,mouseY))
				slotContent[selectedSlot].subtype=i;
				
		}
	}

	mouseClic=false;
}

void Game_editor_menu::give_focus()
{
	asFocus=false;
}

void Game_editor_menu::take_focus()
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
	selectedSlot++;
	if(selectedSlot>=nbSlot)
		selectedSlot-=nbSlot;
}

void Game_editor_menu::decreaseSelectedSlot()
{
	selectedSlot--;
	if (selectedSlot<0)
		selectedSlot+=nbSlot;
}

void Game_editor_menu::draw_board(int x1, int y1, int width, int height, int alpha, int filled)
{
	sf::Sprite sprite;
	sprite.setTexture(barImage);
	sprite.setColor(Color(255,255,255,alpha));
	// leftTopCorner
	sprite.setTextureRect(sf::IntRect(128,0,32,32));
	sprite.setPosition(x1,y1);
	screen->draw(sprite);
	// rightTopCorner
	sprite.setTextureRect(sf::IntRect(128+32*2,0,32,32));
	sprite.setPosition(x1+width-32,y1);
	screen->draw(sprite);
	// leftBottomCorner
	sprite.setTextureRect(sf::IntRect(128,2*32,32,32));
	sprite.setPosition(x1,y1+height-32);
	screen->draw(sprite);
	// rightBottomCorner
	sprite.setTextureRect(sf::IntRect(128+32*2,2*32,32,32));
	sprite.setPosition(x1+width-32,y1+height-32);
	screen->draw(sprite);
	// leftBorder
	sprite.setTextureRect(sf::IntRect(128+32*0,1*32,32,32));
	sprite.setScale(1.0,(height-64.0)/32.0);
	sprite.setPosition(x1,y1+32);
	screen->draw(sprite);
	// rightBorder
	sprite.setTextureRect(sf::IntRect(128+32*2,1*32,32,32));
	sprite.setPosition(x1+width-32,y1+32);
	screen->draw(sprite);
	// topBorder
	sprite.setTextureRect(sf::IntRect(128+32*1,0*32,32,32));
	sprite.setScale((width-64.0)/32.0,1);
	sprite.setPosition(x1+32,y1);
	screen->draw(sprite);
	// bottomBorder
	sprite.setTextureRect(sf::IntRect(128+32*1,2*32,32,32));
	sprite.setPosition(x1+32,y1+height-32);
	screen->draw(sprite);
	if (filled)
	{
		// fill
		sprite.setTextureRect(sf::IntRect(128+32*1,1*32,32,32));
		sprite.setScale((width-64.0)/32.0,(height-64.0)/32.0);
		sprite.setPosition(x1+32,y1+32);
		screen->draw(sprite);
	}
}

void Game_editor_menu::getMouseInfo(bool clic, int x, int y)
{
	mouseX=x;
	mouseY=y;
	mouseClic|=clic;
}
