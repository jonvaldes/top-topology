#include <QtGui>
#include <cmath>
#include "glwidget.h"
#include "PortableGL.h"
#include "util/OBJParser.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
	lastButton = 0; 
	m_wireframe = false;
	m_lightAngle = M_PI;
	m_mergeTime = 0;
	m_timer = 0;
}


GLWidget::~GLWidget()
{
}

void GLWidget::setGLLight()
{
	float ambientLight0[4] = {0.1,0.1,0.1,1.0f};
	float diffuseLight0[4] = {0.5,0.5,0.5,1.0f};
	float specularLight0[4] = {0.0,0.0,0.0,1.0f};
	float lightPosition0 [4] = {2, -4, 2,1.0};


	lightPosition0[0] = 20*sin(m_lightAngle - M_PI);
	lightPosition0[1] = 1.0;
	lightPosition0[2] = 20*cos(m_lightAngle - M_PI);

	glLightfv(GL_LIGHT0,GL_AMBIENT,ambientLight0);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,diffuseLight0);
	glLightfv(GL_LIGHT0,GL_SPECULAR,specularLight0);
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition0);

	glEnable(GL_LIGHT0);
}

void GLWidget::initializeGL()
{
    makeCurrent();
	qglClearColor(QColor::fromRgb(140,140,190));

	glEnable(GL_LIGHTING);

	GLfloat ambientLight[]={0.4,0.35,0.35,1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);

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
	setGLLight();

	m_surface.render(m_mergeTime);
    
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
	emit surfacePoints(QString("").setNum(m_surface.getNumPoints()));
	emit surfaceEdges(QString("").setNum(m_surface.getNumEdges()));
	emit surfaceFaces(QString("").setNum(m_surface.getNumFaces()));

	updateGL();
}

void GLWidget::setLightAngle(int angle)
{
	m_lightAngle = angle/10.0f / 180.0 * M_PI;
	updateGL();
}

void GLWidget::timerEvent(QTimerEvent *)
{
	m_mergeTime+=m_mergeSpeed;
	printf("MergeTime:%f\n",m_mergeTime);
	if(m_mergeTime >= 1.0)
	{
		m_surface.mergeLastFace();
		m_mergeTime = 0;
		if(m_surface.getNumFaces() == 0)
		{
			killTimer(m_timer);
			m_timer = 0;
		}
	}
	updateGL();
}

void GLWidget::setMergeSpeed(int speed)
{
	if(m_timer)
		killTimer(m_timer);

	m_mergeSpeed = speed/10000.0;
	printf("MergeSpeed set to %f\n",m_mergeSpeed);

	if(m_mergeSpeed != 0)
		m_timer = startTimer(1/60.0*1000.0);
}
