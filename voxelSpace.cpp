#include "voxelSpace.h"
#include <set>
#include <map>
#include <cmath>
#include <stdio.h>

using namespace Voxel;

VoxelSpace::VoxelSpace(int widthVoxels, int heightVoxels, int depthVoxels, bool defaultValue)
{
	m_spaceSize[0] = widthVoxels;
	m_spaceSize[1] = heightVoxels;
	m_spaceSize[2] = depthVoxels;
	m_space = new bool[widthVoxels * heightVoxels * depthVoxels];
	for(int i=0;i<widthVoxels*heightVoxels*depthVoxels; ++i)
		m_space[i] = defaultValue;
}

VoxelSpace::~VoxelSpace()
{
	delete m_space;
}

float Point::distanceSquaredTo(const Point &p) const
{
	return	(v[0]-p.v[0]) * (v[0]-p.v[0]) +
			(v[1]-p.v[1]) * (v[1]-p.v[1]) +
			(v[2]-p.v[2]) * (v[2]-p.v[2]);
}

void VoxelSpace::addBall(Point center, float radius)
{
	int maxRadius = ceil(radius);
	float maxDistSquared = radius*radius;
	
	for(int x = center[0]-maxRadius; x<center[0]+maxRadius; ++x)
		for(int y = center[1]-maxRadius; y<center[1]+maxRadius; ++y)
			for(int z = center[2]-maxRadius; z<center[2]+maxRadius; ++z)
				if(center.distanceSquaredTo(Point(x,y,z)) <= maxDistSquared)
					setPoint(Point(x,y,z), true);
	/*	
	for(int z=0; z<m_spaceSize[2]; ++z)
	{
		for(int y=0; y<m_spaceSize[1]; ++y)
		{
			for(int x=0; x<m_spaceSize[0]; ++x)
			{
				printf("%s",getPoint(Point(x,y,z))?"X":"-");
			}
			printf("\n");
		}
		printf("\n\n\n");
	}
*/
}

void VoxelSpace::removeBall(Point center, float radius)
{
	int maxRadius = ceil(radius);
	float maxDistSquared = radius*radius;
	
	for(int x = center[0]-maxRadius; x<center[0]+maxRadius; ++x)
		for(int y = center[1]-maxRadius; y<center[1]+maxRadius; ++y)
			for(int z = center[2]-maxRadius; z<center[2]+maxRadius; ++z)
				if(center.distanceSquaredTo(Point(x,y,z)) <= maxDistSquared)
					setPoint(Point(x,y,z), false);
}

void VoxelSpace::removeNonConnected() // this tries to remove non-manifold-surface-creating cubes
{
	for(size_t z=0;z<m_spaceSize[2];++z)
		for(size_t y=0;y<m_spaceSize[2];++y)
			for(size_t x=0;x<m_spaceSize[0];++x)
			{
				bool value      = getPoint(x,y,z);
				bool valueUp    = getPoint(x,y-1,z);
				bool valueDown  = getPoint(x,y+1,z);
				bool valueLeft  = getPoint(x-1,y,z);
				bool valueRight = getPoint(x+1,y,z);
				bool valueFront = getPoint(x,y,z-1);
				bool valueBack  = getPoint(x,y,z+1);

				bool anyTrue = valueUp || valueDown || valueLeft || valueRight || valueFront || valueBack;
				bool anyFalse= !valueUp || !valueDown || !valueLeft || !valueRight || !valueFront || !valueBack;

				if(value && !anyTrue)
					setPoint(Point(x,y,z),false);
				else if(!value && !anyFalse)
					setPoint(Point(x,y,z),true);
			}
}

void VoxelSpace::removeCilinder(Point p1, Point p2, float radius)
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
					setPoint(Point(x,y,z), false);
			}
}

int VoxelSpace::pointToIndex(Point p)
{
	return p[0] + p[1]*m_spaceSize[0] + p[2]*m_spaceSize[1]*m_spaceSize[0];
}

