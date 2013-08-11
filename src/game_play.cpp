#include "game_play.h"


//----- Grid loading function ----//
int GamePlay::levelLoadFromFile(char* filename)
{
	//TODO
}
void GamePlay::levelLoadFromGrid(Grid* g)
{
	deleteGrid();
	grid=g->allocCopy();

	loadMesh();

}

//----- Execution function -------//
void GamePlay::process()
{
	if (!grid||!window||!objectProgram||!shadowProgram||!playergroup)
		return;
	
	
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
					//if (game_editor_menu.isFocusOn())
						//game_editor_menu.give_focus();
					//else
						//game_editor_menu.take_focus();

				}
			}
			else if (event.type == sf::Event::MouseButtonPressed)
			{
				//if (game_editor_menu.isFocusOn())
				//{
					//if (event.mouseButton.button == sf::Mouse :: Left)
						//game_editor_menu.getMouseInfo(true,event.mouseButton.x,event.mouseButton.y);
				//}
				//else
				//{
					//if (event.mouseButton.button == sf::Mouse :: Left)
						//character.mouse_click(true);
					//else if (event.mouseButton.button == sf::Mouse :: Right)
						//character.mouse_click(false);
				//}
			}
			else if (event.type == sf::Event::MouseMoved)
			{
				//if (game_editor_menu.isFocusOn())
				//{
					//game_editor_menu.getMouseInfo(false,event.mouseMove.x,event.mouseMove.y);
				//}
			}
			//else if (event.type == sf::Event::MouseWheelMoved)
			//{
				//if (event.mouseWheel.delta>0.0)
				//{
					//game_editor_menu.increaseSelectedSlot();
				//}
				//else if (event.mouseWheel.delta<0.0)
				//{
					//game_editor_menu.decreaseSelectedSlot();
				//}
			//}
		}

		

		character.step();

		//sf::Vector2i position = sf::Mouse::getPosition(*window);
		//character.update_mouse_position(position.x-400.0,-(position.y-300.0));
		//position.x=400;
		//position.y=300;
		//sf::Mouse::setPosition(position,*window);
		scene.setCameraMatrix(character.get_view());

		
		int cx,cy,cz;
		character.getPosition(cx,cy,cz);
		scene.setCameraPosition(cx,cy,cz);

		// drawing phase
		for(int mode=BINDFORSHADOW;mode<=BINDFOROBJECT;++mode)
		{
			scene.bindFor(mode);
			
			GLint location = glGetUniformLocation(objectProgram, "tex");
			glUniform1i(location,0);
			glActiveTexture(GL_TEXTURE0);

			grid->draw();
			
		}
		window->display();

		// physic simulation
		if (playergroup->isServer())
		{
			game_physic.stepSimulation(1.0/30.0);
			
		}
		else
		{

		}
		

		double time_elapsed=c.getElapsedTime().asSeconds();
		sf::sleep(sf::seconds(1.0/30.0-time_elapsed));
		c.restart();
	}
}
int GamePlay::getState()
{
	return state;
}

//----- Instanciation function ---//
GamePlay::GamePlay()
{
	grid=NULL;
	window=NULL;
	mesh=NULL;
}
GamePlay::~GamePlay()
{
	deleteGrid();
	if (mesh!=NULL) delete mesh;
}

//----- Scene initilisation function ----//
void GamePlay::setProgram(GLuint s, GLuint o)
{
	shadowProgram=s;
	objectProgram=o;
	scene.setShadowProgram(shadowProgram);	
	scene.setObjectProgram(objectProgram);
}

void GamePlay::setScreen(sf::RenderWindow* w)
{
	window=w;
}

//----- Helper member function -------//
void GamePlay::deleteGrid()
{
	if (grid)
	{
		delete grid;
		grid=NULL;
	}	
}

void GamePlay::setPlayerGroup(PlayerGroup* p)
{
	playergroup=p;
}

void GamePlay::deleteMesh()
{
	if (mesh!=NULL)
	{
		delete mesh;
		mesh=NULL;
	}
}
void GamePlay::loadMesh()
{
	deleteMesh();
	mesh=grid->get_mesh();	
	game_physic.set_world_mesh(mesh);
}
