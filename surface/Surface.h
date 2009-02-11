#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "EdgeSet.h"
#include "Face.h"
#include "../geom/Point3D.h"

namespace surface 
{
	typedef std::vector<Face> FacesList;
	typedef std::vector<geom::Point3D> PointsList;
	
	class Surface
	{
		public:
			Surface();
			void addPoint(const geom::Point3D &p);
			void addFace(const Face &f);

			void render();

		private:
			PointsList m_points;
			EdgeSet	m_edges;
			FacesList m_faces;
	};
}

#endif
