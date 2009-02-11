#ifndef __FREECAMERA_H__
#define __FREECAMERA_H__

#include "../geom/Point3D.h"
#include "../geom/Vector3D.h"
namespace glutil
{
	class FreeCamera
	{
		public:
			FreeCamera(geom::Point3D pos, float yaw, float pitch, float FOV) : m_position(pos), m_yaw(yaw), m_pitch(pitch), m_FOV(FOV) {}
			void move(geom::Vector3D distance);
			void advance(float amount);
			void strafeLeft(float amount);
			void strafeRight(float amount);
			void moveUp(float amount);

			void mouseLookTurn(float dx, float dy);
			void lookAt(geom::Point3D pos);

			const geom::Point3D & position() const {return m_position;}
			float yaw() const {return m_yaw;}
			float pitch() const {return m_pitch;}
			float FOV() const {return m_FOV;}
			void setFOV(float FOV){m_FOV = FOV;}

			void setAsGLCamera(int windowWidth, int windowHeight);

		private:
			geom::Vector3D pitchYawToVector(float yaw, float pitch) const;
			void setPitchYawFromVector(const geom::Vector3D &v);

			geom::Point3D m_position;
			// Estos dos estan en radianes!
			float m_yaw; // giro alrededor del eje vertical
			float m_pitch; // giro alrededor del eje horizontal, perpendicular a la vista
			float m_FOV;
	};
}

#endif
