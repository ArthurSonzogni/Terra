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
	
	g.block_active(1,2,1,255,255,255);
	g.block_active(2,1,1,255,255,255);
	g.block_active(1,2,1,255,255,255);
	g.block_active(1,1,2,255,255,255);
	g.block_active(3,1,2,255,255,255);
	g.block_active(4,1,2,255,255,255);
	g.block_active(5,1,2,255,255,255);
	g.block_active(6,1,2,255,255,255);
	g.block_active(7,1,2,255,255,255);

	g.block_semi_active(1,1,1,255,255,255);
	g.block_semi_active(2,2,1,255,255,255);
	g.block_semi_active(1,2,2,255,255,255);
	g.block_semi_active(2,1,2,255,255,255);
	g.block_semi_active(2,2,2,255,255,255);
	
	//g.block_semi_active(,255,255,255);
    // chargement des ressources, initialisation des états OpenGL, ...
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CW);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);


    //Add ambient light
	GLfloat ambientColor[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

	//Add positioned light
	GLfloat lightPos0[] = {0.f,10.f,0.0f, 1.0f};
	GLfloat lightColor0[] = {1.0f, 0.5f, 0.5f, 1.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = {0.5f, 0.5f, 0.5f, 1.6f}; //Color (0.5, 0.2, 0.2)

	//Coming from the direction (-1, 0.5, 0.5)
	GLfloat lightPos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	


	glEnable(GL_NORMALIZE);

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
