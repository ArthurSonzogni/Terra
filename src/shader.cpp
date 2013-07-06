#include "shader.h"

char* Shader::loadSource(const char *filename)
{
    char *src = NULL;   /* our shader source code */
    FILE *fp = NULL;    /* file */
    long size;          /* file length */
    long i;             /* counter */
     
     
    /* open the file */
    fp = fopen(filename, "r");
    /* opening check */
    if(fp == NULL)
    {
        fprintf(stderr, "unable to open the fil '%s'\n", filename);
        return NULL;
    }
     
    /* we get file length */
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
     
    /* go to the begining of the file */
    rewind(fp);
     
    /* memory allocation */
    src = (char*)malloc(size+1); /* +1 for the last character:'\0' */
    if(src == NULL)
    {
        fclose(fp);
        fprintf(stderr, "error, unable to allocate memory!\n");
        return NULL;
    }
     
    /* file reading */
    for(i=0; i<size; i++)
        src[i] = fgetc(fp);
     
    /* ending character '\0' */
    src[size] = '\0';
     
    fclose(fp);
     
    return src;
}

GLuint Shader::loadShader(GLenum type, const char *filename)
{
    GLuint shader = 0;
    GLsizei logsize = 0;
    GLint compile_status = GL_TRUE;
    char *log = NULL;
    char *src = NULL;
     
    /* shader creation */
    shader = glCreateShader(type);
    if(shader == 0)
    {
        fprintf(stderr, "enable to create a shader\n");
        return 0;
    }
     
    /* source code loading */
    src = loadSource(filename);
    if(src == NULL)
    {
        glDeleteShader(shader);
        return 0;
    }
     
    /* source code assigning */
    glShaderSource(shader, 1, (const GLchar**)&src, NULL);
     
    /* shader compilation */
    glCompileShader(shader);
     
    /* freeing memory */
    free(src);
    src = NULL;
     
    /* compilation success check */
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
    if(compile_status != GL_TRUE)
    {
        /* error text retreiving*/
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
         
        /* memory allocating */
        log = (char*)malloc(logsize + 1);
        if(log == NULL)
        {
            fprintf(stderr, "impossible d'allouer de la memoire !\n");
            return 0;
        }
        /* init content*/
        memset(log, '\0', logsize + 1);
         
        glGetShaderInfoLog(shader, logsize, &logsize, log);
        fprintf(stderr, "unable to compile shader '%s' :\n%s",
                filename, log);
         
        /* freeing memory */
        free(log);
        glDeleteShader(shader);
         
        return 0;
    }
     
	shader_list.push_front(shader);
    return shader;
}


GLuint Shader::loadProgram(
		const char* vertexshader_filename,
		const char* pixelshader_filename
		)
{
	GLuint program=0;
	program = glCreateProgram();
	
	if (program)
	{
		if (vertexshader_filename)
		{
			GLuint vt=loadShader(GL_VERTEX_SHADER,vertexshader_filename);
			if (vt)
			{
				glAttachShader(program,vt);
			}
			else
			{
				glDeleteProgram(program);
				return 0;
			}
		}

		if (pixelshader_filename)
		{
			GLuint px=loadShader(GL_FRAGMENT_SHADER,pixelshader_filename);
			if (px)
			{
				glAttachShader(program,px);
			}
			else
			{
				glDeleteProgram(program);
				return 0;
			}
		}

		glLinkProgram(program);
		GLint result;
		glGetProgramiv(program,GL_LINK_STATUS, &result);
		if (result!=GL_TRUE)
		{
			fprintf(stderr,"error: unable to link the program");
			return 0;
		}
		
		program_list.push_front(program);
		return program;
	}
	else
	{
		fprintf(stderr,"error, enable to create the program\n");
		return 0;
	}
}

Shader::~Shader()
{
	list<GLuint>::iterator it;
	for(it=shader_list.begin();it!=shader_list.end();++it)
	{
		if (glIsShader(*it))
			glDeleteShader(*it);
	}

	for(it=program_list.begin();it!=program_list.end();++it)
	{
		if (glIsProgram(*it))
			glDeleteProgram(*it);
	}
}

Shader::Shader()
{
	glewInit();
	if (
	GL_ARB_shading_language_100
	) printf("oui\n"); else printf("non\n");
	if (
	GL_ARB_shader_objects 
	) printf("oui\n"); else printf("non\n");
	if (
	GL_ARB_vertex_shader 
	) printf("oui\n"); else printf("non\n");
	if (
	GL_ARB_fragment_shader
	) printf("oui\n"); else printf("non\n");
}
