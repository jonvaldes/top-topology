#ifndef __VOXEL_POINT__
#define __VOXEL_POINT__

#include "../util/NTuple.h"
//#include <stdint.h>
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
}
#endif
