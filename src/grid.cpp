#include "grid.h"
#include "semi_block_generator.h"
#include "texture.h"

Grid::Grid():
	dimx(0),
	dimy(0),
	dimz(0),
	is_initiate(false),
	filled(NULL),
	texture(NULL),
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


		// free texture
		for(int x=0;x<=1+dimx;++x)
		{
			for(int y=0;y<=1+dimy;++y)
			{
				delete[] texture[x][y];
			}
			delete[] texture[x];
		}
		delete[] texture;
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

	// initiate texture
	texture=new float**[dimx+2];
	for(int x=0;x<=1+dimx;++x)
	{
		texture[x]=new float*[dimy+2];
		for(int y=0;y<=1+dimy;++y)
		{
			texture[x][y]=new float[dimz+2];
			for(int z=0;z<=1+dimz;++z)
			{
				texture[x][y][z]=0;
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


void Grid::block_semi_active(int xx, int yy, int zz, int text)
{
	if (isPositionValid(xx,yy,zz))
	{
		if (filled[xx][yy][zz]==255) return;
		texture[xx][yy][zz]=text;
		
		int n=0;
		int x,y,z;
		int vx,vy,vz;
		int dx,dy,dz;
		for(x=-1;x<=1;++x)
		for(y=-1;y<=1;++y)
		for(z=-1;z<=1;++z)
		{
			for(vx=0;vx<=1;vx++)
			for(vy=0;vy<=1;vy++)
			for(vz=0;vz<=1;vz++)
			{
				if (get_vertice[vx][vy][vz] & filled[xx+x][yy+y][zz+z])
				{
					dx=x+vx;
					dy=y+vy;
					dz=z+vz;
					if ((dx==0 or dx==1) and
						(dy==0 or dy==1) and
						(dz==0 or dz==1))
					{
						n|=get_vertice[dx][dy][dz];
					}
				}
			}
		}

		if (semi_block_valid[n])
			filled[xx][yy][zz]|=n;
	}
}
void Grid::block_active(int x, int y, int z, int text)
{
	if (isPositionValid(x,y,z))
	{
		texture[x][y][z]=text;
		filled[x][y][z]=255;
		glDeleteLists(display_list,1);
		display_list=0;
	}
}
void Grid::block_delete(int x, int y, int z)
{
	if (isPositionValid(x,y,z))
	{
		filled[x][y][z]=0;
		glDeleteLists(display_list,1);
		display_list=0;
	}
}

void Grid::draw()
{
	if (display_list==0)
		generate_display_list();

	glCallList(display_list);
}


void Grid::generate_display_list()
{
	int x,y,z;
	if (display_list!=0)
		glDeleteLists(display_list,1);
	
	// change color model of the block
	float MatDif[4] = {1.0f, 1.0f, 1.0f, 1.0f};
	float MatAmb[4] = {0.3f, 0.3f, 0.3f, 1.0f};
	float MatSpec[4]= {0.5,0.5,0.5,1.0f};
	float MatShininess[]={128.f};

	glMaterialfv(GL_FRONT,GL_SPECULAR,MatSpec);
	glMaterialfv(GL_FRONT,GL_AMBIENT,MatAmb);
	glMaterialfv(GL_FRONT,GL_SHININESS,MatShininess);
	
	display_list=glGenLists(1);
	glLoadIdentity();
	glNewList(display_list,GL_COMPILE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,get_texture_id(texture_block));
	glBegin(GL_TRIANGLES);
	for(x=1;x<=dimx;++x)
	for(y=1;y<=dimy;++y)
	for(z=1;z<=dimz;++z)
		generate_display_list(x,y,z);
	glEnd();
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

		//glMaterialfv(GL_FRONT,GL_DIFFUSE,color[x][y][z]);

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
			float texture_position[2];
			texture_block_get_position(texture[x][y][z],texture_position);
			{
				glTexCoord2d(
							texture_position[0]+triangle[0]*TEXT_X_INC2,
							texture_position[1]+triangle[1]*TEXT_Y_INC2
				);
				glVertex3f(
						triangle[0]+x,
						triangle[1]+y,
						triangle[2]+z
				);
				glTexCoord2d(
							texture_position[0]+triangle[3]*TEXT_X_INC2,
							texture_position[1]+triangle[4]*TEXT_Y_INC2
				);
				glVertex3f(
						triangle[3]+x,
						triangle[4]+y,
						triangle[5]+z
				);
				glTexCoord2d(
							texture_position[0]+triangle[6]*TEXT_X_INC2,
							texture_position[1]+triangle[7]*TEXT_Y_INC2
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

btBvhTriangleMeshShape* Grid::get_mesh()
{
	btTriangleMesh* m = new btTriangleMesh();
	int x,y,z;
	for(x=1;x<=dimx;++x)
	for(y=1;y<=dimy;++y)
	for(z=1;z<=dimz;++z)
		block_get_mesh(m,x,y,z);

	btBvhTriangleMeshShape* shape = new btBvhTriangleMeshShape(m,true,true);
	return shape;
}
void Grid::block_get_mesh(btTriangleMesh* m,int x, int y,int z)
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
			// semi_block, we abort its computation
			int face_id=semi_block_face_id[i][j];
			int op_id=semi_block_face_id_get_opposite_id[face_id];	
			int rel_x=semi_block_face_id_get_opposite_rel_x[face_id];	
			int rel_y=semi_block_face_id_get_opposite_rel_y[face_id];	
			int rel_z=semi_block_face_id_get_opposite_rel_z[face_id];	
			if (op_id!=0 and 
				((filled[x+rel_x][y+rel_y][z+rel_z] & op_id) == op_id))
				continue;

			float triangle[9];
			for(k=0;k<3;k++)
			{
				int vertice=semi_block_face[i][3*j+k];
				triangle[3*k+0]=semi_block_vertice[i][3*vertice+0];
				triangle[3*k+1]=semi_block_vertice[i][3*vertice+1];
				triangle[3*k+2]=semi_block_vertice[i][3*vertice+2];

			}
			// triangle computation
			{
				btVector3 A(
						triangle[0]+x,
						triangle[1]+y,
						triangle[2]+z
				);
				btVector3 B(
						triangle[3]+x,
						triangle[4]+y,
						triangle[5]+z
				);
				btVector3 C(
						triangle[6]+x,
						triangle[7]+y,
						triangle[8]+z
				);
				m->addTriangle(A,B,C,false);
			}
		}
	}
}


	
