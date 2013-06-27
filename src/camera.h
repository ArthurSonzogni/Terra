class camera
{
	public:
		float x,y,z;
		float angle_xy;
		float angle_xz;
		
		camera();
		void move_forward(float increment);
		void move_laterally(float increment);
		void angle_xy_change(float increment);
		void angle_xz_change(float increment);
};
