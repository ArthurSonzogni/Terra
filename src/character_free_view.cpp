#include "character_free_view.h"

void Character_free_view::update_mouse_position(int x, int y)
{
	camera.angle_xy_change(-x*DEGTORAD/10);
	camera.angle_xz_change(y*DEGTORAD/10);
}



void Character_free_view::mouse_click()
{
	Ray ray=camera.get_ray();
	int n=0;
	int rayx,rayy,rayz;
	ray.get_current_position(&rayx,&rayy,&rayz);
	cout<<rayx<<" "<<rayy<<" "<<rayz<<endl;
	while(grid->get_filled(rayx,rayy,rayz)==0)
	{
		if (n>30) return;
		cout<<"n="<<n++<<endl;
		cout<<rayx<<" "<<rayy<<" "<<rayz<<endl;
		ray.increment();
		ray.get_current_position(&rayx,&rayy,&rayz);
	}
	if (false)
	{
		ray.decrement();
		ray.get_current_position(&rayx,&rayy,&rayz);
		grid->block_active(rayx,rayy,rayz,255,255,255);
	}
	else
	{
		grid->block_delete(rayx,rayy,rayz);
	}
}
