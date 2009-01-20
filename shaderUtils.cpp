#include "shaderUtils.h"
#include "PortableGL.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstdarg>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace glutil;

GLuint loadVertShader(const std::string& shaderText)
{
	GLuint shaderObject;
	shaderObject = glCreateShader(GL_VERTEX_SHADER);
	const char * text = shaderText.c_str();
	glShaderSource(shaderObject,1,&text,NULL);
	glCompileShader(shaderObject);
	GLint shaderCompiled;
	glGetShaderiv(shaderObject,GL_COMPILE_STATUS,&shaderCompiled);
	if(!shaderCompiled)
	{
		printf("El vertex shader no se ha podido compilar\n");
		int logSize;
		glGetShaderiv(shaderObject,GL_INFO_LOG_LENGTH,&logSize);
		char * log = (char*)malloc(sizeof(char)*logSize+1);
		glGetShaderInfoLog(shaderObject, logSize, &logSize, log);
		printf("Log de compilado: %s\n",log);
		free(log);
		glDeleteShader(shaderObject);
		return 0;
	}
	return shaderObject;
}

GLuint loadGeomShader(const std::string& shaderText)
{
	GLuint shaderObject;
	shaderObject = glCreateShader(GL_GEOMETRY_SHADER_EXT);
	const char * text = shaderText.c_str();
	glShaderSource(shaderObject,1,&text,NULL);
	glCompileShader(shaderObject);
	GLint shaderCompiled;
	glGetShaderiv(shaderObject,GL_COMPILE_STATUS,&shaderCompiled);
	if(!shaderCompiled)
	{
		printf("El fragment shader no se ha podido compilar\n");

		int logSize;
		glGetShaderiv(shaderObject,GL_INFO_LOG_LENGTH,&logSize);
		char * log = (char*)malloc(sizeof(char)*logSize+1);
		glGetShaderInfoLog(shaderObject, logSize+1, &logSize, log);
		printf("Log de compilado: %s\n",log);
		free(log);
		glDeleteShader(shaderObject);
		printf("\n\n -- Texto del shader:\n");
		printf("%s\n -- FIN --\n",shaderText.c_str());
		return 0;
	}
	return shaderObject;
}
GLuint loadFragShader(const std::string& shaderText)
{
	GLuint shaderObject;
	shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	const char * text = shaderText.c_str();
	glShaderSource(shaderObject,1,&text,NULL);
	glCompileShader(shaderObject);
	GLint shaderCompiled;
	glGetShaderiv(shaderObject,GL_COMPILE_STATUS,&shaderCompiled);
	if(!shaderCompiled)
	{
		printf("El geometry shader no se ha podido compilar\n");

		int logSize;
		glGetShaderiv(shaderObject,GL_INFO_LOG_LENGTH,&logSize);
		char * log = (char*)malloc(sizeof(char)*logSize+1);
		glGetShaderInfoLog(shaderObject, logSize+1, &logSize, log);
		printf("Log de compilado: %s\n",log);
		free(log);
		glDeleteShader(shaderObject);
		printf("\n\n -- Texto del shader:\n");
		printf("%s\n -- FIN --\n",shaderText.c_str());
		return 0;
	}
	return shaderObject;
}


GLuint createShaderProgram(GLuint shader1, GLuint shader2) 
{
	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, shader1);
	glAttachShader(shaderProgram, shader2);
	return shaderProgram;
}
namespace glutil
{
	GLuint linkProgram(GLuint shaderProgram)
	{
		glLinkProgram(shaderProgram);
		GLint linked;

		glGetProgramiv(shaderProgram,GL_LINK_STATUS,&linked);
		if(linked == GL_FALSE)
		{
			printf("El programa no se ha podido linkar\n");
			int logSize;
			glGetProgramiv(shaderProgram,GL_INFO_LOG_LENGTH,&logSize);
			char * log = (char*)malloc(sizeof(char)*logSize+1);
			glGetProgramInfoLog(shaderProgram, logSize, &logSize, log);
			printf("Log de linkado: %s\n",log);
			free(log);
			glDeleteProgram(shaderProgram);
			return 0;
		}
		return shaderProgram;
	}
}

bool read_file(const char * filename, std::string &s)
{
	std::ifstream in(filename);
	if (!in.is_open())
	{
		printf("El archivo no se pudo abrir\n");
		return false;
	}
	std::stringstream ss;
	ss << in.rdbuf();

	s = ss.str();
	return true;
}

