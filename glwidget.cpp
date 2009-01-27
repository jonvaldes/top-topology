#include <QtGui>
#include <cmath>
#include "glwidget.h"
#include "PortableGL.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
	m_voxelSpace.reset(new voxel::VoxelSpace(100,100,100,0));

	m_voxelSpace->addBall(voxel::Point(20,20,20),18);
	m_voxelSpace->addBall(voxel::Point(40,40,30),20);
	m_voxelSpace->removeCilinder(voxel::Point(30,30,5),voxel::Point(30,30,50),10);
	m_voxelSpace->removeCilinder(voxel::Point(40,40,30),voxel::Point(90,15,5),10);
	//m_voxelSpace->removeCilinder(voxel::Point(40,40,35),voxel::Point(-90,-15,5),10);

	m_voxelSpace->addBall(voxel::Point(65,65,65),20);

	m_triangulation.reset(new voxel::SpaceTriangulation(*m_voxelSpace));

	lastButton = 0; 
	m_wireframe = false;
	m_camera = new glutil::FreeCamera(geom::Point3D(-100,100,0), M_PI_2,M_PI_4,60);
}


GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    makeCurrent();
	qglClearColor(QColor::fromRgb(0,0,0));

	recalculateSurface();

    glEnable(GL_DEPTH_TEST);

}

void GLWidget::paintGL()
{
    makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_wireframe)
	    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

	glLoadIdentity();
	m_camera->setAsGLCamera(widgetWidth,widgetHeight);
	
	const voxel::FaceSet &faces = m_triangulation->faces();
	voxel::FaceSet::iterator iter;
	voxel::FaceSet::iterator end = faces.end();
	glBegin(GL_QUADS);
	{
		for(iter = faces.begin(); iter!=end;iter++)
		{
			const voxel::Face &f = *iter;
			glColor3f(f[0][0]/100.0,f[0][1]/100.0,f[0][2]/100.0);
			glVertex3f(f[0][0],f[0][1],f[0][2]);
			glVertex3f(f[1][0],f[1][1],f[1][2]);
			glVertex3f(f[2][0],f[2][1],f[2][2]);
			glVertex3f(f[3][0],f[3][1],f[3][2]);
		}
	}
	glEnd();
	    
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
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

void GLWidget::recalculateSurface()
{
	m_triangulation->triangulate();

	emit bettiNumberChanged("Not calculated");
	emit holesChanged(QString("").setNum(m_triangulation->calculateNumHoles()));
	emit volumesChanged(QString("").setNum(m_voxelSpace->countVolumes()));
	emit componentsChanged(QString("").setNum(m_triangulation->countComponents()));
	emit pointsChanged(QString("").setNum(m_triangulation->points().size()));
	emit edgesChanged(QString("").setNum(m_triangulation->edges().totalEdgesCount()));
	emit facesChanged(QString("").setNum(m_triangulation->faces().size()));
}

