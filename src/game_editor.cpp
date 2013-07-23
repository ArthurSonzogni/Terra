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
		grid->block_active(x,y,3,texture_block_grass);
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
	Clock c;
	while(running)
	{
		c.restart();
		
		// event management
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
        }


		if (Keyboard::isKeyPressed(sf::Keyboard::Left))
			character.move_left();

		if (Keyboard::isKeyPressed(sf::Keyboard::Right))
			character.move_right();

		if (Keyboard::isKeyPressed(sf::Keyboard::Up))
			character.move_forward();

		if (Keyboard::isKeyPressed(sf::Keyboard::Down))
			character.move_backward();
	
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			character.mouse_click();
		}


		Vector2i position = sf::Mouse::getPosition(*window);
		character.update_mouse_position(position.x-400,-(position.y-300));
		position.x=400;
		position.y=300;
		Mouse::setPosition(position,*window);
		scene.setCameraMatrix(character.get_view());
		
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



		double time_elapsed=c.getElapsedTime().asSeconds();;
		cout<<(time_elapsed*30.0)<<endl;
		window->display();
		sf::sleep(sf::seconds(1./30.-time_elapsed));
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

void Game_editor::setWindow(Window* w)
{
	window=w;
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
