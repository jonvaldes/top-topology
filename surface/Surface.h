#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "EdgeSet.h"
#include "Face.h"

namespace surface 
{
	typedef std::vector<Face> FacesList;
	typedef std::vector<Point> PointsList;
	
	class SpaceTriangulation;

	class Surface
	{
		public:
			Surface(const SpaceTriangulation & triangulation);


		private:
			PointsList m_points;
			EdgeSet	m_edges;
			FacesList m_faces;

	};
}

#endif
