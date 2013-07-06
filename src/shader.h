#ifndef SHADER_2N30PDY9
#define SHADER_2N30PDY9

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#include <list>

using namespace std;

// allow file browsing
#include <dirent.h>
#ifdef  __linux__		
	#include <sys/types.h>  // no need on windows
#endif

#include <unistd.h>

class Shader
{
	private:
		list<GLuint> shader_list;
		list<GLuint> program_list;

		char* loadSource(const char* filename);
		GLuint loadShader(GLenum type, const char *filename);
	public:	
		GLuint loadProgram(
				const char* vertexshader_filename,
				const char* pixelshader_filename
				);
		
		Shader();
		~Shader();
};


#endif /* end of include guard: SHADER_2N30PDY9 */