namespace glutil{
GLuint loadShaderFromFile(int shaderType, const char * filename)
{
	std::string shaderText;
	if(!read_file(filename, shaderText))
	{
		printf("No se pudo leer el shader: %s\n", filename);
		return 0;
	}
	if(shaderType == GL_VERTEX_SHADER)
		return loadVertShader(shaderText);
	else if(shaderType == GL_GEOMETRY_SHADER_EXT)
		return loadGeomShader(shaderText);
	else if(shaderType == GL_FRAGMENT_SHADER)
		return loadFragShader(shaderText);
	else
		throw std::string("Type of shader not one of GL_FRAGMENT_SHADER, GL_VERTEX_SHADER or GL_GEOMETRY_SHADER");
}
}

ShaderProgram::ShaderProgram()
{
	m_shaderID = glCreateProgram();
}

ShaderProgram::~ShaderProgram()
{
	for(unsigned int i=0;i<m_shaderObjectIDs.size();++i)
		glDeleteShader(m_shaderObjectIDs[i]);
	glDeleteProgram(m_shaderID);
}

/** Shader types: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER */
void ShaderProgram::attachShaderFromFile(int shaderType, std::string filename)
{
	GLuint result = loadShaderFromFile(shaderType, filename.c_str());
	if(result!=0)
		m_shaderObjectIDs.push_back(result);
	else
	{
		class ShaderFileNotFoundException : public std::exception{
			public:
			ShaderFileNotFoundException(std::string filename):m_filename(filename){}
			virtual ~ShaderFileNotFoundException()throw(){}
			private:
			virtual const char* what() const throw()
			{	
				return (std::string("Shader file could not be loaded. File:\"") +m_filename+std::string("\"")).c_str();
			}
			std::string m_filename;
		};

		throw ShaderFileNotFoundException(filename); 
	}

}

/** Throws a string when program could not be compiled */
void ShaderProgram::link()
{
	for(unsigned int i=0;i<m_shaderObjectIDs.size();++i)
		glAttachShader(m_shaderID, m_shaderObjectIDs[i]);

	if(linkProgram(m_shaderID) == 0)
	{
		class ShaderNotLinkedException : public std::exception{
			public:
				ShaderNotLinkedException(){}
				virtual ~ShaderNotLinkedException()throw(){}
			private:
				virtual const char* what() const throw()
				{	
					return (std::string("Shader could not be linked.").c_str());
				}
		};
		throw ShaderNotLinkedException();
	}
}

void ShaderProgram::bind()
{
	glUseProgram(m_shaderID);

}

void ShaderProgram::unbind()
{
	glUseProgram(0);
}


void ShaderProgram::setUniform1i(char * name, int value)
{
	// TODO hacer que esto se almacene en el propio shader, para no preguntar a opengl todo el rato
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM,&program);
	glUseProgram(m_shaderID);
	GLint loc = glGetUniformLocation(m_shaderID,name);
	glUniform1i(loc,value);
	glUseProgram(program);
}

void ShaderProgram::setUniform1f(char * name, float value)
{
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM,&program);
	glUseProgram(m_shaderID);
	GLint loc = glGetUniformLocation(m_shaderID,name);
	glUniform1f(loc,value);
	glUseProgram(program);
}

void ShaderProgram::setUniform2i(char * name, int value1,int value2)
{
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM,&program);
	glUseProgram(m_shaderID);
	GLint loc = glGetUniformLocation(m_shaderID,name);
	glUniform2i(loc,value1,value2);
	glUseProgram(program);
}

void ShaderProgram::setUniform2f(char * name, float value1,float value2)
{
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM,&program);
	glUseProgram(m_shaderID);
	GLint loc = glGetUniformLocation(m_shaderID,name);
	glUniform2f(loc,value1,value2);
	glUseProgram(program);
}

void ShaderProgram::setUniform3f(char * name, float value1, float value2, float value3)
{
	GLint program;
	glGetIntegerv(GL_CURRENT_PROGRAM,&program);
	glUseProgram(m_shaderID);
	GLint loc = glGetUniformLocation(m_shaderID,name);
	glUniform3f(loc,value1,value2,value3);
	glUseProgram(program);
}



