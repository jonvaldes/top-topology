#ifndef __VOXEL_FACE__
#define __VOXEL_FACE__

#include "Point.h"

#include "../util/NTuple.h"

using namespace util;

namespace Voxel
{
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
}
#endif
