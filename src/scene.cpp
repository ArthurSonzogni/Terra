#include "scene.h"

Scene::Scene():
	program_shadow(0),
	program_object(0),
	shadow_framebuffer(0),
	shadowMap_id(0),
	camera_matrix_id(0),
	light_matrix_on_object_id(0),
	light_matrix_on_shadow_id(0),
	modelview_on_object_id(0),
	modelview_on_shadow_id(0),
	skybox_tex_id(0),
	skybox_mat_id(0),
	skybox_texture_id(0),
	currentMode(BINDFORSHADOW)
{
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.  
	shadow_framebuffer=0;
	glGenFramebuffers(1, &shadow_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, shadow_framebuffer);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	glGenTextures(1, &shadow_depthTexture);
	glBindTexture(GL_TEXTURE_2D, shadow_depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0,GL_DEPTH_COMPONENT32, 1024, 1024, 0,GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,shadow_depthTexture, 0);

	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout<<"error enable to load depth buffer"<<endl;

	// setting up default matrix;
	setCameraPosition(0,0,0);
	camera_projection_matrix=glm::perspective<float>(70.f, 800.f/600.f, 0.1f, 500.f);
	camera_matrix=glm::lookAt(
			glm::vec3(0,0,0),
			glm::vec3(1,1,1),
			glm::vec3(0,0,1)
	);

	//light_projection_matrix=glm::perspective<float>(70.f, 1.f, 0.01f, 500.f);
	light_projection_matrix=glm::ortho<float>(-25,25,-25,25,0,100);
	
	biaisMatrix=glm::mat4
	(
		0.5,0,0,0,
		0,0.5,0,0,
		0,0,0.5,0,
		0.5,0.5,0.5,1
	);

}

Scene::~Scene()
{
	glDeleteFramebuffers(1,&shadow_framebuffer);
	glDeleteTextures(1,&shadow_depthTexture);
}

void Scene::bindForShadow()
{
	currentMode=BINDFORSHADOW;

	// resetting the modelview matrix
	modelview_matrix=glm::mat4(1.0);

	glViewport(0,0,1024,1024);
	glBindFramebuffer(GL_FRAMEBUFFER,shadow_framebuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program_shadow);

	// sending the projection matrix for light
	glm::mat4 projection_matrix=light_projection_matrix*light_matrix;
	glUniformMatrix4fv(light_matrix_on_shadow_id,
			1,GL_FALSE,&(projection_matrix[0][0]));

	// sending information for the modelview
	glUniformMatrix4fv(modelview_on_shadow_id,
			1,GL_FALSE,&(modelview_matrix[0][0]));

}

void Scene::bindForObject()
{
	currentMode=BINDFOROBJECT;

	// resetting the modelview matrix
	modelview_matrix=glm::mat4(1.0);

	glViewport(0,0,800,600);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
    glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(program_object);
	
	// sending information for light matrix
	glm::mat4 projection_matrix=biaisMatrix*light_projection_matrix*light_matrix;
	glUniformMatrix4fv(light_matrix_on_object_id,
			1,GL_FALSE,&(projection_matrix[0][0]));

	// sending information for camera matrix
	projection_matrix=camera_projection_matrix*camera_matrix;
	glUniformMatrix4fv(camera_matrix_id,
			1,GL_FALSE,&(projection_matrix[0][0]));

	// sending information for camera matrix
	projection_matrix=camera_matrix;
	glUniformMatrix4fv(camera_matrix_without_projection_id,
			1,GL_FALSE,&(projection_matrix[0][0]));

	// sending information for the modelview
	glUniformMatrix4fv(modelview_on_object_id,
			1,GL_FALSE,&(modelview_matrix[0][0]));

	// sending the buffer storing the shadow
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,shadow_depthTexture);
	glUniform1i(shadowMap_id,1);


}

