#include "voxelSpace.h"
#include <set>
#include <map>
#include <cmath>
#include <stdio.h>

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

int VoxelSpace::voxelToIndex(Voxel v)
{
	return v[0] + v[1]*m_spaceSize[0] + v[2]*m_spaceSize[1]*m_spaceSize[0];
}

Voxel VoxelSpace::indexToVoxel(int index)
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

bool VoxelSpace::getVoxel(Voxel v)
{
	for(int i=0;i<3;++i)
		if(v[i]<0 || v[i]>=(int)m_spaceSize[i])
			return false; // all voxels outside the volume are empty

	return m_space[voxelToIndex(v)];
}

bool VoxelSpace::getVoxel(int x, int y, int z)
{
	return getVoxel(Voxel(x,y,z));
}

uint64_t VoxelSpace::getIntFromSurfacePoint(const Point& p)
{
	uint64_t x = (uint64_t)ceil(p.x());
	uint64_t y = (uint64_t)ceil(p.y());
	uint64_t z = (uint64_t)ceil(p.z());

	return x + (m_spaceSize[0]+1)*y +(m_spaceSize[0]+1)*(m_spaceSize[1]+1)*z;
}

Point VoxelSpace::getSurfacePointFromInt(uint64_t i)
{
	float z = i/((m_spaceSize[0]+1) * (m_spaceSize[1]+1)) - 0.5;
	long int remnantz = i%((m_spaceSize[0]+1) *( m_spaceSize[1]+1));
	float y = remnantz / (m_spaceSize[0]+1) - 0.5;
	float x = remnantz % (m_spaceSize[0]+1) - 0.5;
	
	return Point (x,y,z);
}

bool VoxelSpace::isBorder(int x,int y,int z)
{
	if(!getVoxel(x,y,z))
		return false;
	int offsets[6][3] = {{-1,0,0}, {1,0,0}, {0,-1,0},{0,1,0},{0,0,-1},{0,0,1}};

	for(int i=0;i<6;++i)
	{
		if(!getVoxel(x+offsets[i][0], y+offsets[i][1], z+offsets[i][2]))
		{
			return true;
		}
	}
	return false;
}

