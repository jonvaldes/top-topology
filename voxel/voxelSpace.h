#ifndef VOXEL_SPACE 
#define VOXEL_SPACE

#include "Edge.h"
#include "Point.h"
#include "Face.h"


#include <cstring>
#include "../util/NTuple.h"
#include <set>
#include <vector>
#include <stdint.h>
using namespace util;

namespace Voxel
{
	class VoxelSpace
	{
		public:
			VoxelSpace(int widthVoxels, int heightVoxels, int depthVoxels, bool defaultValue);
			~VoxelSpace();

			void addBall(Point center, float diameter);
			void removeBall(Point center, float diameter);
			void removeCilinder(Point p1, Point p2, float radius);

			std::set<Face> & triangulated(){return m_faces;}
			std::set<Point> & triangulationPoints(){ return m_points;}
			std::vector<std::pair<Point,Point> > & nonManifoldEdges(){ return m_nonmaniEdges;}
			void triangulate();
				
			std::vector<std::pair<Point,Point> > m_edges;

		private:
			uint64_t getIntFromSurfacePoint(const Point& p);
			Point getSurfacePointFromInt(uint64_t i);
			bool isBorder(int x, int y, int z);

			void removeNonConnected();

			int pointToIndex(Point p);
			Point indexToPoint(int index);
			void setPoint(Point p, bool value);
			bool getPoint(Point p);
			bool getPoint(int x, int y, int z);

			size_t m_spaceSize[3];
			bool * m_space;	
			std::set<Face> m_faces;
			std::set<Point> m_points;
			std::vector<std::pair<Point,Point> > m_nonmaniEdges;

	};
}
#endif
