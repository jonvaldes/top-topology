#version 120
#extension GL_EXT_geometry_shader4 : enable

varying vec4 gdiffuse,gambient;
varying vec3 gnormal, glightDir;


vec3 unifyNormal(vec3 n)
{
	if(abs(n[0]) > abs(n[1]) && abs(n[0]) > abs(n[2]))
		return normalize(vec3(n[0],0.0,0.0));

	if(abs(n[1]) > abs(n[0]) && abs(n[1]) > abs(n[2]))
		return normalize(vec3(0.0,n[1],0.0));

	if(abs(n[2]) > abs(n[0]) && abs(n[2]) > abs(n[0]))
		return normalize(vec3(0.0,0.0,n[2]));

	return normalize(n);
}  

void main()
{
	vec3 n,halfV;
	float NdotL,NdotHV;

	/* The ambient term will always be present */
	vec4 color = gl_Color;
	n = unifyNormal(gnormal);
	NdotL = max(dot(n,glightDir),0.0);
	color += gdiffuse * NdotL;
	gl_FragColor = color;
}


