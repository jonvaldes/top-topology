#ifndef __VOXEL_FACE__
#define __VOXEL_FACE__

#include "PointID.h"
#include "../util/NTuple.h"

namespace surface
{
	class Face : public util::NTuple<PointID,4>
	{
		public:
			Face()
			{
				for(int i=0;i<4;++i)
					v[i] = 0;
			}
			Face(PointID p1, PointID p2, PointID p3, PointID p4)
			{
				v[0] = p1, v[1] = p2, v[2] = p3, v[3] = p4;
			}

			Face(const Face &f) 
			{
				for(int i=0;i<4;++i)
					v[i] = f.v[i];
			}
	};
}
#endif
