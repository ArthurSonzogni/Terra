#include "game_editor.h"
#include "texture.h"


//----- Grid loading function ----//
int Game_editor::levelLoadFromFile(char* filename)
{

}
void Game_editor::Game_editor::levelLoadFromGrid(Grid* grid)
{

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

}
void Game_editor::levelSaveToGrid()
{

}
	
//----- Execution function -------//
void Game_editor::process()
{
	if (!grid|!window|!objectProgram|!shadowProgram) return;

	bool running=true;
	sf::Clock c;
	while(running)
	{
		sf::Clock cgrid;
		//event management
		sf::Event event;
		while (window->pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				// the program is stopped
				running = false;
				state = game_editor_state_exit_all;
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
					state = game_editor_state_exit;
				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == sf::Mouse :: Left)
					character.mouse_click();
			}
			else if (event.type == sf::Event::MouseWheelMoved)
			{
				if (event.mouseWheel.delta>0)
				{
					game_editor_menu.increaseSelectedSlot();
				}
				else if (event.mouseWheel.delta<0)
				{
					game_editor_menu.decreaseSelectedSlot();
				}
			}
		}
		cout<<"-----"<<endl;
		cout<<"event pool="<<int(cgrid.getElapsedTime().asSeconds()*30*100)<<endl;
		cgrid.restart();


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			character.move_left();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			character.move_right();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			character.move_forward();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			character.move_backward();
	
		SlotElement sl=game_editor_menu.getCurrentSlotElement();
		character.setBlockType(sl);

		character.step();

		sf::Vector2i position = sf::Mouse::getPosition(*window);
		character.update_mouse_position(position.x-400,-(position.y-300));
		position.x=400;
		position.y=300;
		sf::Mouse::setPosition(position,*window);
		scene.setCameraMatrix(character.get_view());

		cout<<"direct event="<<int(cgrid.getElapsedTime().asSeconds()*30*100)<<endl;
		cgrid.restart();


		
		// drawing phase
		for(int mode=BINDFORSHADOW;mode<=BINDFOROBJECT;++mode)
		{
			if (mode==BINDFORSHADOW)
			{
				int cx,cy,cz;
				character.getPosition(cx,cy,cz);
				scene.setCameraPosition(cx,cy,cz);
			}

			scene.bindFor(mode);
			
			GLint location = glGetUniformLocation(objectProgram, "tex");
			glUniform1i(location,0);
			glActiveTexture(GL_TEXTURE0);

			grid->draw();
			
		}
		game_editor_menu.draw();


		cout<<"drawing="<<int(cgrid.getElapsedTime().asSeconds()*30*100)<<endl;
		cgrid.restart();
		window->display();
		cout<<"window display="<<int(cgrid.getElapsedTime().asSeconds()*30*100)<<endl;

		double time_elapsed=c.getElapsedTime().asSeconds();
		
		cgrid.restart();
		sf::sleep(sf::seconds(1.0/30.0-time_elapsed));
		cout<<"sleeping ="<<int(cgrid.getElapsedTime().asSeconds()*30*100)<<endl;
		
		time_elapsed=c.getElapsedTime().asSeconds();
		cout<<"TOTAL TIME="<<int((time_elapsed)*30*100)<<endl;

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
	state=game_editor_state_not_ready;
	objectProgram=0;
	shadowProgram=0;	
}
Game_editor::~Game_editor()
{
	deleteGrid();
}

//----- Scene initilisation function ----//
void Game_editor::setProgram(GLuint s, GLuint o)
{
	shadowProgram=s;
	objectProgram=o;
	scene.setShadowProgram(shadowProgram);	
	scene.setObjectProgram(objectProgram);
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
