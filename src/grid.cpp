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
		color[xx][yy][zz]=color_manager.getColor(r,g,b);
		filled[xx][yy][zz]=0;
		int z=filled[xx][yy][zz-1];
		int Z=filled[xx][yy][zz+1];
		int y=filled[xx][yy-1][zz];
		int Y=filled[xx][yy+1][zz];
		int x=filled[xx-1][yy][zz];
		int X=filled[xx+1][yy][zz];

		int xyz=filled[xx-1][yy-1][zz-1];
		int xyZ=filled[xx-1][yy-1][zz+1];
		int xYz=filled[xx-1][yy+1][zz-1];
		int xYZ=filled[xx-1][yy+1][zz+1];
		int Xyz=filled[xx+1][yy-1][zz-1];
		int XyZ=filled[xx+1][yy-1][zz+1];
		int XYz=filled[xx+1][yy+1][zz-1];
		int XYZ=filled[xx+1][yy+1][zz+1];

		int n=0;
		if ((x&VERTICE_Xyz) || (y&VERTICE_xYz) || (z&VERTICE_xyZ)) n|=VERTICE_xyz;
		if ((x&VERTICE_XyZ) || (y&VERTICE_xYZ) || (Z&VERTICE_xyz)) n|=VERTICE_xyZ;
		if ((x&VERTICE_XYz) || (Y&VERTICE_xyz) || (z&VERTICE_xYZ)) n|=VERTICE_xYz;
		if ((x&VERTICE_XYZ) || (Y&VERTICE_xyZ) || (Z&VERTICE_xYz)) n|=VERTICE_xYZ;
		if ((X&VERTICE_xyz) || (y&VERTICE_XYz) || (z&VERTICE_XyZ)) n|=VERTICE_Xyz;
		if ((X&VERTICE_xyZ) || (y&VERTICE_XYZ) || (Z&VERTICE_Xyz)) n|=VERTICE_XyZ;
		if ((X&VERTICE_xYz) || (Y&VERTICE_Xyz) || (z&VERTICE_XYZ)) n|=VERTICE_XYz;
		if ((X&VERTICE_xYZ) || (Y&VERTICE_XyZ) || (Z&VERTICE_XYz)) n|=VERTICE_XYZ;
		
		if (xyz&VERTICE_XYZ) n|=VERTICE_xyz;
		if (xyZ&VERTICE_XYz) n|=VERTICE_xyZ;
		if (xYz&VERTICE_XyZ) n|=VERTICE_xYz;
		if (xYZ&VERTICE_Xyz) n|=VERTICE_xYZ;
		if (Xyz&VERTICE_xYZ) n|=VERTICE_Xyz;
		if (XyZ&VERTICE_xYz) n|=VERTICE_XyZ;
		if (XYz&VERTICE_xyZ) n|=VERTICE_XYz;
		if (XYZ&VERTICE_xyz) n|=VERTICE_XYZ;

		if (semi_block_valid[n])
			filled[xx][yy][zz]|=n;
	}
}
void Grid::block_active(int x, int y, int z, int r, int g, int b)
{
	if (isPositionValid(x,y,z))
	{
		color[x][y][z]=color_manager.getColor(r,g,b);
		filled[x][y][z]=255;
		glDeleteLists(display_list,1);
		display_list=NULL;
	}
}
void Grid::block_delete(int x, int y, int z)
{
	if (isPositionValid(x,y,z))
	{
		filled[x][y][z]=0;
		glDeleteLists(display_list,1);
		display_list=NULL;
	}
}

void Grid::draw()
{
	if (display_list==0)
	{
		glPushMatrix();
		generate_display_list();
		glPopMatrix();
	}

	glCallList(display_list);
}


void Grid::generate_display_list()
{
	int x,y,z;
	if (display_list!=0)
		glDeleteLists(display_list,1);
	
	// change color model of the block
	float MatDif[4] = {0.5f, 0.5f, 0.5f, 1.0f};
	float MatAmb[4] = {0.4f, 0.4f, 0.4f, 1.0f};
	float MatSpec[4]= {0.1,0.1,0.1,1.0f};
	float MatShininess[]={1.f};

	glMaterialfv(GL_FRONT,GL_SPECULAR,MatSpec);
	glMaterialfv(GL_FRONT,GL_AMBIENT,MatAmb);
	glMaterialfv(GL_FRONT,GL_SHININESS,MatShininess);
	
	display_list=glGenLists(1);
	glLoadIdentity();
	glNewList(display_list,GL_COMPILE);
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	for(x=1;x<=dimx;++x)
	for(y=1;y<=dimy;++y)
	for(z=1;z<=dimz;++z)
		generate_display_list(x,y,z);
	glEnd();
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

		// change color model of the block
		float MatDif[4] = {0.5f, 0.5f, 0.5f, 1.0f};

		glMaterialfv(GL_FRONT,GL_DIFFUSE,color[x][y][z]);

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


			c=(c+3)%7;
			//glColor3f(Color[c],Color[c+1],Color[c+2]);
				
			
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
						triangle[0]+x,
						triangle[1]+y,
						triangle[2]+z
				);
				glVertex3f(
						triangle[3]+x,
						triangle[4]+y,
						triangle[5]+z
				);
				glVertex3f(
						triangle[6]+x,
						triangle[7]+y,
						triangle[8]+z
				);
			}
		}
	}
}

int Grid::get_filled(int x, int y, int z)
{
	if (isPositionValid(x,y,z))
		return filled[x][y][z];
	else
		return 0;
}
