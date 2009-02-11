#ifndef __VOXEL_EDGE__
#define __VOXEL_EDGE__

//#include "Point.h"
#include <stdint.h>
namespace voxel
{
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
}
#endif
