#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include <vector>

typedef unsigned int GLuint;
namespace glutil
{

	GLuint createShaderProgram(GLuint shader1, GLuint shader2);
	GLuint linkProgram(GLuint shaderProgram);
	GLuint loadShaderFromFile(int shaderType, const char * filename);

	class ShaderProgram
	{
		public:
			ShaderProgram();
			~ShaderProgram();
			void setUniform1i(char * name, int value);
			void setUniform1f(char * name, float value);
			void setUniform2i(char * name, int value1,int value2);
			void setUniform2f(char * name, float value1,float value2);
			void setUniform3f(char * name, float value1, float value2, float value3);

			/** Shader types: GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER */
			void attachShaderFromFile(int shaderType, std::string filename);

			/** Throws a string when program could not be compiled */
			void link();

			void bind();
			void unbind();
		private:
			std::vector<GLuint> m_shaderObjectIDs;
			GLuint m_shaderID;
	};
}
#endif
