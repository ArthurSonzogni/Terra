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
	dynamicsWorld->addRigidBody(rigidBody);
	
	sphere.push_back(rigidBody);
	sphere.back()->applyTorqueImpulse(btVector3(10,0,0));
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

/*
int main2 (void)
{
  // Algorithme de tri
  btBroadphaseInterface* broadphase = new btDbvtBroadphase();
 
  // Gestion des callbacks. Le dispatcher et la configuration
  // permettent des réglages plus fins.
  btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
  btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
  btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
  btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
  dynamicsWorld->setGravity(btVector3(0,-10,0));
 
  // Nos entités
  btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
  btCollisionShape* fallShape = new btSphereShape(1);
 
  // On instancie notre sol en tant qu’entité statique.
  btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,-1,0)));
  btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0,0,0));
  btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
  dynamicsWorld->addRigidBody(groundRigidBody);
 
  // On instancie notre sphère et on lui applique une inertie.
  btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));
  btScalar mass = 1;
  btVector3 fallInertia(0,0,0);
  fallShape->calculateLocalInertia(mass,fallInertia);
  btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
  btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
  dynamicsWorld->addRigidBody(fallRigidBody);
 
  // On lance la simulation pendant 300 frames.
  for (int i=0 ; i < 300 ; i++) 
  {
    dynamicsWorld->stepSimulation(1/60.f,10);
    btTransform trans;
    fallRigidBody->getMotionState()->getWorldTransform(trans);
    std::cout << "sphere height: " << trans.getOrigin().getY() << std::endl;
  }
 
  // On libere nos entités…
  dynamicsWorld->removeRigidBody(fallRigidBody);
  delete fallRigidBody->getMotionState();
  delete fallRigidBody;
  dynamicsWorld->removeRigidBody(groundRigidBody);
  delete groundRigidBody->getMotionState();
  delete groundRigidBody;
  delete fallShape;
 
  delete groundShape;
  // … et tous les éléments de Bullet Physics.
  delete dynamicsWorld;
  delete solver;
  delete collisionConfiguration;
  delete dispatcher;
  delete broadphase;
 
  return 0;
}
*/
