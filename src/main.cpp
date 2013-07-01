#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "grid.h"
#include "character_free_view.h"
#include <GL/glu.h>
#include <cmath>
#include "game_physic.h"

using namespace sf;

int andom=3;
int Random()
{
	andom+=7;
	andom*=13;
	andom+=andom%10;
	return  255+(andom%1);
	//return 255+andom%255;
}

bool z_ok(int x,int y,int z)
{
	return z<11-10*cos(float(x)*0.1)*cos(float(y)*0.1);
	//return z<10-10*cos(float(x+y)*0.1)*cos(float(x-y)*0.1);
	//return z<3+((x%2+x%5)%2 + (y%2+y%3)%2)%2;
	//return z<4+3*sin(float(x*y)*0.01);;
	//return z<30-abs(x-25)-abs(y-25);
}

int main()
{
	Game_physic gm;

    // crée la fenêtre
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

	Grid g;
	int grid_dimx=100;
	int grid_dimy=100;
	int grid_dimz=21;
	g.set_dimension(grid_dimx,grid_dimy,grid_dimz);
	

	for(int x=0;x<grid_dimx;++x)
	for(int y=0;y<grid_dimy;++y)
	for(int z=0;z<grid_dimz;++z)
	{
		if (z_ok(x,y,z))
			g.block_active(x,y,z,
					Random(),
					Random(),
					Random()
					);
	}

	for(int k=0;k<1;++k)
	for(int x=0;x<grid_dimx;++x)
	for(int y=0;y<grid_dimy;++y)
	for(int z=0;z<grid_dimz;++z)
	{
			g.block_semi_active(x,y,z,
					Random(),
					Random(),
					Random()
					);
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.f, 1.f, 1.f, 500.f);
	//glOrtho(-1,1,-1,1,-100,100);

	glMatrixMode(GL_MODELVIEW);
	 
	float Light1Pos[4] = {25.f, 25.f, 1000.5, 1.0f};
	float Light1Dif[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	float Light1Spec[4] = {1.0f, 0.0f, 1.0f, 1.0f};
	float Light1Amb[4] = {0.1f, 0.1f, 0.1f, 1.0f};

	GLfloat  matShininess[]={1.0};
	 
	 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light1Dif); 	//Et ceux de la lumière
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light1Spec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light1Amb);


	float Light1Dir[3] = {-1.0f, -1.0f, -1.0f};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Light1Dir);

	// Character creation
	Character_free_view character;
	character.set_grid(&g);
	Vector2i position;
	position.x=400;
	position.y=300;
	Mouse::setPosition(position,window);
	//character.camera.z=0;

    // la boucle principale
    bool running = true;
    while (running)
    {
        // gestion des évènements
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                // on stoppe le programme
                running = false;
            }
            else if (event.type == sf::Event::Resized)
            {
                // on ajuste le viewport lorsque la fenêtre est redimensionnée
                glViewport(0, 0, event.size.width, event.size.height);
            }
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Escape)
				{
					running=false;
				}
			}
        }

        // effacement les tampons de couleur/profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (Keyboard::isKeyPressed(sf::Keyboard::Left))
			character.move_left();

		if (Keyboard::isKeyPressed(sf::Keyboard::Right))
			character.move_right();

		if (Keyboard::isKeyPressed(sf::Keyboard::Up))
			character.move_forward();

		if (Keyboard::isKeyPressed(sf::Keyboard::Down))
			character.move_backward();
	
		if (Mouse::isButtonPressed(Mouse::Left))
			character.mouse_click();


		Vector2i position = sf::Mouse::getPosition(window);
		character.update_mouse_position(position.x-400,-(position.y-300));
		position.x=400;
		position.y=300;
		Mouse::setPosition(position,window);
		character.get_view();
		glLightfv(GL_LIGHT0, GL_POSITION, Light1Pos);
		g.draw();

        // termine la trame courante (en interne, échange les deux tampons de rendu)
        window.display();
    }

    // libération des ressources...

    return 0;
}
