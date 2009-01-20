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

			void addBall(Point center, float diameter) 		{setBall(center,diameter,true);}
			void removeBall(Point center, float diameter) 	{setBall(center,diameter,false);}
			
			void addCilinder(Point p1, Point p2, float radius) {setCilinder(p1,p2,radius,true);}
			void removeCilinder(Point p1, Point p2, float radius){setCilinder(p1,p2,radius,false);}

			std::set<Face> & triangulated(){return m_faces;} // remove
			std::set<Point> & triangulationPoints(){ return m_points;} // remove
			std::vector<std::pair<Point,Point> > & nonManifoldEdges(){ return m_nonmaniEdges;} //remove
			void triangulate();
				
			std::vector<std::pair<Point,Point> > m_edges; // remove

		private:
			void setBall(Point center, float diameter,bool value);
			void setCilinder(Point p1, Point p2, float radius, bool value);

			uint64_t getIntFromSurfacePoint(const Point& p);
			Point getSurfacePointFromInt(uint64_t i);
			bool isBorder(int x, int y, int z);

			void removeNonConnected();

			int pointToIndex(Point p);
			Point indexToPoint(int index);
			void setVoxel(Point p, bool value);
			bool getVoxel(Point p);
			bool getVoxel(int x, int y, int z);

			size_t m_spaceSize[3];
			bool * m_space;	
			std::set<Face> m_faces; // remove
			std::set<Point> m_points; // remove
			std::vector<std::pair<Point,Point> > m_nonmaniEdges; // remove

	};
}
#endif
