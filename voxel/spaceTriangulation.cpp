#include "spaceTriangulation.h"
#include <set>
#include <map>
#include <cmath>
#include <stdio.h>

using namespace voxel;

SpaceTriangulation::SpaceTriangulation(const VoxelSpace & space) : m_space(space)
{
	
}

SpaceTriangulation::~SpaceTriangulation()
{

}

PointID SpaceTriangulation::getIDFromSurfacePoint(const Point& p) const
{
	uint64_t x = (uint64_t)ceil(p.x());
	uint64_t y = (uint64_t)ceil(p.y());
	uint64_t z = (uint64_t)ceil(p.z());

	return x + (m_space.getSize(0)+1)*y +(m_space.getSize(0)+1)*(m_space.getSize(1)+1)*z;
}

Point SpaceTriangulation::getSurfacePointFromID(PointID i) const
{
	float z = i/((m_space.getSize(0)+1) * (m_space.getSize(1)+1)) - 0.5;
	long int remnantz = i%((m_space.getSize(0)+1) *( m_space.getSize(1)+1));
	float y = remnantz / (m_space.getSize(0)+1) - 0.5;
	float x = remnantz % (m_space.getSize(0)+1) - 0.5;
	
	return Point (x,y,z);
}

void SpaceTriangulation::triangulate()
{	
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
	for(int x=-1;x<=(int)m_space.getSize(0)+1; ++x)
		for(int y=-1;y<=(int)m_space.getSize(1)+1; ++y)
			for(int z=-1;z<=(int)m_space.getSize(2)+1; ++z)
			{
				bool facesToAdd[3] = {false,false,false};
				
				if(m_space.getVoxel(x,y,z)) // El voxel actual esta lleno
				{
					if(!m_space.getVoxel(x-1,y,z)) facesToAdd[0] = true; // Pero los anteriores estaban vacios
					if(!m_space.getVoxel(x,y-1,z)) facesToAdd[1] = true;
					if(!m_space.getVoxel(x,y,z-1)) facesToAdd[2] = true;
				}
				else	// El voxel actual esta lleno
				{	
					if(m_space.getVoxel(x-1,y,z)) facesToAdd[0] = true; // Pero los anteriores estaban vacios
					if(m_space.getVoxel(x,y-1,z)) facesToAdd[1] = true;
					if(m_space.getVoxel(x,y,z-1)) facesToAdd[2] = true;
				}
				//if(facesToAdd[0] || facesToAdd[1] || facesToAdd[2])
				//	printf("Faces: point: %i,%i,%i, faces:%i,%i,%i\n", x,y,z, facesToAdd[0],facesToAdd[1], facesToAdd[2]);

				if(facesToAdd[0]) // Anyadimos la cara de la izda
				{
					Point p1(x-0.5,y-0.5,z+0.5);
					Point p2(x-0.5,y-0.5,z-0.5);
					Point p3(x-0.5,y+0.5,z-0.5);
					Point p4(x-0.5,y+0.5,z+0.5);

					PointID ip1 = getIDFromSurfacePoint(p1);
					PointID ip2 = getIDFromSurfacePoint(p2);
					PointID ip3 = getIDFromSurfacePoint(p3);
					PointID ip4 = getIDFromSurfacePoint(p4);

					m_points.insert(ip1); m_points.insert(ip2); m_points.insert(ip3); m_points.insert(ip4);
					m_edges.addEdge(ip1,ip2);
					m_edges.addEdge(ip2,ip3);
					m_edges.addEdge(ip3,ip4);
					m_edges.addEdge(ip4,ip1);

					m_faces.insert(Face(p1,p2,p3,p4));
				}
				if(facesToAdd[1]) // Anyadimos la cara del frente
				{
					Point p1(x-0.5,y-0.5,z-0.5);
					Point p2(x+0.5,y-0.5,z-0.5);
					Point p3(x+0.5,y-0.5,z+0.5);
					Point p4(x-0.5,y-0.5,z+0.5);

					PointID ip1 = getIDFromSurfacePoint(p1);
					PointID ip2 = getIDFromSurfacePoint(p2);
					PointID ip3 = getIDFromSurfacePoint(p3);
					PointID ip4 = getIDFromSurfacePoint(p4);

					m_points.insert(ip1); m_points.insert(ip2); m_points.insert(ip3); m_points.insert(ip4);
					
					m_edges.addEdge(ip1,ip2);
					m_edges.addEdge(ip2,ip3);
					m_edges.addEdge(ip3,ip4);
					m_edges.addEdge(ip4,ip1);

					m_faces.insert(Face(p1,p2,p3,p4));
				}
				if(facesToAdd[2]) // Anyadimos la cara de arriba
				{
					Point p1(x+0.5,y+0.5,z-0.5);
					Point p2(x+0.5,y-0.5,z-0.5);
					Point p3(x-0.5,y-0.5,z-0.5);
					Point p4(x-0.5,y+0.5,z-0.5);

					PointID ip1 = getIDFromSurfacePoint(p1);
					PointID ip2 = getIDFromSurfacePoint(p2);
					PointID ip3 = getIDFromSurfacePoint(p3);
					PointID ip4 = getIDFromSurfacePoint(p4);
					
					m_points.insert(ip1); m_points.insert(ip2); m_points.insert(ip3); m_points.insert(ip4);

					m_edges.addEdge(ip1,ip2);
					m_edges.addEdge(ip2,ip3);
					m_edges.addEdge(ip3,ip4);
					m_edges.addEdge(ip4,ip1);

					m_faces.insert(Face(p1,p2,p3,p4));
				}
			}
}

int SpaceTriangulation::countComponents() const
{
	SpaceTriangulation p = *this;
	return p.countComponentsDestructively();
}

int SpaceTriangulation::countComponentsDestructively()
{
	int components = 0;
	while( m_edges.totalEdgesCount() > 0)
	{
		AdjacencyList::const_iterator it = m_edges.begin();
		m_edges.removeComponent(it->first);
		++components;
	}
	return components;
}

int SpaceTriangulation::calculateNumHoles() const
{
	size_t components = this->countComponents(); // can also use m_space.countComponents to use the voxel-based component count
	size_t points = m_points.size();
	size_t edges = m_edges.totalEdgesCount();
	size_t faces = m_faces.size();
	size_t volumes = m_space.countVolumes();

	int holes = points - edges + faces - volumes - components;
	return holes/-2;
}

SpaceTriangulation SpaceTriangulation::getComponent(PointID p) const
{
	SpaceTriangulation result(m_space);
	result.m_edges = m_edges.getComponent(p);
	AdjacencyList::const_iterator it ;
	AdjacencyList::const_iterator end = result.m_edges.end();
	for(it = result.m_edges.begin(); it!=end; ++it)
		result.m_points.insert(it->first);

	FaceSet::const_iterator faceIt;
	FaceSet::const_iterator faceEnd = m_faces.end();
	for(faceIt = m_faces.begin(); faceIt!=faceEnd;++faceIt)
	{
		for(int i=0;i<4;++i)
			if(result.m_points.find(getIDFromSurfacePoint((*faceIt)[i])) != result.m_points.end())
			{
				result.m_faces.insert(*faceIt);
				break;
			}
	}
	return result;
}
