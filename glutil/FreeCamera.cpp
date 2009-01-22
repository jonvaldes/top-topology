#include "FreeCamera.h"
#include "PortableGL.h"
#include <stdio.h>
using namespace glutil;


// IN THIS CAMERA, Y IS UP!!!
// --------------------------

namespace glutil
{
	static const float FREECAMERA_NEAR_DISTANCE = 0.1;
	static const float FREECAMERA_FAR_DISTANCE = 1000;
}

void FreeCamera::move(geom::Vector3D distance)
{
	m_position = m_position+distance;
}

void FreeCamera::advance(float amount)
{
	m_position.x-=amount*sin(-m_yaw);
	m_position.z-=amount*cos(-m_yaw);
	m_position.y+=amount*sin(-m_pitch);
}

void FreeCamera::strafeRight(float amount)
{
	float yawbackup = m_yaw;
	float pitchbackup = m_pitch;
	m_yaw+=M_PI_2;
	m_pitch = 0;
	advance(amount);
	m_yaw = yawbackup;
	m_pitch = pitchbackup;
}

void FreeCamera::strafeLeft(float amount)
{
	strafeRight(-amount);
}

void FreeCamera::moveUp(float amount)
{
	m_position.y+=amount;
}

void FreeCamera::mouseLookTurn(float dx, float dy)
{
	m_yaw+=dx;
	m_pitch+=dy;
	if(m_pitch > M_PI_2)
		m_pitch = M_PI_2;
	if(m_pitch < -M_PI_2)
		m_pitch = -M_PI_2;
}

void FreeCamera::setAsGLCamera(int windowWidth, int windowHeight)
{
	const float toDegrees = 180.0/M_PI;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective (m_FOV, windowWidth/(float)windowHeight, FREECAMERA_NEAR_DISTANCE, FREECAMERA_FAR_DISTANCE);

	glMatrixMode(GL_MODELVIEW);                          
	glLoadIdentity();                                    
	glRotatef(m_pitch*toDegrees,1.0f,0.0f,0.0f);         
	glRotatef(m_yaw*toDegrees,0.0f,1.0f,0.0f);           
	glTranslatef(-m_position.x,-m_position.y,-m_position.z);  
}

geom::Vector3D FreeCamera::pitchYawToVector(float yaw, float pitch) const
{
	geom::Vector3D result;
	result.dx = sin(yaw) * cos(pitch);
	result.dz = cos(yaw) * cos(pitch);
	result.dy =	sin(-pitch);
	return result;
}

