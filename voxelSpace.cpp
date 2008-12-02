#include "voxelSpace.h"
#include <set>
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

void VoxelSpace::triangulate()
{	
	std::set<Edge> edges;
	for(int x=-1;x<=(int)m_spaceSize[0]+1; ++x)
		for(int y=-1;y<=(int)m_spaceSize[1]+1; ++y)
			for(int z=-1;z<=(int)m_spaceSize[2]+1; ++z)
			{
				bool facesToAdd[3] = {false,false,false};

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
					Point p3(x-0.5,y+0.5,z+0.5);
					Point p4(x-0.5,y+0.5,z-0.5);

					m_points.insert(p1); m_points.insert(p2); m_points.insert(p3); m_points.insert(p4);

					edges.insert(Edge(p1,p2));	edges.insert(Edge(p2,p3));	edges.insert(Edge(p3,p1));	
					edges.insert(Edge(p2,p4));	edges.insert(Edge(p4,p3));

					m_faces.insert(Face(p1,p2,p3));
					m_faces.insert(Face(p2,p4,p3));
				}
				if(facesToAdd[1]) // Anyadimos la cara del frente
				{
					Point p1(x-0.5,y-0.5,z-0.5);
					Point p2(x+0.5,y-0.5,z-0.5);
					Point p3(x-0.5,y-0.5,z+0.5);
					Point p4(x+0.5,y-0.5,z+0.5);

					m_points.insert(p1); m_points.insert(p2); m_points.insert(p3); m_points.insert(p4);

					edges.insert(Edge(p1,p2));	edges.insert(Edge(p2,p3));	edges.insert(Edge(p3,p1));	
					edges.insert(Edge(p2,p4));	edges.insert(Edge(p4,p3));
					
					m_faces.insert(Face(p1,p2,p3));
					m_faces.insert(Face(p2,p4,p3));
		
				}
				if(facesToAdd[2]) // Anyadimos la cara de la izda
				{
					Point p1(x+0.5,y+0.5,z-0.5);
					Point p2(x+0.5,y-0.5,z-0.5);
					Point p3(x-0.5,y+0.5,z-0.5);
					Point p4(x-0.5,y-0.5,z-0.5);

					m_points.insert(p1); m_points.insert(p2); m_points.insert(p3); m_points.insert(p4);

					edges.insert(Edge(p1,p2));	edges.insert(Edge(p2,p3));	edges.insert(Edge(p3,p1));	
					edges.insert(Edge(p2,p4));	edges.insert(Edge(p4,p3));
					
					// --+ --- -++
					// --- -+- -++
					m_faces.insert(Face(p1,p2,p3));
					m_faces.insert(Face(p2,p4,p3));
	
				}
			}

	printf("Num points: %i\n",(int) m_points.size());
	printf("Num edges: %i\n", (int) edges.size());
	printf("Num faces: %i\n", (int) m_faces.size());
	printf("Euler charactertic: %i\n", m_points.size() - edges.size() + m_faces.size());

	std::set<Edge>::iterator it;
	std::set<Edge>::iterator end = edges.end();
/*
	for(it=edges.begin();it!=end; ++it)
	{
		Edge e = *it;
		printf("Edge: %f,%f,%f --> %f,%f,%f\n", e[0][0]-0.5,e[0][1]-0.5,e[0][2]-0.5, e[1][0]-0.5,e[1][1]-0.5,e[1][2]-0.5);
	}
	*/
}
