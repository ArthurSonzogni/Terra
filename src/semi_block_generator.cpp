#include "semi_block_generator.h"
#include "convexHull.h"   
#include <iostream>
using namespace std;

float* semi_block_vertice[256];
int semi_block_n_vertice[256];
int* semi_block_face[256];
int semi_block_n_face[256];

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
		// affichage
		/*cout<<endl;
		cout<<"i="<<i<<endl;
		cout<<"nb_vertice="<<semi_block_n_vertice[i]<<endl;
		cout<<"nb_face="<<semi_block_n_face[i]<<endl;*/
	}
}

semi_block_generator_filler::~semi_block_generator_filler()
{
	for(int i=0;i<256;++i)
	{
		delete[] semi_block_vertice[i];
		delete[] semi_block_face[i];
	}
}


semi_block_generator_filler filler;
