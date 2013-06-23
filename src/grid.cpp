#include "grid.h"
#include "semi_block_generator.h"

Grid::Grid():
	dimx(0),
	dimy(0),
	dimz(0),
	is_initiate(false),
	filled(NULL),
	color(NULL),
	color_manager(),
	display_list(0)
{
}

void Grid::free()
{
	if (display_list!=0)
		glDeleteLists(display_list,1);

	if (is_initiate)
	{
		is_initiate=false;	

		// free filled
		for(int x=0;x<=1+dimx;++x)
		{
			for(int y=0;y<=1+dimy;++y)
			{
				delete[] filled[x][y];
			}
			delete[] filled[x];
		}
		delete[] filled;


		// free color
		for(int x=0;x<=1+dimx;++x)
		{
			for(int y=0;y<=1+dimy;++y)
			{
				delete[] color[x][y];
			}
			delete[] color[x];
		}
		delete[] color;
	}
}

void Grid::initiate()
{
	if (is_initiate)
		free();

	// initiate filled
	filled=new int**[dimx+2];
	for(int x=0;x<=1+dimx;++x)
	{
		filled[x]=new int*[dimy+2];
		for(int y=0;y<=1+dimy;++y)
		{
			filled[x][y]=new int[dimz+2];
			for(int z=0;z<=1+dimz;++z)
			{
				filled[x][y][z]=0;
			}
		}
	}

	// initiate color
	color=new float***[dimx+2];
	for(int x=0;x<=1+dimx;++x)
	{
		color[x]=new float**[dimy+2];
		for(int y=0;y<=1+dimy;++y)
		{
			color[x][y]=new float*[dimz+2];
			for(int z=0;z<=1+dimz;++z)
			{
				color[x][y][z]=NULL;
			}
		}
	}
	
	is_initiate=true;
}


void Grid::set_dimension(int Dimx,int Dimy,int Dimz)
{
	dimx=Dimx;
	dimy=Dimy;
	dimz=Dimz;
	initiate();
}

Grid::~Grid()
{
	free();
}

bool Grid::isPositionValid(int x, int y, int z)
{
	return	x>=1 &&
			y>=1 &&
			z>=1 &&
			x<=dimx &&
			y<=dimy &&
			z<=dimz;
}

void Grid::update(int x,int y,int z)
{
	if (filled[x][y][z])
	{
		return;
	}
}


void Grid::block_semi_active(int xx, int yy, int zz, int r, int g, int b)
{
	if (isPositionValid(xx,yy,zz))
	{
		filled[xx][yy][zz]=0;
		int z=filled[xx][yy][zz-1];
		int Z=filled[xx][yy][zz+1];
		int y=filled[xx][yy-1][zz];
		int Y=filled[xx][yy+1][zz];
		int x=filled[xx-1][yy][zz];
		int X=filled[xx+1][yy][zz];
		int n=0;
		if (x&VERTICE_Xyz || y&VERTICE_xYz || z&VERTICE_xyZ) n|=VERTICE_xyz;
		if (x&VERTICE_XyZ || y&VERTICE_xYZ || Z&VERTICE_xyz) n|=VERTICE_xyZ;
		if (x&VERTICE_XYz || Y&VERTICE_xyz || z&VERTICE_xYZ) n|=VERTICE_xYz;
		if (x&VERTICE_XYZ || Y&VERTICE_xyZ || Z&VERTICE_xYz) n|=VERTICE_xYZ;
		if (X&VERTICE_xyz || y&VERTICE_XYz || z&VERTICE_XyZ) n|=VERTICE_Xyz;
		if (X&VERTICE_xyZ || y&VERTICE_XYZ || Z&VERTICE_Xyz) n|=VERTICE_XyZ;
		if (X&VERTICE_xYz || Y&VERTICE_Xyz || z&VERTICE_XYZ) n|=VERTICE_XYz;
		if (X&VERTICE_xYZ || Y&VERTICE_XyZ || Z&VERTICE_XYz) n|=VERTICE_XYZ;
		filled[xx][yy][zz]=n;
	}
}
void Grid::block_active(int x, int y, int z, int r, int g, int b)
{
	if (isPositionValid(x,y,z))
	{
		filled[x][y][z]=255;
	}
}
void Grid::block_delete(int x, int y, int z)
{
	if (isPositionValid(x,y,z))
	{
		filled[x][y][z]=255;
	}
}

