#ifndef GAME_PLAY_3T24XN2U

#define GAME_PLAY_3T24XN2U
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include "grid.h"
#include "scene.h"
#include "player_group.h"
#include "character_player.h"
#include "game_physic.h"


class GamePlay
{
		enum
		{
			normal,
			exit_all,
			exit
		};
	public:
		//----- Grid loading function ----//
		int levelLoadFromFile(char* filename);
		void levelLoadFromGrid(Grid* grid);

		//----- Execution function -------//
		void process();
		int getState();

		//----- Instanciation function ---//
		GamePlay();
		~GamePlay();

		//----- Scene initilisation function ----//
		void setProgram(GLuint shadowProgram, GLuint objectProgram, GLuint skyboxProgram);
		
		void setScreen(sf::RenderWindow* w);

		//-----Network-PlayerGroup-----//
		void setPlayerGroup(PlayerGroup* p);

		void setSkyboxTextureId(GLuint id);
	private:
		Grid* grid;
		Scene scene;
		int state;
		
		Character_player character;

		Game_physic game_physic;
		btBvhTriangleMeshShape* mesh;

		GLuint objectProgram;
		GLuint shadowProgram;
		GLuint skyboxProgram;
		
		sf::RenderWindow* window;
		//----- Helper member function -------//
		void deleteGrid();	
		void deleteMesh();
		void loadMesh();

		//-----Network-PlayerGroup-----//
		PlayerGroup* playergroup;
};

#endif /* end of include guard: GAME_PLAY_3T24XN2U */
