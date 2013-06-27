#ifndef RAY_U3XLX2Q7
#define RAY_U3XLX2Q7

class Ray
{
	public:
		float x,y,z;		// ray position
		float dx,dy,dz;		// ray direction
		
		float p_x,p_y,p_z;		// ray position after incrementation
		
		Ray();
		void set_position(float X,float Y,float Z);
		void set_direction(float DX, float DY, float DZ);
		void increment();
		void get_current_position(int* X,int* Y,int *Z);
};

#endif /* end of include guard: RAY_U3XLX2Q7 */

