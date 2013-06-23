#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "grid.h"

int main()
{
    // crée la fenêtre
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);
	Grid g;
	g.set_dimension(10,10,10);
	
	g.block_active(1,1,1,255,255,255);
	g.block_active(2,1,1,255,0,0);
	g.block_active(3,1,1,0,0,255);
	g.block_active(4,1,1,0,255,0);
	g.block_active(5,1,1,255,255,0);
	g.block_active(6,1,1,255,0,255);
	g.block_active(7,1,1,0,255,255);
	g.block_active(8,1,1,0,0,0);

	g.block_active(1,2,1,255,255,255);
	g.block_active(1,1,2,255,255,255);
	g.block_active(1,2,2,255,255,255);
	g.block_semi_active(2,2,1,255,255,255);
	g.block_semi_active(2,2,2,255,255,255);
	g.block_semi_active(1,2,1,255,255,255);


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);
	glMatrixMode(GL_MODELVIEW);
	glShadeModel(GL_SMOOTH);


	 
	float Light1Pos[4] = {0.0f, 1.f, -3.f, 1.0f};
	float Light1Dif[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	float Light1Spec[4] = {1.0f, 1.f, 1.f, 1.0f};
	float Light1Amb[4] = {0.1f, 0.1f, 0.1f, 1.0f};

	GLfloat  matShininess[]={50.0};
	 
	 
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light1Dif); 	//Et ceux de la lumière
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light1Spec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light1Amb);


	glLightfv(GL_LIGHT0, GL_POSITION, Light1Pos);
	float Light1Dir[3] = {0.0f, -1.0f, 1.0f};
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Light1Dir);

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
        }

        // effacement les tampons de couleur/profondeur
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		g.draw();

        // termine la trame courante (en interne, échange les deux tampons de rendu)
        window.display();
    }

    // libération des ressources...

    return 0;
}
