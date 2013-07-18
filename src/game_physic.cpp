#include "game_physic.h"
#include <iostream>
using namespace std;

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
	world_body=NULL;
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

	// delete world mesh;
	delete_world_mesh();

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
	rigidBody->setDamping(0.01,0.1);
	rigidBody->setRestitution(0.2);
	rigidBody->setFriction(7.0);
	rigidBody->setAngularFactor(1.0);
	rigidBody->forceActivationState(DISABLE_DEACTIVATION);
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
		delete rigidBody->getCollisionShape();
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
	dynamicsWorld->stepSimulation(time,30);
	//sphere[0]->applyCentralForce(btVector3(5,0,0));
	//sphere[0]->applyTorqueImpulse(btVector3(0.1,0,0));
}

void Game_physic::delete_world_mesh()
{
	cout<<"je delete world_mesh"<<endl;
	if (world_body)
	{
		dynamicsWorld->removeRigidBody(world_body);
		delete world_body->getMotionState();
		delete world_body;
		delete world_mesh;
		world_body=NULL;
		world_mesh=NULL;
	}
}

void Game_physic::set_world_mesh(btBvhTriangleMeshShape* mesh)
{
	delete_world_mesh();

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
			mesh,
			inertia);
	btRigidBody* world_body = new btRigidBody(rigidBodyCI);
	world_body->setRollingFriction(0.1);
	world_body->setFriction(7.0);
	world_body->setRestitution(0.2);
	world_body->setAngularFactor(1.0);
	dynamicsWorld->addRigidBody(world_body);
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
