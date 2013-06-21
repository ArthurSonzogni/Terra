#include <map>
using namespace std;

class Color_manager
{
	private:
		map<int,float*> content;
	public:
		float* getColor(int red,int green,int blue);
		~Color_manager();
		
};
