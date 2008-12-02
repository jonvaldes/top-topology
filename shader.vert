#version 120
#extension GL_EXT_geometry_shader4 : enable

varying vec4 diffuse,ambient;
varying vec3 lightDir;

void main()
{	
	lightDir = normalize(vec3(gl_LightSource[0].position));

	diffuse = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
	ambient = gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
	ambient += gl_LightModel.ambient * gl_FrontMaterial.ambient;

	gl_Position = ftransform();
	gl_FrontColor = gl_Vertex;
} 

