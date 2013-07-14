#include "game_physic.h"

Game_physic::Game_physic()
{
	broadphase = new btDbvtBroadphase();
 
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(
			dispatcher,
			broadphase,
			solver,
			collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0,0,-10));
	world_mesh=NULL;
}

Game_physic::~Game_physic()
{
	// delete sphere
	int i;
	for(i=0;i<sphere.size();++i)
	{
		remove_sphere(i);
	}

	// delete bullet physic element
	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
}

void Game_physic::add_sphere(int x, int y, int z)
{
	btCollisionShape* sphereShape = new btSphereShape(1);
	btDefaultMotionState* motionState = new btDefaultMotionState(
													btTransform(
														btQuaternion(0,0,0,1),
														btVector3(x,y,z)
														)
													);
	btScalar mass = 1;
	btVector3 inertia(0,0,0);
	sphereShape->calculateLocalInertia(mass,inertia);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			mass,
			motionState,
			sphereShape,
			inertia);
	btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
	rigidBody->setRollingFriction(0.1);
	//rigidBody->setDamping(0,1);
	dynamicsWorld->addRigidBody(rigidBody);
	
	sphere.push_back(rigidBody);
	//sphere.back()->applyTorqueImpulse(btVector3(10,0,0));
}
void Game_physic::remove_sphere(int index)
{
	btRigidBody* rigidBody=sphere[index];
	if (rigidBody!=NULL)
	{
		dynamicsWorld->removeRigidBody(rigidBody);
		delete rigidBody->getMotionState();
		delete rigidBody;
		sphere[index]=NULL;
	}
}

btTransform Game_physic::get_sphere_transformation(int index)
{
    btTransform trans;
	sphere[index]->getMotionState()->getWorldTransform(trans);
	return trans;
}

void Game_physic::stepSimulation(float time)
{
	dynamicsWorld->stepSimulation(time,2);
	//sphere[0]->applyCentralForce(btVector3(5,0,0));
	//sphere[0]->applyTorqueImpulse(btVector3(0.1,0,0));
}

void Game_physic::delete_world_mesh()
{
	if (world_mesh!=NULL)
	{
		delete world_mesh;	
		world_mesh=NULL;
	}
}

void Game_physic::set_world_mesh(btBvhTriangleMeshShape* mesh)
{
	if (world_mesh) delete_world_mesh();
	world_mesh=mesh;
	btDefaultMotionState* motionState = new btDefaultMotionState(
													btTransform(
														btQuaternion(0,0,0,1),
														btVector3(0,0,0)
														)
													);
	btScalar mass = 0;
	btVector3 inertia(0,0,0);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(
			mass,
			motionState,
			world_mesh,
			inertia);
	btRigidBody* rigidBody = new btRigidBody(rigidBodyCI);
	dynamicsWorld->addRigidBody(rigidBody);
	
}

void Game_physic::sphere_applyTorque(int index, float dx, float dy, float dz)
{
	if (index<0 or index>=sphere.size()) return;
	if (!sphere[index]) return ;
	sphere[index]->applyTorqueImpulse(btVector3(dx,dy,dz));
}
void Game_physic::sphere_applyImpulse(int index, float dx, float dy, float dz)
{
	if (index<0 or index>=sphere.size()) return;
	if (!sphere[index]) return ;
	sphere[index]->applyCentralImpulse(btVector3(dx,dy,dz));
}
