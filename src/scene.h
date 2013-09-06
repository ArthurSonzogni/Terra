#ifndef SCENE_YM5Y8NVL
#define SCENE_YM5Y8NVL

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include "character.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <list>
using namespace std;

#define BINDFORSHADOW 0
#define BINDFORSKYBOX 1
#define BINDFOROBJECT 2

class Scene
{
	private:
		float projection_matrix[4][4];
		GLuint program_shadow;
		GLuint program_object;
		GLuint program_skybox;
		GLuint shadow_framebuffer;
		GLuint shadow_depthTexture;
		GLuint shadowMap_id;
		GLuint camera_matrix_id;
		GLuint camera_matrix_without_projection_id;
		GLuint light_matrix_on_object_id;
		GLuint light_matrix_on_shadow_id;
		GLuint modelview_on_object_id;
		GLuint modelview_on_shadow_id;
		GLuint skybox_tex_id;
		GLuint skybox_mat_id;
		GLuint skybox_texture_id;
		

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
		void bindForSkybox();

		void bindFor(int mode);
		int getBinding();
		
		void setShadowProgram(GLuint program);
		void setObjectProgram(GLuint program);
		void setSkyBoxProgram(GLuint program);
		
		void setCameraMatrix(glm::mat4 camera);
		

		void setModelViewMatrix(glm::mat4& m);
		void multModelViewMatrix(glm::mat4& m);
		void sendModelViewMatrix();
		void pushModelViewMatrix();
		void popModelViewMatrix();
		void setCameraPosition(int x, int y, int z);
		
		void setSkyboxTextureId(GLuint id);
		void performSkyboxDrawing();
};


#endif /* end of include guard: SCENE_YM5Y8NVL */
