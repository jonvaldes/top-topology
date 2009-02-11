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

	geom::Vector3D v1 = m_points[f[0]] - m_points[f[1]];
	geom::Vector3D v2 = m_points[f[2]] - m_points[f[1]];
	geom::Vector3D n1 = v2.vectorProduct(v1);

	geom::Vector3D v3 = m_points[f[2]] - m_points[f[3]];
	geom::Vector3D v4 = m_points[f[0]] - m_points[f[3]];
	geom::Vector3D n2 = v4.vectorProduct(v3);

	m_faceNormals.push_back( (n2+n1).getNormalized());
}

void Surface::render()
{
	int numFaces = m_faces.size();
	glBegin(GL_QUADS);
	for(int i=0;i<numFaces;++i)
	{
		glNormal3fv(m_faceNormals[i].dv);
		glColor3fv(m_points[m_faces[i][0]].v);
		glVertex3fv(m_points[m_faces[i][0]].v);
		glVertex3fv(m_points[m_faces[i][1]].v);
		glVertex3fv(m_points[m_faces[i][2]].v);
		glVertex3fv(m_points[m_faces[i][3]].v);
	}
	glEnd();
}
