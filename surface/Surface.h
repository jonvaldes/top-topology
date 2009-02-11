#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "EdgeSet.h"
#include "Face.h"
#include "../geom/Point3D.h"
#include "../geom/Vector3D.h"

namespace surface 
{
	typedef std::vector<Face> FacesList;
	typedef std::vector<geom::Point3D> PointsList;
	typedef std::vector<geom::Vector3D> VectorsList;
	class Surface
	{
		public:
			Surface();
			void addPoint(const geom::Point3D &p);
			void addFace(const Face &f);
			int getNumPoints() const {return m_points.size();}
			int getNumFaces() const {return m_faces.size();}
			int getNumEdges() const {return m_edges.totalEdgesCount();}

			void render(float lastFaceMergingPercentage, bool showWireframe, bool showFaces);

			void mergeLastFace();

		private:
			PointsList m_points;
			EdgeSet	m_edges;
			FacesList m_faces;
			VectorsList m_faceNormals;

	};
}

#endif