void Scene::bindForSkybox()
{
	currentMode=BINDFORSKYBOX;

	glViewport(0,0,800,600);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glUseProgram(program_skybox);

	// resetting the modelview matrix
	modelview_matrix=glm::mat4(1.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	// sending information for camera matrix
	glm::mat4 mat=camera_matrix*modelview_matrix;
	mat[3][0]=0.0;
	mat[3][1]=0.0;
	mat[3][2]=0.0;
	mat=camera_projection_matrix*mat;

	glUniformMatrix4fv(skybox_mat_id,1,GL_FALSE,&(mat[0][0]));
	
	// sending tex
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(skybox_tex_id,0);
	
}

void Scene::setShadowProgram(GLuint program)
{
	program_shadow=program;
	glUseProgram(program);
	
	// getting id
	light_matrix_on_shadow_id=glGetUniformLocation(program,"light_mat");
	modelview_on_shadow_id=glGetUniformLocation(program,"modelview_mat");
}

void Scene::setObjectProgram(GLuint program)
{
	glUseProgram(program);
	program_object=program;

	// getting id
	shadowMap_id=glGetUniformLocation(program,"shadowMap");
	camera_matrix_id=glGetUniformLocation(program,"camera_mat");
	camera_matrix_without_projection_id=glGetUniformLocation(program,"camera_mat_without_projection");
	light_matrix_on_object_id=glGetUniformLocation(program,"light_mat");
	modelview_on_object_id=glGetUniformLocation(program,"modelview_mat");
}

void Scene::setSkyBoxProgram(GLuint program)
{
	glUseProgram(program);
	program_skybox=program;

	// getting id
	skybox_mat_id=glGetUniformLocation(program,"mat");	
	skybox_tex_id=glGetUniformLocation(program,"tex");
}

void Scene::bindFor(int mode)
{
	switch (mode)
	{
		case BINDFORSHADOW: bindForShadow(); break;
		case BINDFORSKYBOX: bindForSkybox(); break;
		case BINDFOROBJECT: bindForObject(); break;
	}
}

void Scene::setCameraMatrix(glm::mat4 camera)
{
	camera_matrix=camera;
}


void Scene::multModelViewMatrix(glm::mat4& m)
{
	modelview_matrix*=m;
}
void Scene::setModelViewMatrix(glm::mat4& m)
{
	modelview_matrix=m;
}

void Scene::sendModelViewMatrix()
{
	if (currentMode==BINDFORSHADOW)
	{
		// sending information for the modelview
		glUniformMatrix4fv(modelview_on_shadow_id,
				1,GL_FALSE,&(modelview_matrix[0][0]));

	}
	else if (currentMode==BINDFOROBJECT)
	{
		// sending information for the modelview
		glUniformMatrix4fv(modelview_on_object_id,
				1,GL_FALSE,&(modelview_matrix[0][0]));
	}
}

void Scene::pushModelViewMatrix()
{
	modelViewPool.push_front(modelview_matrix);
}

void Scene::popModelViewMatrix()
{
	modelview_matrix=modelViewPool.front();
	modelViewPool.pop_front();
}

void Scene::setCameraPosition(int x, int y, int z)
{
	light_matrix=glm::lookAt(
			glm::vec3(x-5,y-5,z+10),
			glm::vec3(x+6,y+5,z-8),
			glm::vec3(0,0,1)
	);
}
int Scene::getBinding()
{
	return currentMode;
}

void Scene::setSkyboxTextureId(GLuint id)
{
	skybox_texture_id=id;
}
void Scene::performSkyboxDrawing()
{
	cout<<"hi"<<endl;
	glBindTexture(GL_TEXTURE_2D, skybox_texture_id);
	const float length_sup=1.0;
	const float length_inf=1.0;
	// Render the front quad
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(  length_sup, -length_sup, -length_inf );
		glTexCoord2f(0, 1); glVertex3f(  length_sup,  length_sup, -length_inf );
		glTexCoord2f(1, 1); glVertex3f( -length_sup,  length_sup, -length_inf );
		glTexCoord2f(1, 0); glVertex3f( -length_sup, -length_sup, -length_inf );

	// Render the left quad
		glTexCoord2f(0, 0); glVertex3f(  length_inf, -length_sup,  length_sup );
		glTexCoord2f(0, 1); glVertex3f(  length_inf,  length_sup,  length_sup );
		glTexCoord2f(1, 1); glVertex3f(  length_inf,  length_sup, -length_sup );
		glTexCoord2f(1, 0); glVertex3f(  length_inf, -length_sup, -length_sup );

	// Render the back quad
		glTexCoord2f(0, 0); glVertex3f( -length_sup, -length_sup,  length_inf );
		glTexCoord2f(0, 1); glVertex3f( -length_sup,  length_sup,  length_inf );
		glTexCoord2f(1, 1); glVertex3f(  length_sup,  length_sup,  length_inf );
		glTexCoord2f(1, 0); glVertex3f(  length_sup, -length_sup,  length_inf );

	// Render the right quad
		glTexCoord2f(0, 0); glVertex3f( -length_inf, -length_sup, -length_sup );
		glTexCoord2f(0, 1); glVertex3f( -length_inf,  length_sup, -length_sup );
		glTexCoord2f(1, 1); glVertex3f( -length_inf,  length_sup,  length_sup );
		glTexCoord2f(1, 0); glVertex3f( -length_inf, -length_sup,  length_sup );

	// Render the top quad
		glTexCoord2f(1, 0); glVertex3f( -length_sup,  length_inf, -length_sup );
		glTexCoord2f(0, 0); glVertex3f(  length_sup,  length_inf, -length_sup );
		glTexCoord2f(0, 1); glVertex3f(  length_sup,  length_inf,  length_sup );
		glTexCoord2f(1, 1); glVertex3f( -length_sup,  length_inf,  length_sup );

	// Render the bottom quad
		glTexCoord2f(0, 0); glVertex3f( -length_sup, -length_inf, -length_sup );
		glTexCoord2f(0, 1); glVertex3f( -length_sup, -length_inf,  length_sup );
		glTexCoord2f(1, 1); glVertex3f(  length_sup, -length_inf,  length_sup );
		glTexCoord2f(1, 0); glVertex3f(  length_sup, -length_inf, -length_sup );
	glEnd();
}
