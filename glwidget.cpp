#include <QtGui>
#include <math.h>
#include "glwidget.h"
#include "PortableGL.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
	m_voxelSpace = new Voxel::VoxelSpace(100,100,100,0);

	m_voxelSpace->addBall(Voxel::Point(20,20,20),18);
	m_voxelSpace->addBall(Voxel::Point(40,40,30),30);
	m_voxelSpace->removeCilinder(Voxel::Point(30,30,5),Voxel::Point(30,30,50),10);
	m_voxelSpace->removeCilinder(Voxel::Point(40,40,30),Voxel::Point(90,15,5),10);
	m_voxelSpace->removeCilinder(Voxel::Point(40,40,35),Voxel::Point(-90,-15,5),10);

	m_voxelSpace->addBall(Voxel::Point(65,65,65),20);

	m_camera = new glutil::FreeCamera(geom::Point3D(0,50,0), 0,0,45);

	m_voxelSpace->triangulate();
	lastButton = 0; 
	m_wireframe = false;
	m_showNonManifold = false;
}


GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    makeCurrent();
	qglClearColor(QColor::fromRgb(0,0,0));

    glEnable(GL_DEPTH_TEST);

}

void GLWidget::paintGL()
{
    makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Dibujamos todo el contenido
	if(m_wireframe)
	    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glLoadIdentity();
	m_camera->setAsGLCamera(widgetWidth,widgetHeight);
	
	std::set<Voxel::Face> & faces = m_voxelSpace->triangulated();
	std::set<Voxel::Face>::iterator iter;
	std::set<Voxel::Face>::iterator end = faces.end();
	glBegin(GL_QUADS);
	{
		for(iter = faces.begin(); iter!=end;iter++)
		{
			const Voxel::Face &f = *iter;
			glColor3f(f[0][0]/100.0,f[0][1]/100.0,f[0][2]/100.0);
			glVertex3f(f[0][0],f[0][1],f[0][2]);
			glVertex3f(f[1][0],f[1][1],f[1][2]);
			glVertex3f(f[2][0],f[2][1],f[2][2]);
			glVertex3f(f[3][0],f[3][1],f[3][2]);
		}
	}
	glEnd();
	    
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	typedef std::vector<std::pair<Voxel::Point, Voxel::Point> > EdgesVec;

	EdgesVec & points = m_voxelSpace->nonManifoldEdges();
	EdgesVec::iterator iter2;
	EdgesVec::iterator end2 = points.end();

	if(m_showNonManifold)
	{
		glDisable(GL_DEPTH_TEST);
		glBegin(GL_LINES);
		{
			glColor3f(1.0,1.0,1.0);
			for(iter2 = points.begin(); iter2!=end2;++iter2)
			{
				Voxel::Point p1 = iter2->first;
				Voxel::Point p2 = iter2->second;
				glVertex3f(p1.x(),p1.y(),p1.z());
				glVertex3f(p2.x(),p2.y(),p2.z());
			}
		}
		glEnd();
		glEnable(GL_DEPTH_TEST);
	}
}

void GLWidget::resizeGL(int width, int height)
{
    widgetWidth = width;
    widgetHeight = height;
    glViewport(0, 0, width, height);
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
	m_camera->advance(event->delta()/100.0);
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
	if(event->button() == Qt::RightButton)
		lastButton =1;
	else
		lastButton = 0;
}

void GLWidget::keyPressEvent ( QKeyEvent * event )
{
	if(event->key() == Qt::Key_W)
		m_camera->advance(1.0);

	if(event->key() == Qt::Key_S)
		m_camera->advance(-1.0);

	if(event->key() == Qt::Key_A)
		m_camera->strafeLeft(1.0);

	if(event->key() == Qt::Key_D)
		m_camera->strafeRight(1.0);

    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

	lastPos = event->pos();

	if(lastButton == 1)
	{
		m_camera->strafeLeft(dx/10.0);
		m_camera->moveUp(dy/10.0);
	}
	else
		m_camera->mouseLookTurn(dx/60.0,dy/60.0);

    updateGL();
}

void GLWidget::setWireframe(bool showWire)
{
	m_wireframe = showWire;
	updateGL();
}

void GLWidget::setShowNonManifold(bool showEdges)
{
	m_showNonManifold = showEdges;
	updateGL();
}

