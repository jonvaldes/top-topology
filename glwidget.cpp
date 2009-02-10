#include <QtGui>
#include <cmath>
#include "glwidget.h"
#include "PortableGL.h"
#include "util/OBJParser.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
	lastButton = 0; 
	m_wireframe = false;
}


GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    makeCurrent();
	qglClearColor(QColor::fromRgb(0,0,0));

	m_camera = new glutil::FreeCamera(geom::Point3D(-100,100,0), M_PI_2,M_PI_4,60);

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

	m_surface.render();
    
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

void GLWidget::openOBJFile()
{
	QString fn = QFileDialog::getOpenFileName( this, QString::null, tr( "OBJ Files (*.obj *.OBJ);;All Files (*)" ));
	if ( fn.isEmpty() )
		return;
	std::string filename = fn.toStdString();
	modelfile::OBJParser parser(filename.c_str());
	m_surface = parser.getSurface();
	updateGL();
}
