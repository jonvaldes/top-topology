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

void Surface::render(float lastFaceMergingPercentage, bool showWireframe, bool showFaces)
{
	int numFaces = m_faces.size();

	int lastFace = numFaces-1;
	PointID firstPointID = LONG_MAX, thirdPointID = LONG_MAX;
	geom::Point3D finalFirstPoint;
	geom::Point3D finalThirdPoint;

	if(lastFace >= 0)
	{
		firstPointID = m_faces[lastFace][0];
		thirdPointID= m_faces[lastFace][2];

		geom::Point3D firstPoint = m_points[firstPointID];
		geom::Point3D thirdPoint = m_points[thirdPointID];

		geom::Vector3D moveVector = (firstPoint - thirdPoint)/2;

		finalFirstPoint = firstPoint + (-moveVector) * lastFaceMergingPercentage;
		finalThirdPoint = thirdPoint + moveVector * lastFaceMergingPercentage;
	}

	if(showFaces)
	{
		glColor4f(0.8,0.8,0.8,1.0);
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(1,1);
		glBegin(GL_QUADS);
		for(int i=0;i<numFaces;++i)
		{
			glNormal3fv(m_faceNormals[i].dv);
			for(int j=0;j<4;++j)
			{
				PointID pid = m_faces[i][j];
				if(pid == firstPointID)
					glVertex3fv(finalFirstPoint.v);
				else if (pid == thirdPointID)
					glVertex3fv(finalThirdPoint.v);
				else
					glVertex3fv(m_points[pid].v);
			}
		}
		glEnd();
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	if(showWireframe)
	{
		AdjacencyList::const_iterator it;
		AdjacencyList::const_iterator end = m_edges.end();
		glBegin(GL_LINES);
		glColor4f(0.0,0.0,0.0,1.0);
		for(it = m_edges.begin(); it!=end; ++it)
		{
			PointID pid = it->first;
			if(pid == firstPointID)
				glVertex3fv(finalFirstPoint.v);
			else if (pid == thirdPointID)
				glVertex3fv(finalThirdPoint.v);
			else
				glVertex3fv(m_points[pid].v);

			pid = it->second;
			if(pid == firstPointID)
				glVertex3fv(finalFirstPoint.v);
			else if (pid == thirdPointID)
				glVertex3fv(finalThirdPoint.v);
			else
				glVertex3fv(m_points[pid].v);
		}
		glEnd();
	}
}

void Surface::mergeLastFace()
{

	// Here, the first and third points in the last face should be merged
	// (I'm saying last, because when using a vector you can remove the last element in a fast way, using pop_back)

	if(m_faces.size() == 0)
		return;
	PointID p1 = m_faces.back()[0];
	PointID p2 = m_faces.back()[2];

	geom::Point3D middlePoint;
	for(int i=0;i<3;++i)
		middlePoint.v[i] = (m_points[p1].v[i] + m_points[p2].v[i])/2;

	m_points[p1] = middlePoint;

	int numFaces = m_faces.size();
	for(int i=0;i<numFaces;++i)
		for(int j=0;j<4;++j)
			if(m_faces[i][j] == p2)
				m_faces[i][j] = p1;

	m_edges.mergePoint(p2,p1);

	m_discardedPoints.insert(p2);
	m_faces.pop_back();
}


void Surface::applyLoveAndHate( float timeStep)
{
	int numPoints = m_points.size();
	float love, hate, totalForce;
	geom::Vector3D qToP;
	geom::Vector3D pToQ;
	for(int i=0;i<numPoints;++i)
	{
		if(m_discardedPoints.find(i) != m_discardedPoints.end()) // if the point is among the discarded ones
			continue;
		geom::Point3D p = m_points[i];
		geom::Point3D & pref = m_points[i];

		std::vector<PointID> connected = m_edges.getConnectedTo(i);
		int numConnected = connected.size();
		for(int n=0;n<numConnected;++n)
		{
			const geom::Point3D & q = m_points[connected[n]];
			float dist = p.distance(q);
			if(dist < 0.05)
				dist = 0.05f;
			pToQ = q-p;
			pref = pref + pToQ/ sqrt(dist) * 0.005f;
		}
	

		// naive implementation, O(n*n) complexity
		// general Love-hate
		for(int j=0;j<numPoints;++j)
		{
			if(j == i)
				continue;
			//if(m_discardedPoints.find(j) != m_discardedPoints.end()) // if the point is among the discarded ones
			//	continue;
			const geom::Point3D & q = m_points[j];
			float sqDist = p.distanceSquared(q);
			qToP = p-q;

			hate = (1.0f/sqDist) * 0.005f;
			pref = pref + qToP * (hate* timeStep);
		}
	}

}
