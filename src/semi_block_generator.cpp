#include "semi_block_generator.h"
#include "convexHull.h"   
#include <iostream>
using namespace std;

float* semi_block_vertice[256];
int semi_block_n_vertice[256];
int* semi_block_face[256];
int* semi_block_face_id[256];
int semi_block_n_face[256];
int semi_block_face_id_get_opposite_id[256];
int semi_block_face_id_get_opposite_rel_x[256];
int semi_block_face_id_get_opposite_rel_y[256];
int semi_block_face_id_get_opposite_rel_z[256];

class semi_block_generator_filler
{
	public:
		semi_block_generator_filler();
		~semi_block_generator_filler();
};


semi_block_generator_filler::semi_block_generator_filler()
{
	float all_vertices[3*8]=
	{
		0.f,0.f,0.f,
		0.f,0.f,1.f,
		0.f,1.f,0.f,
		0.f,1.f,1.f,
		1.f,0.f,0.f,
		1.f,0.f,1.f,
		1.f,1.f,0.f,
		1.f,1.f,1.f,
	};
	float vertices[3*8];
	int n_vertice=8;
	int i;
	// for every possible semi_cube
	for(i=0;i<256;++i)
	{
		// vertice index
		int j=0;
		// vertice index (binary position)
		int jj=1; 
		// current vertice added
		int k=0;

		for(j=0;j<8;j++)
		{
			if (jj&i)
			{
				vertices[3*k]=all_vertices[3*j];
				vertices[3*k+1]=all_vertices[3*j+1];
				vertices[3*k+2]=all_vertices[3*j+2];
				k++;		
			}
			jj<<=1;
		}

		// A 3d shape own more than 3 vertices.
		if (k<3)
		{
			semi_block_n_face[i]=0;
			semi_block_n_vertice[i]=0;
			semi_block_face[i]=NULL;
			semi_block_vertice[i]=NULL;
			continue;
		}

		Chull3D chull(vertices,k);
		chull.compute();
		chull.get_convex_hull(
				semi_block_vertice+i,
				semi_block_n_vertice+i,
				semi_block_face+i,
				semi_block_n_face+i
		);
		
		// semi_block_face_id generation
		semi_block_face_id[i]=new int[semi_block_n_face[i]];
		float current_vertice[3];
		for(int face=0;face<semi_block_n_face[i];++face)
		{
			int& id=semi_block_face_id[i][face];
			id=0;
			for(int vertice=0;vertice<3;++vertice)
			{
				int vertice_id=semi_block_face[i][3*face+vertice];
				current_vertice[0]=semi_block_vertice[i][3*vertice_id+0];
				current_vertice[1]=semi_block_vertice[i][3*vertice_id+1];
				current_vertice[2]=semi_block_vertice[i][3*vertice_id+2];
				if (current_vertice[0]==0.f)
					if (current_vertice[1]==0.f)
						if (current_vertice[2]==0.f)
							id|=VERTICE_xyz;		
						else // current_vertice[2]==1
							id|=VERTICE_xyZ;
					else // current_vertice[1]==1
						if (current_vertice[2]==0.f)
							id|=VERTICE_xYz;
						else // current_vertice[2]==1
							id|=VERTICE_xYZ;
				else // current_vertice[0]==1
					if (current_vertice[1]==0.f)
						if (current_vertice[2]==0.f)
							id|=VERTICE_Xyz;		
						else // current_vertice[2]==1
							id|=VERTICE_XyZ;
					else // current_vertice[1]==1
						if (current_vertice[2]==0.f)
							id|=VERTICE_XYz;
						else // current_vertice[2]==1
							id|=VERTICE_XYZ;
			}
		}
	}
	/*
	 *		Generate face_id_get_opposite;
	 */
	
	// fill with 0
	for(int i=0;i<255;++i)
	{
		semi_block_face_id_get_opposite_id[i]=0;
		semi_block_face_id_get_opposite_rel_x[i]=0;
		semi_block_face_id_get_opposite_rel_y[i]=0;
		semi_block_face_id_get_opposite_rel_z[i]=0;
	}

	// alias
	int* t=semi_block_face_id_get_opposite_id;	
	int* xx=semi_block_face_id_get_opposite_rel_x;
	int* yy=semi_block_face_id_get_opposite_rel_y;
	int* zz=semi_block_face_id_get_opposite_rel_z;

	// x face
	t[0b0001110]=0b11100000; xx[0b0001110]=-1;
	t[0b0001101]=0b11010000; xx[0b0001101]=-1;
	t[0b0001011]=0b10110000; xx[0b0001011]=-1;
	t[0b0000111]=0b01110000; xx[0b0000111]=-1;
	// X face
	t[0b11100000]=0b00001110; xx[0b11100000]=1;
	t[0b11010000]=0b00001101; xx[0b11010000]=1;
	t[0b10110000]=0b00001011; xx[0b10110000]=1;
	t[0b01110000]=0b00000111; xx[0b01110000]=1;
		
	// y face
	t[0b00110010]=0b11001000; yy[0b00110010]=-1;
	t[0b00110001]=0b11000100; yy[0b00110001]=-1;
	t[0b00100011]=0b10001100; yy[0b00100011]=-1;
	t[0b00010011]=0b01001100; yy[0b00010011]=-1;
	// Y face
	t[0b11001000]=0b00110010; yy[0b11001000]=1;
	t[0b11000100]=0b00110001; yy[0b11000100]=1;
	t[0b10001100]=0b00100011; yy[0b10001100]=1;
	t[0b01001100]=0b00010011; yy[0b01001100]=1;

	// z face
	t[0b01010100]=0b10101000; zz[0b01010100]=-1;
	t[0b01010001]=0b10100010; zz[0b01010001]=-1;
	t[0b01000101]=0b10001010; zz[0b01000101]=-1;
	t[0b00010101]=0b00101010; zz[0b00010101]=-1;
	// Z face
	t[0b10101000]=0b01010100; zz[0b10101000]=1;
	t[0b10100010]=0b01010001; zz[0b10100010]=1;
	t[0b10001010]=0b01000101; zz[0b10001010]=1;
	t[0b00101010]=0b00010101; zz[0b00101010]=1;

}

// I define a class that is instanciate juste after its declaration
// It permits:
// 		before the main, automaticly fill all the semi_block array
//		after the main, automaicly free the memory
semi_block_generator_filler::~semi_block_generator_filler()
{
	for(int i=0;i<256;++i)
	{
		delete[] semi_block_vertice[i];
		delete[] semi_block_face[i];
		delete[] semi_block_face_id[i];
	}
}
semi_block_generator_filler filler;
