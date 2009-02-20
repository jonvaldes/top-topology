#ifndef __SURFACE_H__
#define __SURFACE_H__

#include "EdgeSet.h"
#include "Face.h"
#include "../geom/Point3D.h"
#include "../geom/Vector3D.h"
#include <set>
#include <list>

namespace surface 
{
	typedef std::list<Face> FacesList;
	typedef std::vector<geom::Point3D> PointsList;
	typedef std::list<geom::Vector3D> VectorsList;
	class CannotMergeMoreException	{	};

	class Surface
	{
		public:
			Surface();
			void addPoint(const geom::Point3D &p);
			void addFace(const Face &f);
			int getNumPoints() const {return m_points.size() - m_discardedPoints.size();}
			int getNumFaces() const {return m_faces.size();}
			int getNumEdges() const {return m_edges.totalEdgesCount();}

			void render(float lastFaceMergingPercentage, bool showWireframe, bool showFaces);

			void mergeLastFace();
			void applyLoveAndHate( float timeStep);
			void findNextMergeable();
			bool areThereOverlappingFaces(PointID p1, PointID p2) const;


		private:
			PointsList m_points;
			std::set<PointID> m_discardedPoints;
			EdgeSet	m_edges;
			FacesList m_faces;
			VectorsList m_faceNormals;
	};
}

#endif
