#include "grid.h"

Grid::Grid():
	dimx(0),
	dimy(0),
	dimz(0),
	is_initiate(false),
	active(NULL),
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

		// free active
		for(int x=0;x<=dimx;++x)
		{
			for(int y=0;y<=dimy;++y)
			{
				delete[] active[x][y];
			}
			delete[] active[x];
		}
		delete[] active;

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
	
	// initiate active point
	active=new bool**[dimx+1];
	for(int x=0;x<=dimx;++x)
	{
		active[x]=new bool*[dimy+1];
		for(int y=0;y<=dimy;++y)
		{
			active[x][y]=new bool[dimz+1];
			for(int z=0;z<=dimz;++z)
			{
				active[x][y][z]=false;
			}
		}
	}
	

	// initiate filled
	filled=new bool**[dimx+2];
	for(int x=0;x<=1+dimx;++x)
	{
		filled[x]=new bool*[dimy+2];
		for(int y=0;y<=1+dimy;++y)
		{
			filled[x][y]=new bool[dimz+2];
			for(int z=0;z<=1+dimz;++z)
			{
				filled[x][y][z]=false;
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

bool Grid::getNfilled(int x, int y, int z)
{
	int n=0;
	if (filled[x-1][y][z]) ++n;
	if (filled[x+1][y][z]) ++n;
	if (filled[x][y-1][z]) ++n;
	if (filled[x][y+1][z]) ++n;
	if (filled[x][y][z-1]) ++n;
	if (filled[x][y][z+1]) ++n;
	return n;
}

void Grid::update(int x,int y,int z)
{
	if (filled[x][y][z])
	{
		return;
	}
}


void Grid::block_semi_active(int x, int y, int z, int r, int g, int b)
{
	if (isPositionValid(x,y,z))
	{
		filled[x][y][z]=true;
	}
}
void Grid::block_active(int x, int y, int z, int r, int g, int b)
{
	if (isPositionValid(x,y,z))
	{
		filled[x][y][z]=true;
		active[x][y][z]=true;
		active[x-1][y][z]=true;
		active[x][y-1][z]=true;
		active[x][y][z-1]=true;
		active[x-1][y-1][z]=true;
		active[x][y-1][z-1]=true;
		active[x-1][y][z-1]=true;
		active[x-1][y-1][z-1]=true;
	}
}
void Grid::block_delete(int x, int y, int z)
{
	if (isPositionValid(x,y,z))
	{
		active[x][y][z]=false;
		active[x-1][y][z]=false;
		active[x][y-1][z]=false;
		active[x][y][z-1]=false;
		active[x-1][y-1][z]=false;
		active[x][y-1][z-1]=false;
		active[x-1][y][z-1]=false;
		active[x-1][y-1][z-1]=false;
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
	
	glLoadIdentity();
	glNewList(display_list,GL_COMPILE);
	glPushMatrix();
		for(x=1;x<=dimx;++x)
		for(y=1;y<=dimy;++y)
		for(z=1;z<=dimz;++z)
		{
			glTranslatef(0.01f,0.f,0.f);
			generate_display_list(x,y,z);
		}

	glPopMatrix();
	glEndList();;
}

void Grid::generate_display_list(int x,int y, int z)
{
	glBegin(GL_TRIANGLES);
		glColor3f(1.0, 1.0, 1.0); glVertex2f(0.0, 0.9);
		glColor3f(1.0, 0.0, 0.0); glVertex2f(-0.5, 0.3);
		glColor3f(0.0, 1.0, 0.0); glVertex2f(0.5, 0.3);
	glEnd();
	glBegin(GL_QUADS);
		glColor3f(1.0, 0.0, 0.0); glVertex2f(-0.5, -0.8);
		glColor3f(0.0, 1.0, 0.0); glVertex2f(0.5, -0.8);
		glColor3f(0.0, 1.0, 0.0); glVertex2f(0.5, 0.3);
		glColor3f(1.0, 0.0, 0.0); glVertex2f(-0.5, 0.3);
	glEnd();
}
