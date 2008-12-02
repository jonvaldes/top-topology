#ifndef VOXEL_SPACE 
#define VOXEL_SPACE

#include <cstring>
#include "util/NTuple.h"
#include <set>
using namespace util;

namespace Voxel
{
	class Point : public Triplet<float>
	{
		public:
			Point(){}
			Point(float x, float y, float z){v[0] =x, v[1]= y, v[2]=z;}
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

	class Edge : public Pair<Point>
	{
		public:
			Edge(const Point &p1, const Point &p2)
			{
				if(p1<p2)
				{
					v[0] = p1;
					v[1] = p2;
				}
				else
				{
					v[0] = p2;
					v[1] = p1;
				}
			}
			Edge(const Edge &e)
			{
				v[0] = e.v[0];
				v[1] = e.v[1];
			}
			bool operator==(const Edge &e) const
			{
				return (v[0] == e.v[0] && v[1] == e.v[1]);
			}
			bool operator<(const Edge &e) const
			{
				if(*this==e)
					return false;
				if(v[0].coordsSum() < e.v[0].coordsSum())
					return true;
				if(v[0].coordsSum() > e.v[0].coordsSum())
					return false;
				if(v[1].coordsSum() < e.v[1].coordsSum())
					return true;
				return false;
			}
	};

	class Face : public Triplet <Point>
	{
		public:
			Face(Point p1, Point p2, Point p3)
			{
				v[0] = p1, v[1] = p2, v[2] = p3;
			}
			Face(const Face &f)
			{
				v[0] = f.v[0], v[1] = f.v[1], v[2] = f.v[2];
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
			std::set<Face> & triangulated(){return m_faces;}
			std::set<Point> & triangulationPoints(){ return m_points;}
			void triangulate();
				
		private:
			int pointToIndex(Point p);
			Point indexToPoint(int index);
			void setPoint(Point p, bool value);
			bool getPoint(Point p);
			bool getPoint(int x, int y, int z);

			size_t m_spaceSize[3];
			bool * m_space;	
			std::set<Face> m_faces;
			std::set<Point> m_points;

	};
}
#endif