void VoxelSpace::triangulate()
{	
	std::map<uint64_t, int> edges;
	/* Conversion xyz --> int
		La idea es convertir una coordenada xyz en un entero, como si estuvieramos 
		trabajando en coordenadas enteras y lo metieramos todo en un array.

		Hacemos que:
			x-0.5 se considere x al convertir.
			x+0.5 se considere x+1 al convertir.
			El numero total de puntos en cada eje es m_spaceSize[i]+1
			
		Con lo que el int será:
			  x+y*(m_spaceSize[0]+1)+z*(m_spaceSize[0]+1)*(m_spaceSize[1]+1)
	   */
	for(int x=-1;x<=(int)m_spaceSize[0]+1; ++x)
		for(int y=-1;y<=(int)m_spaceSize[1]+1; ++y)
			for(int z=-1;z<=(int)m_spaceSize[2]+1; ++z)
			{
				bool facesToAdd[3] = {false,false,false};
				bool current = getVoxel(x,y,z);

				if(current != getVoxel(x-1,y,z)) facesToAdd[0] = true; 
				if(current != getVoxel(x,y-1,z)) facesToAdd[1] = true;
				if(current != getVoxel(x,y,z-1)) facesToAdd[2] = true;

				if(facesToAdd[0]) // Anyadimos la cara de la izda
				{
					Point p1(x-0.5,y-0.5,z+0.5);
					Point p2(x-0.5,y-0.5,z-0.5);
					Point p3(x-0.5,y+0.5,z-0.5);
					Point p4(x-0.5,y+0.5,z+0.5);

					uint64_t ip1 = getIntFromSurfacePoint(p1);
					uint64_t ip2 = getIntFromSurfacePoint(p2);
					uint64_t ip3 = getIntFromSurfacePoint(p3);
					//uint64_t ip4 = getIntFromSurfacePoint(p4);
					m_points.insert(p1); m_points.insert(p2); m_points.insert(p3); m_points.insert(p4);

					edges[Edge(ip2,Edge::DIR_Y).getID()]++;
					edges[Edge(ip2,Edge::DIR_Z).getID()]++;
					edges[Edge(ip1,Edge::DIR_Y).getID()]++;
					edges[Edge(ip3,Edge::DIR_Z).getID()]++;

					Face f(p1,p2,p3,p4);
					f.normal[0] = -1, f.normal[1] = 0, f.normal[2] = 0;
					m_faces.insert(f);
				}
				if(facesToAdd[1]) // Anyadimos la cara del frente
				{
					Point p1(x-0.5,y-0.5,z-0.5);
					Point p2(x+0.5,y-0.5,z-0.5);
					Point p3(x+0.5,y-0.5,z+0.5);
					Point p4(x-0.5,y-0.5,z+0.5);

					uint64_t ip1 = getIntFromSurfacePoint(p1);
					uint64_t ip2 = getIntFromSurfacePoint(p2);
					//uint64_t ip3 = getIntFromSurfacePoint(p3);
					uint64_t ip4 = getIntFromSurfacePoint(p4);

					m_points.insert(p1); m_points.insert(p2); m_points.insert(p3); m_points.insert(p4);

					edges[Edge(ip1,Edge::DIR_X).getID()]++;
					edges[Edge(ip1,Edge::DIR_Z).getID()]++;
					edges[Edge(ip2,Edge::DIR_Z).getID()]++;
					edges[Edge(ip4,Edge::DIR_X).getID()]++;
				
					Face f(p1,p2,p3,p4);
					f.normal[0] = 0, f.normal[1] = 0, f.normal[2] = 1;
					m_faces.insert(f);
		
				}
				if(facesToAdd[2]) // Anyadimos la cara de arriba
				{
					Point p1(x+0.5,y+0.5,z-0.5);
					Point p2(x+0.5,y-0.5,z-0.5);
					Point p3(x-0.5,y-0.5,z-0.5);
					Point p4(x-0.5,y+0.5,z-0.5);

					//uint64_t ip1 = getIntFromSurfacePoint(p1);
					uint64_t ip2 = getIntFromSurfacePoint(p2);
					uint64_t ip3 = getIntFromSurfacePoint(p3);
					uint64_t ip4 = getIntFromSurfacePoint(p4);
					
					m_points.insert(p1); m_points.insert(p2); m_points.insert(p3); m_points.insert(p4);

					edges[Edge(ip3,Edge::DIR_X).getID()]++;
					edges[Edge(ip3,Edge::DIR_Y).getID()]++;
					edges[Edge(ip4,Edge::DIR_X).getID()]++;
					edges[Edge(ip2,Edge::DIR_Y).getID()]++;


					Face f(p1,p2,p3,p4);
					f.normal[0] = 0, f.normal[1] = 1, f.normal[2] = 0;
					m_faces.insert(f);
				}
			}

	int numPoints = m_points.size();
	int numEdges = edges.size();
	int numFaces = m_faces.size();
	int numVolumes = countVolumes();
	int numComponents = countComponents();
	int numHoles =(-numComponents + numPoints - numEdges + numFaces - numVolumes)/-2; 

	printf("Num components: %i\n", numComponents);
	printf("Num points: %i\n", numPoints);
	printf("Num edges: %i\n", numEdges);
	printf("Num faces: %i\n", numFaces);
	printf("Num volumes: %i\n",numVolumes);
	printf("Number of holes: %i\n",numHoles );

	std::map<uint64_t,int>::iterator it;
	std::map<uint64_t,int>::iterator end = edges.end();

	//long int maxPoint = getIntFromSurfacePoint(Point(m_spaceSize[0]+0.5, m_spaceSize[1]+0.5, m_spaceSize[2]+0.5));

	for(it=edges.begin();it!=end; ++it)
	{
		std::pair<uint64_t,int> p = *it;
		if(p.second > 2)
		{
			Point point = getSurfacePointFromInt(p.first>>2);
			Point tpoint(point);

			if(p.first & 0x10) // es eje Z
				tpoint.z()+=1.0;
			else if(p.first & 0x01) // es el eje Y
				tpoint.y()+=1.0;
			else // tiene que ser el X
				tpoint.x()+=1.0;
			m_nonmaniEdges.push_back(std::pair<Point,Point>(point,tpoint));
		}
	}
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
			int offsetTable[] = {-1,0,0,	1,0,0,		0,-1,0,		0,1,0,		0,0,-1,		0,0,1};
			for(int o=0;o<6;++o)
			{
				int * offset = offsetTable + 3*o;
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
					{
						if(getVoxel(vnow.x()+x,vnow.y()+y,vnow.z()+z))
							toRemoveNext.insert(Voxel(vnow.x()+x,vnow.y()+y,vnow.z()+z));
					}
		}
		toRemoveNow = toRemoveNext;
		toRemoveNext.clear();
	}
}

int VoxelSpace::countComponents()
{
	int totalNumVoxels = m_spaceSize[0] * m_spaceSize[1] * m_spaceSize[2];
	bool * spaceBackup = new bool[totalNumVoxels];
	memcpy(spaceBackup, m_space, sizeof(bool) * totalNumVoxels);
	int numComponents = 0;
	for(unsigned int x=0;x<m_spaceSize[0];++x)
		for(unsigned int y=0;y<m_spaceSize[1];++y)
			for(unsigned int z=0;z<m_spaceSize[2];++z)
			{
				if(getVoxel(x,y,z)) // found a new component
				{
					++numComponents;
					emptyComponent(Voxel(x,y,z));
				}
			}
	delete [] m_space;
	m_space = spaceBackup;
	return numComponents;
}

int VoxelSpace::countVolumes()
{
	int totalNumVoxels = m_spaceSize[0] * m_spaceSize[1] * m_spaceSize[2];
	bool * spaceBackup = new bool[totalNumVoxels];
	memcpy(spaceBackup, m_space, sizeof(bool) * totalNumVoxels);
	int numVolumes= 0;
	for(unsigned int x=0;x<m_spaceSize[0];++x)
		for(unsigned int y=0;y<m_spaceSize[1];++y)
			for(unsigned int z=0;z<m_spaceSize[2];++z)
			{
				if(getVoxel(x,y,z)) // found a new volume
				{
					++numVolumes;
					emptyVolume(Voxel(x,y,z));
				}
			}
	delete [] m_space;
	m_space = spaceBackup;
	return numVolumes;
}

