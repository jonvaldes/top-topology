#ifndef VOXEL_SPACE 
#define VOXEL_SPACE

#include <cstring>
#include "util/NTuple.h"
#include <set>
#include <vector>
#include <stdint.h>
using namespace util;

namespace Voxel
{
	class Point : public Triplet<float>
	{
		public:
			Point(){}
			Point(float x, float y, float z){v[0] =x, v[1]= y, v[2]=z;}
			const float & x() const {return v[0];}
			const float & y() const {return v[1];}
			const float & z() const {return v[2];}

			float & x(){return v[0];}
			float & y(){return v[1];}
			float & z(){return v[2];}

			float distanceSquaredTo(const Point &p) const;

			bool operator==(const Point &p) const
			{
				return this->distanceSquaredTo(p) < 0.00001;
			}

			bool operator<(const Point &p) const
			{
				return coordsSum() < p.coordsSum();
			}
			int coordsSum() const
			{
				return v[0] + v[1]*100 + v[2]*10000;
			}
	};

	class Edge
	{
		public:
			enum {DIR_X=0, DIR_Y=1, DIR_Z=2};

			Edge(const uint64_t pointID, int direction)
			{
				uint64_t pr = pointID<<2;
				e_id = pr+direction;
			}
			Edge(const Edge &e)
			{
				e_id = e.e_id;
			}
			bool operator==(const Edge &e) const
			{
				return e_id == e.e_id;
			}
			bool operator<(const Edge &e) const
			{
				return e_id < e.e_id;
			}
			const uint64_t& getID() const {return e_id;}
		private:
			uint64_t e_id;
	};

	class Face : public NTuple <Point,4>
	{
		public:
			Face(Point p1, Point p2, Point p3, Point p4)
			{
				v[0] = p1, v[1] = p2, v[2] = p3, v[3] = p4;
			}
			Face(const Face &f) : NTuple<Point,4>()
			{
				v[0] = f.v[0], v[1] = f.v[1], v[2] = f.v[2], v[3] = f.v[3];
			}
			bool operator<(const Face &f) const
			{
				if(v[0].coordsSum() < f.v[0].coordsSum())
					return true;
				if(v[0].coordsSum() > f.v[0].coordsSum())
					return false;
				if(v[1].coordsSum() < f.v[1].coordsSum())
					return true;
				if(v[1].coordsSum() > f.v[1].coordsSum())
					return false;
				if(v[2].coordsSum() < f.v[2].coordsSum())
					return true;
				if(v[2].coordsSum() > f.v[2].coordsSum())
					return false;
				if(v[3].coordsSum() < f.v[3].coordsSum())
					return true;
				return false;
			}
	};

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
