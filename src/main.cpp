#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "grid.h"
#include "character_free_view.h"
#include <GL/glu.h>
#include <cmath>
#include "game_physic.h"
#include <SFML/System.hpp>
#include "texture.h"
#include "shader.h"


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

    // crée la fenêtre
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

	// shader
	Shader shader;
	GLuint program=shader.loadProgram("shader/simple_vertex_shader","shader/simple_pixel_shader");
	glUseProgram(program);

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
					texture_block_grass
					);
	}

	for(int k=0;k<1;++k)
	for(int x=0;x<grid_dimx;++x)
	for(int y=0;y<grid_dimy;++y)
	for(int z=0;z<grid_dimz;++z)
	{
			g.block_semi_active(x,y,z,
					texture_block_gravel
					);
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.f, 1.f, 0.01f, 500.f);
	//glOrtho(-1,1,-1,1,-100,100);

	glMatrixMode(GL_MODELVIEW);
	 
	float Light1Pos[4] = {25.f, 25.f, 1000.5, 1.0f};
	float Light1Dif[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	float Light1Spec[4] = {1.0f, 1.0f, 1.0f, 1.0f};
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
	
	// Game_physic
	Game_physic gm;
	for(int i=0;i<20;++i)
	gm.add_sphere(20+2*i,20+2*i,25);
	btBvhTriangleMeshShape* mesh;
	mesh=g.get_mesh();
	gm.set_world_mesh(mesh);
		

	// texture
	Texture_loader tl;	


    // la boucle principale
    bool running = true;
	Clock c;
    while (running)
    {
		c.restart();
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
		
		{
			GLint location = glGetUniformLocation(program, "tex");
	    	glUniform1i(location,0);
			//glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,get_texture_id(texture_ball));
			//glBindSampler(0,0);
		}
		for(int i=0;i<20;++i)
		{
			btTransform tr=gm.get_sphere_transformation(i);
			GLUquadricObj *quadric=gluNewQuadric();
			gluQuadricNormals(quadric, GLU_SMOOTH);
			gluQuadricTexture(quadric, GL_TRUE);
			btScalar m[16];
			glPushMatrix();
			tr.getOpenGLMatrix(m);
			glMultMatrixf((GLfloat*)m);
			gluSphere(quadric, 1.0f,20,20);
			glPopMatrix();
			gluDeleteQuadric(quadric);
		}
		gm.stepSimulation(1.0/30.0);
		window.display();

		double time_elapsed=c.getElapsedTime().asSeconds();;
		cout<<(time_elapsed*30.0)<<endl;
		sf::sleep(sf::seconds(1./30.-time_elapsed));
    }

	return 0;
}