void Grid::draw()
{
	static float angle=0;

	angle+=0.6+angle*0.001;
	glLoadIdentity();
	glRotatef(angle,1,0.1,0);
	glRotatef(angle,0.1,1.0,0);
	if (display_list==0)
		generate_display_list();

	glCallList(display_list);
}


void Grid::generate_display_list()
{
	int x,y,z;
	if (display_list!=0)
		glDeleteLists(display_list,1);
	
	display_list=glGenLists(1);
	glLoadIdentity();
	glNewList(display_list,GL_COMPILE);
	glScalef(0.15,0.15,0.15);
	glPushMatrix();
		for(x=1;x<=dimx;++x)
		{
			glPushMatrix();
			for(y=1;y<=dimy;++y)
			{
				glPushMatrix();
				for(z=1;z<=dimz;++z)
				{
					glTranslatef(0.01f,0.f,0.f);
					generate_display_list(x,y,z);
					glTranslatef(0.f,0.f,1.f);
				}
				glPopMatrix();
				glTranslatef(0.f,1.f,0.f);
			}
			glPopMatrix();
			glTranslatef(1.f,0.f,0.f);
		}
	glPopMatrix();
	glEndList();;
	
}

float Color[9]=
{
	1.0,0.0,0.0,
	1.0,0.0,1.0,
	1.0,0.0,0.1,
};


inline void triangle_get_normal(float t[9],float* x,float* y,float* z)
{
	/*
	 *   v1    v2
	 *   (0-3) (0-6)
	 *   (1-4) (1-7)
	 *   (2-5) (2-8)
	 */
	*x=(t[1]-t[4])*(t[2]-t[8]) - (t[1]-t[7])*(t[2]-t[5]);
	*y=(t[2]-t[5])*(t[0]-t[6]) - (t[2]-t[8])*(t[0]-t[3]);
	*z=(t[0]-t[3])*(t[1]-t[7]) - (t[0]-t[6])*(t[1]-t[4]);

}

void Grid::generate_display_list(int x,int y, int z)
{
	int i,j,k;
	if (filled[x][y][z])
	{
		if (filled[x-1][y][z]==255 &&
			filled[x+1][y][z]==255 &&
			filled[x][y-1][z]==255 &&
			filled[x][y+1][z]==255 &&
			filled[x][y][z-1]==255 &&
			filled[x][y][z+1]==255)
		return;

		static int c=0;
		i=filled[x][y][z];
		for(j=0;j<semi_block_n_face[i];++j)
		{
			// if this triangle touch another triangle of the opposite
			// semi_block, we abort its display.
			int face_id=semi_block_face_id[i][j];
			int op_id=semi_block_face_id_get_opposite_id[face_id];	
			int rel_x=semi_block_face_id_get_opposite_rel_x[face_id];	
			int rel_y=semi_block_face_id_get_opposite_rel_y[face_id];	
			int rel_z=semi_block_face_id_get_opposite_rel_z[face_id];	
			if (op_id!=0 and 
				((filled[x+rel_x][y+rel_y][z+rel_z] & op_id) == op_id))
				continue;

			glBegin(GL_TRIANGLES);
			c=(c+3)%7;
			glColor3f(Color[c],Color[c+1],Color[c+2]);
			float triangle[9];
			for(k=0;k<3;k++)
			{
				int vertice=semi_block_face[i][3*j+k];
				triangle[3*k+0]=semi_block_vertice[i][3*vertice+0];
				triangle[3*k+1]=semi_block_vertice[i][3*vertice+1];
				triangle[3*k+2]=semi_block_vertice[i][3*vertice+2];

			}
			// normal computation
			{
				float nx,ny,nz;
				triangle_get_normal(triangle,&nx,&ny,&nz);
				glNormal3f(nx,ny,nz);
			}
			// triangle draw
			{
				glVertex3f(
						triangle[0],
						triangle[1],
						triangle[2]
				);
				glVertex3f(
						triangle[3],
						triangle[4],
						triangle[5]
				);
				glVertex3f(
						triangle[6],
						triangle[7],
						triangle[8]
				);
			}
			glEnd();
		}
	}
}

