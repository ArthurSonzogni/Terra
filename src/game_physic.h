#ifndef GAME_PHYSIC_QCKZ2XP4
#define GAME_PHYSIC_QCKZ2XP4

#include <btBulletDynamicsCommon.h>
#include <vector>
using namespace std;
#define SPHERE_RADIUS

class Game_physic
{
	private:
		// AABB fast collision detection
		btBroadphaseInterface* broadphase;

		// Callback management, dispatcher and configuration
		btDefaultCollisionConfiguration* collisionConfiguration;
		btCollisionDispatcher* dispatcher;
		btSequentialImpulseConstraintSolver* solver;
		btDiscreteDynamicsWorld* dynamicsWorld;
		
		vector<btRigidBody*> sphere;
	public:
		Game_physic();
		~Game_physic();
			
		// sphere related function
		void add_sphere(int x, int y, int z);
		void remove_sphere(int index);
		btTransform get_sphere_transformation(int index);

		// world related function
		void stepSimulation();
		
};

#endif /* end of include guard: GAME_PHYSIC_QCKZ2XP4 */
