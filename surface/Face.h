#ifndef __VOXEL_FACE__
#define __VOXEL_FACE__

#include "PointID.h"

#include "../util/NTuple.h"

using namespace util;

namespace voxel
{
	class Face : public NTuple <PointID,4>
	{
		public:
			Face(PointID p1, PointID p2, PointID p3, PointID p4)
			{
				v[0] = p1, v[1] = p2, v[2] = p3, v[3] = p4;
			}
			Face(const Face &f) : NTuple<PointID,4>()
			{
				v[0] = f.v[0], v[1] = f.v[1], v[2] = f.v[2], v[3] = f.v[3];
			}
			bool operator<(const Face &f) const
			{
				if(v[0] < f.v[0])
					return true;
				if(v[0] > f.v[0])
					return false;
				if(v[1] < f.v[1])
					return true;
				if(v[1] > f.v[1])
					return false;
				if(v[2] < f.v[2])
					return true;
				if(v[2] > f.v[2])
					return false;
				if(v[3] < f.v[3])
					return true;
				return false;
			}
	};
}
#endif
