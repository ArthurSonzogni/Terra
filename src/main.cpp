#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include "grid.h"
#include "character_free_view.h"
#include <GL/glu.h>
#include <cmath>
#include "game_physic.h"
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include "texture.h"
#include "shader.h"
#include "scene.h"

#include "game_editor.h"


int andom=3;
int Random()
{
	andom+=7;
	andom*=13;
	andom+=andom%10;
	return  andom%255;
	//return 255+andom%255;
}

bool z_ok(int x,int y,int z)
{
	//return z<3;
	//return z<11-10*cos(float(x)*0.1)*cos(float(y)*0.1);
	//return z<10-10*cos(float(x+y)*0.1)*cos(float(x-y)*0.1);
	//return z<3+((x%2+x%5)%2 + (y%2+y%3)%2)%2;
	//return z<4+3*sin(float(x*y)*0.01);;
	//return z<30-abs(x-25)-abs(y-25);
	//return z<((x%20)+(y%20))%20;
	//return z<4+((Random()%10)==0?Random()%2:0);
	return z<4;
}

int main()
{

    // crée la fenêtre
    sf::RenderWindow window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, sf::ContextSettings(32));
    window.setVerticalSyncEnabled(true);

	// texture loading
	Texture_loader tl;	

	// shader
	Shader shader;
	GLuint programShadow=shader.loadProgram("shader/shadow_vertex_shader","shader/shadow_pixel_shader");
	GLuint programObject=shader.loadProgram("shader/simple_vertex_shader","shader/simple_pixel_shader");

	// opengl initialisation
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	
	float Light1Pos[4] = {-100.0f, -100.0f, -200, 1.0f};
	float Light1Dif[4] = {2.0f, 2.0f, 2.0f, 2.0f};
	float Light1Spec[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	float Light1Amb[4] = {1.0f, 1.0f, 1.0f, 1.0f};

	GLfloat  matShininess[]={1.0};

	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light1Dif);
	glLightfv(GL_LIGHT0, GL_SPECULAR, Light1Spec);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light1Amb);

	float Light1Dir[3] = {-1.0f, -1.0f, -1.0f};
	glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,Light1Dir);
	
	

	// game editor
	Game_editor game_editor;
	game_editor.levelLoadEmpty();
	game_editor.setScreen(&window);
	game_editor.setProgram(programShadow,programObject);
	game_editor.process();



	//*
	// scene
	Scene scene;
	scene.setShadowProgram(programShadow);
	scene.setObjectProgram(programObject);



	Grid g;
	int grid_dimx=50;
	int grid_dimy=50;
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
					texture_block_grass
			);
	}


	// Character creation
	Character_free_view character;
	character.set_grid(&g);
	sf::Vector2i position;
	position.x=400;
	position.y=300;
	sf::Mouse::setPosition(position,window);
	
	// Game_physic
	int nb_sphere=2;
	Game_physic gm;
	for(int i=0;i<nb_sphere;++i)
	gm.add_sphere(20+2*i,20+2*i,25);
	btBvhTriangleMeshShape* mesh;
	mesh=g.get_mesh();
	gm.set_world_mesh(mesh);
		


    // la boucle principale
    bool running = true;
	sf::Clock c;
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


		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			character.move_left();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			character.move_right();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			character.move_forward();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			character.move_backward();
	
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			character.mouse_click();
			mesh=g.get_mesh();
			gm.set_world_mesh(mesh);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
			gm.sphere_applyTorque(0,0,1.2,0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
			gm.sphere_applyTorque(0,-1.2,0,0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			gm.sphere_applyTorque(0,0,-1.2,0);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			gm.sphere_applyTorque(0,1.2,0,0);
			


		sf::Vector2i position = sf::Mouse::getPosition(window);
		character.update_mouse_position(position.x-400,-(position.y-300));
		position.x=400;
		position.y=300;
		sf::Mouse::setPosition(position,window);
		scene.setCameraMatrix(character.get_view());
		glLightfv(GL_LIGHT0,GL_POSITION,Light1Pos);
		
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
			
			GLint location = glGetUniformLocation(programObject, "tex");
			glUniform1i(location,0);
			glActiveTexture(GL_TEXTURE0);


			g.draw();
			
			{
				GLint location = glGetUniformLocation(programObject, "tex");
				glUniform1i(location,0);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D,get_texture_id(texture_ball));
				//glBindSampler(0,0);
			}
			for(int i=0;i<nb_sphere;++i)
			{
				// get transformation we need to apply on the sphere
				btTransform tr=gm.get_sphere_transformation(i);
				btScalar m[16];
				tr.getOpenGLMatrix(m);
				glm::mat4 mat=glm::mat4(
						m[0],
						m[1],
						m[2],
						m[3],
						m[4],
						m[5],
						m[6],
						m[7],
						m[8],
						m[9],
						m[10],
						m[11],
						m[12],
						m[13],
						m[14],
						m[15]
				);
				scene.pushModelViewMatrix();
				scene.multModelViewMatrix(mat);
				scene.sendModelViewMatrix();
				scene.popModelViewMatrix();
				// drawing the sphere
				GLUquadricObj *quadric=gluNewQuadric();
				gluQuadricNormals(quadric, GLU_SMOOTH);
				gluQuadricTexture(quadric, GL_TRUE);
				gluSphere(quadric, 1.0f,20,20);
				gluDeleteQuadric(quadric);
			}
		}



		double time_elapsed=c.getElapsedTime().asSeconds();;
		cout<<(time_elapsed*30.0)<<endl;
		gm.stepSimulation(1.0/30.0);
		window.display();
		sf::sleep(sf::seconds(1./30.-time_elapsed));
    }

	//*/

	return 0;
}
