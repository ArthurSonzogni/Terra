#include "game_editor.h"
#include "texture.h"


//----- Grid loading function ----//
int Game_editor::levelLoadFromFile(char* filename)
{
	//TODO
	return 0;
}
void Game_editor::levelLoadFromGrid(Grid* g)
{
	deleteGrid();
	grid=g->allocCopy();
}
void Game_editor::levelLoadEmpty()
{
	deleteGrid();
	grid=new Grid();
	grid->set_dimension(32,32,32);
	character.set_grid(grid);
	for(int x=1;x<=32;++x)
	for(int y=1;y<=32;++y)
	{
		grid->block_active(x,y,3,texture_block_rock);
	}
}

//----- Grid saving function -----//
int Game_editor::levelSaveToFile(char* filename)
{
	return 0;
}
void Game_editor::levelSaveToGrid()
{

}
	
//----- Execution function -------//
void Game_editor::process()
{
	if (!grid||!window||!objectProgram||!shadowProgram||!shadowProgram) return;

	bool running=true;
	sf::Clock c;
	while(running)
	{
		//event management
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// the program is stopped
				running = false;
				state = exit_all;
			}
			else if (event.type == sf::Event::Resized)
			{
				// viewport adjusting when the window is redimensionned
				glViewport(0, 0, event.size.width, event.size.height);
			}
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					running=false;
					state = exit;
				}
				else if (event.key.code == sf::Keyboard::Space)
				{
					if (game_editor_menu.isFocusOn())
						game_editor_menu.give_focus();
					else
						game_editor_menu.take_focus();

				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (game_editor_menu.isFocusOn())
				{
					if (event.mouseButton.button == sf::Mouse :: Left)
						game_editor_menu.getMouseInfo(true,event.mouseButton.x,event.mouseButton.y);
				}
				else
				{
					if (event.mouseButton.button == sf::Mouse :: Left)
						character.mouse_click(true);
					else if (event.mouseButton.button == sf::Mouse :: Right)
						character.mouse_click(false);
				}
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				if (game_editor_menu.isFocusOn())
				{
					game_editor_menu.getMouseInfo(false,event.mouseMove.x,event.mouseMove.y);
				}
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				if (event.mouseWheel.delta>0.0)
				{
					game_editor_menu.increaseSelectedSlot();
				}
				else if (event.mouseWheel.delta<0.0)
				{
					game_editor_menu.decreaseSelectedSlot();
				}
			}
		}

		
		if (not game_editor_menu.isFocusOn())
		{

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				character.move_left();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				character.move_right();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				character.move_forward();

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				character.move_backward();

			character.step();
	
			sf::Vector2i position = sf::Mouse::getPosition(*window);
			character.update_mouse_position(position.x-400.0,-(position.y-300.0));
			position.x=400;
			position.y=300;
			sf::Mouse::setPosition(position,*window);
			scene.setCameraMatrix(character.get_view());
		}

		SlotElement sl=game_editor_menu.getCurrentSlotElement();
		character.setBlockType(sl);

		int cx,cy,cz;
		character.getPosition(cx,cy,cz);
		scene.setCameraPosition(cx,cy,cz);

		// drawing phase
		for(int mode=BINDFORSHADOW;mode<=BINDFOROBJECT;++mode)
		{
			scene.bindFor(mode);
			
			if (mode==BINDFORSKYBOX)
			{
				scene.performSkyboxDrawing();
			}
			
			if (mode==BINDFOROBJECT or mode==BINDFORSHADOW)
			{
				if (mode==BINDFOROBJECT)
				{
					GLint location = glGetUniformLocation(objectProgram, "tex");
					glUniform1i(location,0);
					glActiveTexture(GL_TEXTURE0);
				}

				grid->draw_special(Grid::DRAW_STARTS_POINT|Grid::DRAW_END_POINT,scene);
				grid->draw(scene);
			}
			
		}
		game_editor_menu.draw();


		window->display();
		double time_elapsed=c.getElapsedTime().asSeconds();
		sf::sleep(sf::seconds(1.0/30.0-time_elapsed));
		c.restart();
	}
}
int Game_editor::getState()
{
	return state;
}

//----- Instanciation function ---//
Game_editor::Game_editor()
{	
	grid=NULL;
	window=NULL;
	state=not_ready;
	objectProgram=0;
	shadowProgram=0;	
	skyboxProgram=0;
}
Game_editor::~Game_editor()
{
	deleteGrid();
}

//----- Scene initilisation function ----//
void Game_editor::setProgram(GLuint s, GLuint o, GLuint k)
{
	shadowProgram=s;
	objectProgram=o;
	skyboxProgram=k;
	scene.setShadowProgram(shadowProgram);	
	scene.setObjectProgram(objectProgram);
	scene.setSkyBoxProgram(skyboxProgram);
}

void Game_editor::setScreen(sf::RenderWindow* w)
{
	window=w;
	game_editor_menu.setScreen(w);
}

//----- Helper member function -------//
void Game_editor::deleteGrid()
{
	if (grid)
	{
		delete grid;
		grid=NULL;
	}	
}

Grid* Game_editor::getGrid()
{
	return grid;
}

void Game_editor::setSkyboxTextureId(GLuint id)
{
	scene.setSkyboxTextureId(id);
}
