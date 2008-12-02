#ifndef SHADERUTILS_H
#define SHADERUTILS_H
#include "GLee.h"
#ifdef __APPLE__
#include <OpenGL/GL.h>
#else
#include <GL/gl.h>
#endif

//GLuint createShaderProgram(GLuint shader, ... );
GLuint createShaderProgram(GLuint shader1, GLuint shader2, GLuint shader3);
GLuint linkProgram(GLuint shaderProgram);
GLuint loadShaderFromFile(int shaderType, const char * filename);

#endif
