#include "character_free_view.h"
#include "texture.h"

Character_free_view::Character_free_view()
{
	slotElement.type=SlotElementTypeNone;
	slotElement.subtype=0;
	
}

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
		n++;
		ray.increment();
		ray.get_current_position(&rayx,&rayy,&rayz);
	}
	if (true)
	{
		ray.decrement();
		ray.get_current_position(&rayx,&rayy,&rayz);
		switch (slotElement.type)
		{
			case SlotElementTypeNone:
			{
		
			} break;
			case SlotElementTypeSemiBlock:
			{
				grid->block_semi_active(rayx,rayy,rayz,slotElement.subtype);
			} break;
			case SlotElementTypeBlock:
			{
				grid->block_active(rayx,rayy,rayz,slotElement.subtype);
			} break;
		}
	}
	else
	{
		grid->block_delete(rayx,rayy,rayz);
	}
}

void Character_free_view::step()
{
	Ray ray=camera.get_ray();
	int n=0;
	int rayx,rayy,rayz;
	ray.get_current_position(&rayx,&rayy,&rayz);
	while(grid->get_filled(rayx,rayy,rayz)==0)
	{
		if (n>30) return;
		n++;
		ray.increment();
		ray.get_current_position(&rayx,&rayy,&rayz);
	}
	if (true)
	{
		ray.decrement();
		ray.get_current_position(&rayx,&rayy,&rayz);
		switch (slotElement.type)
		{
			case SlotElementTypeNone:
			{
		
			} break;
			case SlotElementTypeSemiBlock:
			{
				grid->draw_block_ghost(true,rayx,rayy,rayz,slotElement.subtype);
			} break;
			case SlotElementTypeBlock:
			{
				grid->draw_block_ghost(false,rayx,rayy,rayz,slotElement.subtype);
			} break;
		}
	}
}

void Character_free_view::setBlockType(SlotElement& s)
{
	slotElement=s;
}