Point VoxelSpace::indexToPoint(int index)
{
	Point p(0,0,0);
	p[2] = index / (m_spaceSize[0]*m_space[1]);
	int indexMinusZ = index % (m_spaceSize[0] * m_spaceSize[1]);
	p[1] = indexMinusZ % m_spaceSize[0];
	p[0] = indexMinusZ / m_spaceSize[0];
	return p;
}

void VoxelSpace::setPoint(Point p, bool value)
{
	if(p.x() < 0 || p.y()<0 || p.z()<0)
		return;
	if(p.x() >= m_spaceSize[0] || p.y() >= m_spaceSize[1] || p.z() >=m_spaceSize[2])
		return;
	m_space[pointToIndex(p)] = value;
}

bool VoxelSpace::getPoint(Point p)
{
	if(p[0]<0 || p[1]<0 || p[2]<0)
		return false;

	if(p[0]>=(int)m_spaceSize[0] || p[1] >=(int)m_spaceSize[1] || p[2]>=(int)m_spaceSize[2])
		return false;

	return m_space[pointToIndex(p)];
}

bool VoxelSpace::getPoint(int x, int y, int z)
{
	return getPoint(Point(x,y,z));
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
	if(!getPoint(x,y,z))
		return false;
	int offsets[6][3] = {{-1,0,0}, {1,0,0}, {0,-1,0},{0,1,0},{0,0,-1},{0,0,1}};

	for(int i=0;i<6;++i)
	{
		if(!getPoint(x+offsets[i][0], y+offsets[i][1], z+offsets[i][2]))
		{
			return true;
		}
	}
	return false;
}

void VoxelSpace::triangulate()
{	

	this->removeNonConnected();

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
				/*
				if(getPoint(x,y,z))
				{
					int offsets[17][3] = {{-1,-1,0}, {-1,0,-1},{-1,0,0}, {-1,0,1}, {-1,1,0}, 
						{0,-1,-1}, {0,-1,0}, {0,0,-1}, {0,0,1}, {0,1,-1}, {0,1,0}, {0,1,1}, 
						{1,-1,0}, {1,0,-1}, {1,0,0}, {1,0,1}, {1,1,0}};
				 //	int offsets[6][3] = {{-1,0,0}, {1,0,0}, {0,-1,0},{0,1,0},{0,0,-1},{0,0,1}};
					
					if(isBorder(x,y,z))
					{
						for(int i=0;i<17;++i)
							if(isBorder(x+offsets[i][0], y+offsets[i][1], z+offsets[i][2]))
								m_edges.push_back(std::pair<Point,Point>(Point(x+0.5,y+0.5,z+0.5), Point(x+offsets[i][0]+0.5, y+offsets[i][1]+0.5, z+offsets[i][2]+0.5)));
					}
				}
				*/
				if(getPoint(x,y,z)) // El voxel actual esta lleno
				{
					if(!getPoint(x-1,y,z)) facesToAdd[0] = true; // Pero los anteriores estaban vacios
					if(!getPoint(x,y-1,z)) facesToAdd[1] = true;
					if(!getPoint(x,y,z-1)) facesToAdd[2] = true;
				}
				else	// El voxel actual esta lleno
				{	
					if(getPoint(x-1,y,z)) facesToAdd[0] = true; // Pero los anteriores estaban vacios
					if(getPoint(x,y-1,z)) facesToAdd[1] = true;
					if(getPoint(x,y,z-1)) facesToAdd[2] = true;
				}
				//if(facesToAdd[0] || facesToAdd[1] || facesToAdd[2])
				//	printf("Faces: point: %i,%i,%i, faces:%i,%i,%i\n", x,y,z, facesToAdd[0],facesToAdd[1], facesToAdd[2]);

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

					m_faces.insert(Face(p1,p2,p3,p4));
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
					
					m_faces.insert(Face(p1,p2,p3,p4));
		
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

					m_faces.insert(Face(p1,p2,p3,p4));
				}
			}

	printf("Num points: %i\n",(int) m_points.size());
	printf("Num edges: %i\n", (int) edges.size());
	printf("Num faces: %i\n", (int) m_faces.size());
	printf("Euler characteristic: %li\n", (long int)m_points.size() - (long int)edges.size() + (long int)m_faces.size());

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
