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
		
		btRigidBody* world_body;
		btBvhTriangleMeshShape* world_mesh;
		void delete_world_mesh();
	public:
		Game_physic();
		~Game_physic();
			
		// sphere related function
		void add_sphere(int x, int y, int z);
		void remove_sphere(int index);
		int getNbSphere();
		void sphere_applyTorque(int index, float dx, float dy, float dz);
		void sphere_applyTorque(int index, float dx, float dy, float dz, float angle);
		void sphere_applyImpulse(int index, float dx, float dy, float dz);
		btTransform get_sphere_transformation(int index);
		void  set_sphere_transformation(int index, btTransform tr);


		// world mesh function
		void set_world_mesh(btBvhTriangleMeshShape* mesh);

		// world related function
		void stepSimulation(float time);
		
};

#endif /* end of include guard: GAME_PHYSIC_QCKZ2XP4 */
