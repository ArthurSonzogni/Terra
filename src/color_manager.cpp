#include "color_manager.h"

float* Color_manager::getColor(int red, int green, int blue)
{
	int concat=red+green<<8+blue<<16;
	map<int,float*>::iterator retour;
	retour=content.find(concat);
	if (retour!=content.end())
		return retour->second;
	else
	{
		float* allocation=new float[3];
		allocation[0]=float(red)/255.0;
		allocation[0]=float(green)/255.0;
		allocation[0]=float(blue)/255.0;
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
