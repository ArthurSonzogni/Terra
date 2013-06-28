#ifndef CAMERA_HUWKOK3I
#define CAMERA_HUWKOK3I

#define RADTODEG 57.2957795131
#define DEGTORAD 0.01745329252
#include "ray.h"

class Camera
{
	public:
		float x,y,z;
		float angle_xy;
		float angle_xz;
		
		Camera();
		void move_forward(float increment);
		void move_laterally(float increment);
		void angle_xy_change(float increment);
		void angle_xz_change(float increment);
		Ray get_ray();

		void get_view();
};

#endif /* end of include guard: CAMERA_HUWKOK3I */

