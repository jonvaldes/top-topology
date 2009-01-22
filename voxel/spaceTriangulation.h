#ifndef __SPACE_TRIANGULATION_H__
#define __SPACE_TRIANGULATION_H__

#include "voxelSpace.h"
#include "EdgeSet.h"
#include "Face.h"
#include "Point.h"

#include <set>
#include <map>

using namespace util;

namespace voxel
{
	typedef std::set<Face> FaceSet;
	typedef std::set<PointID> PointSet;

	class SpaceTriangulation
	{
		public:

			SpaceTriangulation(const VoxelSpace & space);
			~SpaceTriangulation();

			const FaceSet& faces(){return m_faces;}
			const EdgeSet& edges(){ return m_edges;}
			const PointSet& points(){ return m_points;}

			void triangulate();
			int countComponents() const;
			int calculateNumHoles() const;

			PointID getIDFromSurfacePoint(const Point& p);
			Point getSurfacePointFromID(PointID p);

		private:
			/* Counts components in a triangulation, but destroys the triangulation in the process */
			int countComponentsDestructively();
			const VoxelSpace & m_space;	
			FaceSet m_faces;
			EdgeSet m_edges;
			PointSet m_points;
	};
}
#endif
