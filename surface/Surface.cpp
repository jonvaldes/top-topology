#include "Surface.h"
#include "../PortableGL.h"
#include <cstdio>
using namespace surface;

Surface::Surface()
{

}

void Surface::addPoint(const geom::Point3D &p)
{
	m_points.push_back(p);
}

void Surface::addFace(const Face &f)
{
	m_faces.push_back(f);
	m_edges.addEdge(f[0], f[1]);
	m_edges.addEdge(f[1], f[2]);
	m_edges.addEdge(f[2], f[3]);
	m_edges.addEdge(f[3], f[0]);
}

void Surface::render()
{
	int numFaces = m_faces.size();
	printf("Rendering %i faces\n",numFaces);
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glBegin(GL_QUADS);
	for(int i=0;i<numFaces;++i)
	{
		glColor3fv(m_points[m_faces[i][0]].v);
		glVertex3fv(m_points[m_faces[i][0]].v);
		glVertex3fv(m_points[m_faces[i][1]].v);
		glVertex3fv(m_points[m_faces[i][2]].v);
		glVertex3fv(m_points[m_faces[i][3]].v);
	}
	glEnd();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}
