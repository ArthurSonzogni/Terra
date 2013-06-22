#include <iostream>
#include <SFML/OpenGL.hpp>
#include "color_manager.h"
#include <list>

using namespace std;

class Grid
{
	private:
		int dimx;
		int dimy;
		int dimz;
		
		bool is_initiate;
		 
		int ***filled;
		float ****color; 	
		
		Color_manager color_manager;

		
		void initiate();
		void free();
		
		bool isPositionValid(int x, int y, int z);
		int getNfilled(int x, int y, int z);
		int getNActive(int x, int y, int z);
		void update(int x,int y,int z);
	
		GLuint display_list;
		bool display_list_is_generated;
		void generate_display_list();
		void generate_display_list(int x,int y, int z);

	public:
		Grid();
		
		void set_dimension(int Dimx, int Dimy, int Dimz);
		void block_semi_active(int x, int y, int z, int r, int g, int b);
		void block_active(int x, int y, int z, int r, int g, int b);
		void block_delete(int x, int y, int z);
		void draw();
		
		~Grid();
};
