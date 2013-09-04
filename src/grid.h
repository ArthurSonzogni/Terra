#ifndef GRID_V9ZCSRYS
#define GRID_V9ZCSRYS

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <iostream>
#include "color_manager.h"
#include <list>
#include <btBulletDynamicsCommon.h>

using namespace std;

class Scene;
#include "scene.h"

struct IntCoord
{
	int x,y,z;
};

class Grid
{
	private:
		int dimx;
		int dimy;
		int dimz;
		
		bool is_initiate;
		 
		// grid information
		int ***filled;
		int ***texture; 
		
		
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
		void block_get_mesh(btTriangleMesh* m,int x,int y,int z);
		

		int  ghost_block_enable;
		bool ghost_block_semi;
		int  ghost_block_x;
		int  ghost_block_y;
		int  ghost_block_z;
		int  ghost_block_tex;
		void draw_block_ghost_helper(Scene& scene);
		
	public:
		Grid();
		
		list<IntCoord> getStartPointList();
		

		void set_dimension(int Dimx, int Dimy, int Dimz);
		void get_dimension(int& Dimx, int& Dimy, int& Dimz);
		void block_semi_active(int x, int y, int z, int text);
		void block_active(int x, int y, int z, int text);
		void block_delete(int x, int y, int z);
		void assignBlock(int x, int y, int z, int block, int texture);
		void block_start_point(int x, int y, int z);
		void block_end_point(int x, int y, int z);
		void draw(Scene& scene);
		
		enum
		{
			DRAW_STARTS_POINT = 1,
			DRAW_END_POINT = 2
		};
		void draw_special(unsigned int flag,Scene& scene);
		void draw_special_start_point(int x, int y , int z, Scene& scene);

		btBvhTriangleMeshShape* get_mesh();
		
		int get_filled(int x, int y, int z);
		void getPtr(int***& filledPtr, int***& texturePtr);

		~Grid();
		
		void draw_block_ghost(bool semi, int x, int y, int z, int tex);
		Grid* allocCopy();
		void copy(Grid& grid);


		// saving and loading
		void saveToFile(string filename);
		void loadFromFile(string filename);
		
};


#endif /* end of include guard: GRID_V9ZCSRYS */

