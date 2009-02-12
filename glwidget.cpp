#include <QtGui>
#include <cmath>
#include "glwidget.h"
#include "PortableGL.h"
#include "util/OBJParser.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
	lastButton = 0; 
	m_wireframe = true;
	m_showFaces = true;
	m_lightAngle = M_PI;
	m_mergeTime = 0;
	m_mustSpherify = false;
	m_timer = 0;
	for(int i=0;i<4;++i)
		m_keysStatus[i] = false;
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
	glEnable(GL_COLOR_MATERIAL);

	GLfloat ambientLight[]={0.4,0.35,0.35,1.0};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,ambientLight);

	m_camera = new glutil::FreeCamera(geom::Point3D(0,2,10), 0,0,60);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glEnable(GL_DEPTH_TEST);
}

void GLWidget::paintGL()
{
    makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	m_camera->setAsGLCamera(widgetWidth,widgetHeight);

	m_surface.render(m_mergeTime, m_wireframe, m_showFaces);

	// We draw the X and Z axis
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBegin(GL_LINES);
	{
		glColor4f(0.8,0.0,0.0,0.4); glVertex3f(-10.0,0.0,  0.0); glVertex3f(10.0,0.0, 0.0);
		glColor4f(0.0,0.8,0.0,0.4); glVertex3f(  0.0,0.0,-10.0); glVertex3f( 0.0,0.0,10.0);
		glColor4f(0.5,0.5,0.5,0.5);
		for(int i=0;i<=20;++i)
		{
			glVertex3f(-10,0.0,i-10);
			glVertex3f(10,0.0,i-10);
			glVertex3f(i-10,0.0,-10);
			glVertex3f(i-10,0.0,10);
		}
	}
	glEnd();
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);
	setGLLight();
}

void GLWidget::resizeGL(int width, int height)
{
    widgetWidth = width;
    widgetHeight = height;
    glViewport(0, 0, width, height);
}

void GLWidget::focusOutEvent ( QFocusEvent * )  
{
	for(int i=0;i<4;++i)
		m_keysStatus[i] = false;
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
	m_camera->advance(event->delta()/100.0);
	queueUpdate();
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
		m_keysStatus[KEY_W] = true;
	if(event->key() == Qt::Key_S)
		m_keysStatus[KEY_S] = true;
	if(event->key() == Qt::Key_A)
		m_keysStatus[KEY_A] = true;
	if(event->key() == Qt::Key_D)
		m_keysStatus[KEY_D] = true;

	queueUpdate();
}

void GLWidget::keyReleaseEvent(QKeyEvent * event)
{
	if(event->key() == Qt::Key_W)
		m_keysStatus[KEY_W] = false;
	if(event->key() == Qt::Key_S)
		m_keysStatus[KEY_S] = false;
	if(event->key() == Qt::Key_A)
		m_keysStatus[KEY_A] = false;
	if(event->key() == Qt::Key_D)
		m_keysStatus[KEY_D] = false;
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

	queueUpdate();
}

void GLWidget::setWireframe(bool showWire)
{
	m_wireframe = showWire;
	queueUpdate();
}

void GLWidget::setShowFaces(bool showFaces)
{
	m_showFaces = showFaces;
	queueUpdate();
}

void GLWidget::openOBJFile()
{
	QString fn = QFileDialog::getOpenFileName( this, tr("Open OBJ model file"), "./shapes/", tr( "OBJ Files (*.obj);;All Files (*.*)" ));
	if ( fn.isEmpty() )
		return;
	std::string filename = fn.toStdString();
	modelfile::OBJParser parser(filename.c_str());
	m_surface = parser.getSurface();
	m_backupSurface = m_surface;
	emit surfacePoints(QString("").setNum(m_surface.getNumPoints()));
	emit surfaceEdges(QString("").setNum(m_surface.getNumEdges()));
	emit surfaceFaces(QString("").setNum(m_surface.getNumFaces()));

	queueUpdate();
}

void GLWidget::setLightAngle(int angle)
{
	m_lightAngle = angle/10.0f / 180.0 * M_PI;
	queueUpdate();
}

void GLWidget::timerEvent(QTimerEvent *)
{
	m_mergeTime+=m_mergeSpeed;

	bool mergeStopped = (m_mergeSpeed == 0);

	if(m_mergeTime >= 1.0)
	{
		m_surface.mergeLastFace();
		m_mergeTime = 0;
		if(m_surface.getNumFaces() == 0)
			mergeStopped = true;
	}

	if(m_keysStatus[KEY_W])
		m_camera->advance(0.2);

	if(m_keysStatus[KEY_S])
		m_camera->advance(-0.2);

	if(m_keysStatus[KEY_A])
		m_camera->strafeLeft(0.2);

	if(m_keysStatus[KEY_D])
		m_camera->strafeRight(0.2);

	bool cameraStopped = true;
	for(int i=0;i<4;++i)
		if(m_keysStatus[i])
		{
			cameraStopped = false;
			break;
		}
	if(m_mustSpherify)
		for(int i=0;i<4;++i)
			m_surface.applyLoveAndHate(0.01);

	if(mergeStopped && cameraStopped && !m_mustSpherify)
	{
		killTimer(m_timer);
		m_timer = 0;
	}
	updateGL();
}

void GLWidget::setMergeSpeed(int speed)
{
	m_mergeSpeed = speed/500.0;
	queueUpdate();
}

void GLWidget::queueUpdate()
{
	if(!m_timer)
		m_timer = startTimer(1.0/120.0*1000.0);
}

void GLWidget::resetCamera()
{
	m_camera->setPosition(geom::Point3D(0,2,10));
	m_camera->setPitchAndYaw(0,0);
	queueUpdate();
}

void GLWidget::resetSurface()
{
	m_surface = m_backupSurface;
	queueUpdate();
}

void GLWidget::mustSpherify(bool must)
{
	m_mustSpherify = must;

	queueUpdate();
}
