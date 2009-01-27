#ifndef VOXEL_SPACE 
#define VOXEL_SPACE

#include "Edge.h"
#include "Point.h"
#include "Voxel.h"

#include <cstring>
#include "../util/NTuple.h"
#include <set>
#include <vector>
#include <stdint.h>
using namespace util;

namespace voxel
{
	class VoxelSpace
	{
		public:
			VoxelSpace(int widthVoxels, int heightVoxels, int depthVoxels, bool defaultValue);
			VoxelSpace(const VoxelSpace & other);
			~VoxelSpace();
			const VoxelSpace & operator=(const VoxelSpace &other);


			void addBall(Point center, float diameter) 		{setBall(center,diameter,true);}
			void removeBall(Point center, float diameter) 	{setBall(center,diameter,false);}
			
			void addCilinder(Point p1, Point p2, float radius) {setCilinder(p1,p2,radius,true);}
			void removeCilinder(Point p1, Point p2, float radius){setCilinder(p1,p2,radius,false);}


			size_t getSize(int dimension) const {return m_spaceSize[dimension];} 
			bool isBorder(int x, int y, int z) const;
			bool getVoxel(int x, int y, int z) const;
			bool getVoxel(Voxel p) const;
		
			std::vector<VoxelSpace> getComponents() const;
			int countComponents() const;
			int countVolumes() const;


		private:
			std::vector<VoxelSpace> getComponentsDestructively();
			VoxelSpace getComponentDestructively(Voxel v);

			int countComponentsDestructively();
			int countVolumesDestructively();

			void setBall(Point center, float diameter,bool value);
			void setCilinder(Point p1, Point p2, float radius, bool value);

			void emptyComponent(Voxel v);
			void emptyVolume(Voxel v);


			int voxelToIndex(Voxel p) const;
			Voxel indexToVoxel(int index) const;
			void setVoxel(Voxel p, bool value);

			size_t m_spaceSize[3];
			bool * m_space;	
	};
}
#endif
