#ifndef __VOXEL_VOXEL_H__
#define __VOXEL_VOXEL_H__

#include "../util/NTuple.h"
//#include <stdint.h>
using namespace util;

namespace voxel
{
	class Voxel: public Triplet<int>
	{
		public:
			Voxel(){}
			Voxel(int x, int y, int z){v[0] =x, v[1]= y, v[2]=z;}
			const int & x() const {return v[0];}
			const int & y() const {return v[1];}
			const int & z() const {return v[2];}

			bool operator==(const Point &p) const
			{
				return (x() == p.x() && y() == p.y() &&	z() == p.z());
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
