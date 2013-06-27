#ifndef COLOR_MANAGER_EUY9ZRUP
#define COLOR_MANAGER_EUY9ZRUP

#include "color_manager.h"

float* Color_manager::getColor(int red, int green, int blue)
{
	int concat=0;
	concat=red | green<<8 | blue<<16;
	map<int,float*>::iterator retour;
	retour=content.find(concat);
	if (retour!=content.end())
		return retour->second;
	else
	{
		float* allocation=new float[4];
		allocation[0]=float(red)/255.0;
		allocation[1]=float(green)/255.0;
		allocation[2]=float(blue)/255.0;
		allocation[3]=1.0;
		content[concat]=allocation;
		return allocation;
	}
}

Color_manager::~Color_manager()
{
	map<int,float*>::iterator it;
	for(it=content.begin();it!=content.end();++it)
	{
		delete[] it->second;
	}
}

#endif /* end of include guard: COLOR_MANAGER_EUY9ZRUP */
