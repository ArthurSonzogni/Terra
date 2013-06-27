#ifndef CHARACTER_5KCWC804
#define CHARACTER_5KCWC804

#include "camera.h"
#include "ray.h"
#include "grid.h"


class Character
{
	public:
		Camera camera;
		Grid* grid;

	public:
		Character();
		void set_grid(Grid* grid);
		virtual void mouse_click();
		virtual void update_mouse_position(int x, int y);
		virtual void move_forward();
		virtual void move_backward();
		virtual void move_left();
		virtual void move_right(); 

		void get_view();
};


#endif /* end of include guard: CHARACTER_5KCWC804 */

