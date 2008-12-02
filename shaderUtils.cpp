#include "shaderUtils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstdarg>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

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


GLuint createShaderProgram(GLuint shader1, GLuint shader2, GLuint shader3) 
{
	GLuint id = 0;
	va_list argptr;

	GLuint shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, shader1);
	glAttachShader(shaderProgram, shader2);
	glAttachShader(shaderProgram, shader3);
	return shaderProgram;
}

GLuint linkProgram(GLuint shaderProgram)
{
	glLinkProgram(shaderProgram);
	GLint linked;

	glGetProgramiv(shaderProgram,GL_LINK_STATUS,&linked);
	if(linked == GL_FALSE)
	{
		printf("El programa no se ha podido compilar\n");
		int logSize;
		glGetProgramiv(shaderProgram,GL_INFO_LOG_LENGTH,&logSize);
		char * log = (char*)malloc(sizeof(char)*logSize+1);
		glGetProgramInfoLog(shaderProgram, logSize, &logSize, log);
		printf("Log de compilado: %s\n",log);
		free(log);
		glDeleteProgram(shaderProgram);
		return 0;
	}
	return shaderProgram;
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
	else
		return loadFragShader(shaderText);
}

