#include "voxelSpace.h"
#include <cassert>

int main()
{
	VoxelSpace vs(11,11,11,0);
	vs.addBall(Point(5,5,5),4);
	vs.triangulate();
	return 0;
}
