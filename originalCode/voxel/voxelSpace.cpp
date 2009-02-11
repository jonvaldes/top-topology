#include "voxelSpace.h"
#include <set>
#include <map>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>


using namespace voxel;

VoxelSpace::VoxelSpace(int widthVoxels, int heightVoxels, int depthVoxels, bool defaultValue)
{
	m_spaceSize[0] = widthVoxels;
	m_spaceSize[1] = heightVoxels;
	m_spaceSize[2] = depthVoxels;

	for(int i=0;i<3;++i)
		if(m_spaceSize[i] > 100)
		{
			printf("ERROR: Space size > 100, voxels won't work with that value unless their code is changed\n");
			exit(-1);
		}

	m_space = new bool[widthVoxels * heightVoxels * depthVoxels];
	for(int i=0;i<widthVoxels*heightVoxels*depthVoxels; ++i)
		m_space[i] = defaultValue;
}

VoxelSpace::~VoxelSpace()
{
	delete m_space;
}

void VoxelSpace::setBall(Point center, float radius,bool value)
{
	int maxRadius = ceil(radius);
	float maxDistSquared = radius*radius;
	
	for(int x = center[0]-maxRadius; x<center[0]+maxRadius; ++x)
		for(int y = center[1]-maxRadius; y<center[1]+maxRadius; ++y)
			for(int z = center[2]-maxRadius; z<center[2]+maxRadius; ++z)
				if(center.distanceSquaredTo(Point(x,y,z)) <= maxDistSquared)
					setVoxel(Voxel(x,y,z), value);
}

/* It doesn't really set cilinders, but capsules */
void VoxelSpace::setCilinder(Point p1, Point p2, float radius, bool value)
{
	for(unsigned int x = 0; x<m_spaceSize[0]; ++x)
		for(unsigned int y = 0; y<m_spaceSize[1]; ++y)
			for(unsigned int z = 0; z<m_spaceSize[2]; ++z)
			{
				// Calculamos el plano perpendicular a p1p2 que pasa por xyz
				double a = p2.x() - p1.x();
				double b = p2.y() - p1.y();
				double c = p2.z() - p1.z();
				double d = -(a*x + b*y +c*z);

				// Calculamos la interseccion con p1p2
				double t = (-d - a*p1.x() - b*p1.y() -c*p1.z()) / (a*(p2.x() - p1.x()) + b*(p2.y() - p1.y()) + c*(p2.z() - p1.z()));

				Point p(0,0,0);
				// Si 0<=t<=1, calculamos la distancia al punto interseccion
				if(t>=0 && t<=1)
					p = Point( p1.x() + t*(p2.x() - p1.x()), p1.y() + t*(p2.y() - p1.y()), p1.z() + t*(p2.z() - p1.z()));
				// Si t>1 calculamos la distancia a p2
				else if(t>1)
					p = p2;
				// Si no, calculamos la distancia a p1
				else
					p = p1;
				// Si la distancia es menor o igual que radius, se vacia
				if(p.distanceSquaredTo(Point(x,y,z))< radius*radius)
					setVoxel(Voxel(x,y,z), value);
			}
}

int VoxelSpace::voxelToIndex(Voxel v) const
{
	return v[0] + v[1]*m_spaceSize[0] + v[2]*m_spaceSize[1]*m_spaceSize[0];
}

Voxel VoxelSpace::indexToVoxel(int index) const
{
	Voxel v(0,0,0);
	v[2] = index / (m_spaceSize[0]*m_space[1]);
	int indexMinusZ = index % (m_spaceSize[0] * m_spaceSize[1]);
	v[1] = indexMinusZ % m_spaceSize[0];
	v[0] = indexMinusZ / m_spaceSize[0];
	return v;
}

void VoxelSpace::setVoxel(Voxel v, bool value)
{
	for(int i=0;i<3;++i)
		if(v[i]<0 || v[i]>=(int)m_spaceSize[i])
			return;

	m_space[voxelToIndex(v)] = value;
}

bool VoxelSpace::getVoxel(Voxel v) const
{
	for(int i=0;i<3;++i)
		if(v[i]<0 || v[i]>=(int)m_spaceSize[i])
			return false; // all voxels outside the volume are empty

	return m_space[voxelToIndex(v)];
}

bool VoxelSpace::getVoxel(int x, int y, int z) const 
{
	return getVoxel(Voxel(x,y,z));
}

bool VoxelSpace::isBorder(int x,int y,int z) const
{
	if(!getVoxel(x,y,z))
		return false;

	int offsets[6][3] = {{-1,0,0}, {1,0,0}, {0,-1,0},{0,1,0},{0,0,-1},{0,0,1}};

	for(int i=0;i<6;++i)
		if(!getVoxel(x+offsets[i][0], y+offsets[i][1], z+offsets[i][2]))
			return true;

	return false;
}

