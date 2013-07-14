#ifndef SCENE_YM5Y8NVL
#define SCENE_YM5Y8NVL

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include "character.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace std;


#define BINDFORSHADOW 0
#define BINDFOROBJECT 1

class Scene
{
	private:
		float projection_matrix[4][4];
		GLuint program_shadow;
		GLuint program_object;
		GLuint shadow_framebuffer;
		GLuint shadowMap_id;
		GLuint camera_matrix_id;
		GLuint light_matrix_on_object_id;
		GLuint light_matrix_on_shadow_id;
		GLuint modelview_on_object_id;
		GLuint modelview_on_shadow_id;
		

		glm::mat4 camera_projection_matrix;
		glm::mat4 light_projection_matrix;
		glm::mat4 camera_matrix;
		glm::mat4 light_matrix;
		glm::mat4 modelview_matrix;
		glm::mat4 biaisMatrix;

		int currentMode;
		list<glm::mat4> modelViewPool;
	public:
		Scene();
		~Scene();
		
		void bindForShadow();
		void bindForObject();
		void bindFor(int mode);
		
		void setShadowProgram(GLuint program);
		void setObjectProgram(GLuint program);
		
		void setCameraMatrix(glm::mat4 camera);
		
		void multModelViewMatrix(glm::mat4 m);
		void sendModelViewMatrix();
		void pushModelViewMatrix();
		void popModelViewMatrix();
		void setCameraPosition(int x, int y, int z);
};


#endif /* end of include guard: SCENE_YM5Y8NVL */
