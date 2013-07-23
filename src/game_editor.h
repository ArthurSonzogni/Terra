#ifndef GAME_EDITOR_9627AVWI

#define GAME_EDITOR_9627AVWI
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/OpenGL.hpp>
#include "grid.h"
#include "character_free_view.h"
#include "scene.h"

using namespace sf;

enum game_editor_state
{
	game_editor_state_not_ready,
	game_editor_state_normal,
	game_editor_state_exit,
	game_editor_state_exit_all
};

class Game_editor
{
	public:
		//----- Grid loading function ----//
		int levelLoadFromFile(char* filename);
		void levelLoadFromGrid(Grid* grid);
		void levelLoadEmpty();

		//----- Grid saving function -----//
		int levelSaveToFile(char* filename);
		void levelSaveToGrid();
			
		//----- Execution function -------//
		void process();
		int getState();

		//----- Instanciation function ---//
		Game_editor();
		~Game_editor();

		//----- Scene initilisation function ----//
		void setProgram(GLuint shadowProgram, GLuint objectProgram);
		
		void setWindow(Window* w);
		

	private:

		Character_free_view character;
		Grid* grid;
		Scene scene;

		int state;

		
		GLuint objectProgram;
		GLuint shadowProgram;
		
		Window* window;

		
		//----- Helper member function -------//
		void deleteGrid();	
		
};


#endif /* end of include guard: GAME_EDITOR_9627AVWI */