/* empties the volume of voxels of wich 'v' forms part */
void VoxelSpace::emptyVolume(Voxel v)
{
	std::set<Voxel> toRemoveNow;
	std::set<Voxel> toRemoveNext;
	toRemoveNow.insert(v);

	while(toRemoveNow.size() > 0)
	{
		std::set<Voxel>::iterator it = toRemoveNow.begin();
		std::set<Voxel>::iterator end= toRemoveNow.end();
		for(;it!=end;++it)
		{
			const Voxel &vnow = *it;
			setVoxel(vnow,false);
			int offsetTable[] = {-1,0,0,   1,0,0,   0,-1,0,   0,1,0,   0,0,-1,   0,0,1};
			for(int off=0;off<6;++off)
			{
				int * offset = offsetTable + 3*off;
				if(getVoxel(vnow.x()+offset[0],vnow.y()+offset[1],vnow.z()+offset[2]))
					toRemoveNext.insert(Voxel(vnow.x()+offset[0],vnow.y()+offset[1],vnow.z()+offset[2]));
			}
		}
		toRemoveNow = toRemoveNext;
		toRemoveNext.clear();
	}
}

/* empties the component of wich 'v' forms part */
void VoxelSpace::emptyComponent(Voxel v)
{
	std::set<Voxel> toRemoveNow;
	std::set<Voxel> toRemoveNext;
	toRemoveNow.insert(v);

	while(toRemoveNow.size() > 0)
	{
		std::set<Voxel>::iterator it = toRemoveNow.begin();
		std::set<Voxel>::iterator end= toRemoveNow.end();
		for(;it!=end;++it)
		{
			const Voxel &vnow = *it;
			setVoxel(vnow,false);

			for(int x=-1;x<=1;++x)
				for(int y=-1;y<=1;++y)
					for(int z=-1;z<=1;++z)
						if(getVoxel(vnow.x()+x,vnow.y()+y,vnow.z()+z))
							toRemoveNext.insert(Voxel(vnow.x()+x,vnow.y()+y,vnow.z()+z));
		}
		toRemoveNow = toRemoveNext;
		toRemoveNext.clear();
	}
}

std::vector<VoxelSpace> VoxelSpace::getComponents() const
{
	VoxelSpace v = *this;
	return v.getComponentsDestructively();
}

std::vector<VoxelSpace> VoxelSpace::getComponentsDestructively()
{
	std::vector<VoxelSpace> result;
	for(unsigned int x=0;x<m_spaceSize[0];++x)
		for(unsigned int y=0;y<m_spaceSize[1];++y)
			for(unsigned int z=0;z<m_spaceSize[2];++z)
				if(getVoxel(x,y,z)) // found a new component
					result.push_back(getComponentDestructively(Voxel(x,y,z)));

	return result;
}

VoxelSpace VoxelSpace::getComponentDestructively(Voxel v)
{
	std::set<Voxel> toRemoveNow;
	std::set<Voxel> toRemoveNext;
	toRemoveNow.insert(v);

	VoxelSpace result(m_spaceSize[0], m_spaceSize[1], m_spaceSize[2],false);

	while(toRemoveNow.size() > 0)
	{
		std::set<Voxel>::iterator it = toRemoveNow.begin();
		std::set<Voxel>::iterator end= toRemoveNow.end();
		for(;it!=end;++it)
		{
			const Voxel &vnow = *it;
			setVoxel(vnow,false);
			result.setVoxel(vnow,true);

			for(int x=-1;x<=1;++x)
				for(int y=-1;y<=1;++y)
					for(int z=-1;z<=1;++z)
						if(getVoxel(vnow.x()+x,vnow.y()+y,vnow.z()+z))
							toRemoveNext.insert(Voxel(vnow.x()+x,vnow.y()+y,vnow.z()+z));
		}
		toRemoveNow = toRemoveNext;
		toRemoveNext.clear();
	}
	return result;
}



int VoxelSpace::countComponents() const
{
	VoxelSpace v = *this;
	return v.countComponentsDestructively();
}

int VoxelSpace::countComponentsDestructively()
{
	int numComponents = 0;
	for(unsigned int x=0;x<m_spaceSize[0];++x)
		for(unsigned int y=0;y<m_spaceSize[1];++y)
			for(unsigned int z=0;z<m_spaceSize[2];++z)
				if(getVoxel(x,y,z)) // found a new component
				{
					++numComponents;
					emptyComponent(Voxel(x,y,z));
				}

	return numComponents;
}

int VoxelSpace::countVolumes() const
{
	VoxelSpace v = *this;
	return v.countVolumesDestructively();
}

int VoxelSpace::countVolumesDestructively()
{
	int numVolumes= 0;
	for(unsigned int x=0;x<m_spaceSize[0];++x)
		for(unsigned int y=0;y<m_spaceSize[1];++y)
			for(unsigned int z=0;z<m_spaceSize[2];++z)
				if(getVoxel(x,y,z)) // found a new volume
				{
					++numVolumes;
					emptyVolume(Voxel(x,y,z));
				}

	return numVolumes;
}

const VoxelSpace & VoxelSpace::operator=(const VoxelSpace &other)
{
	for(int i=0;i<3;++i)
		m_spaceSize[i] = other.m_spaceSize[i];

	int totalNumVoxels = m_spaceSize[0] * m_spaceSize[1] * m_spaceSize[2];
	m_space = new bool[totalNumVoxels];
	memcpy(m_space, other.m_space, sizeof(bool) * totalNumVoxels);
	return *this;
}

VoxelSpace::VoxelSpace(const VoxelSpace &other)
{
	*this = other;
}

