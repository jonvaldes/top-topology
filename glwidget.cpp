#include "shaderUtils.h"
#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include <iostream>
#include <fstream>
#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent)
{
	m_voxelSpace = new Voxel::VoxelSpace(100,100,100,0);
	m_voxelSpace->addBall(Voxel::Point(20,20,20),18);
	m_voxelSpace->addBall(Voxel::Point(40,40,30),26);
	m_voxelSpace->removeBall(Voxel::Point(30,30,5),10);
	m_voxelSpace->removeBall(Voxel::Point(30,30,10),10);
	m_voxelSpace->removeBall(Voxel::Point(30,30,15),10);
	m_voxelSpace->removeBall(Voxel::Point(30,30,20),10);
	m_voxelSpace->removeBall(Voxel::Point(30,30,25),10);
	m_voxelSpace->removeBall(Voxel::Point(30,30,30),10);
	m_voxelSpace->removeBall(Voxel::Point(30,30,35),10);
	m_voxelSpace->removeBall(Voxel::Point(30,30,40),10);
	m_voxelSpace->removeBall(Voxel::Point(30,30,45),10);
	m_voxelSpace->triangulate();
}


GLWidget::~GLWidget()
{
}

void GLWidget::initializeGL()
{
    makeCurrent();
	qglClearColor(QColor::fromRgb(100,100,255));

    glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_FLAT);

	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	float lightpos[3]={1.0,1.0,1.0};
	float lightdiffuse[3]={0.4,0.4,0.4};
	float lightspecular[3]={1.0,1.0,1.0};
	float lightambient[3]={0.1,0.1,0.1};

	glLightfv(GL_LIGHT0,GL_DIFFUSE,lightdiffuse);    //updates the light's diffuse colour
	glLightfv(GL_LIGHT0,GL_SPECULAR,lightspecular);  //updates the light's specular colour
	glLightfv(GL_LIGHT0,GL_AMBIENT,lightambient);    //updates the light's ambient colour


	cubeSizeLoc = glGetUniformLocation(shaderProgram, "cubeSide");

	zoom = 1.0;
	rotX = 0.0;
	rotY = 0.0;
	startTimer(1);
	timer.animationTimeValue = 0.0;
	timer.last_time = QTime::currentTime();
	mustShowCube = true;
	changeFractalIterations(-1);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

}

void GLWidget::timerEvent(QTimerEvent * event)
{
	timer.current_time = QTime::currentTime();
	timer.animationTimeValue+= timer.last_time.msecsTo(timer.current_time)* morphSpeed / 10000.0;
	updateGL();
	timer.last_time = timer.current_time;
}

void GLWidget::computeCube(vector<Point> * v, float cx, float cy, float cz, double side, int iterations)
{
	if(iterations == 0)
	{
		Point p;
		p.v[0] = cx;
		p.v[1] = cy;
		p.v[2] = cz;
		v->push_back(p);
	}
	else
	{
		// En el Menger sponge faltan los cuadrados de +/- x/y/z, y el del centro
		double d = side/3;
		computeCube(v,cx-d, cy-d, cz, d, iterations-1);
		computeCube(v,cx+d, cy+d, cz, d, iterations-1);
		computeCube(v,cx-d, cy+d, cz, d, iterations-1);
		computeCube(v,cx+d, cy-d, cz, d, iterations-1);
		
		computeCube(v,cx, cy-d, cz-d, d, iterations-1);
		computeCube(v,cx, cy+d, cz+d, d, iterations-1);
		computeCube(v,cx, cy+d, cz-d, d, iterations-1);
		computeCube(v,cx, cy-d, cz+d, d, iterations-1);

		computeCube(v,cx-d, cy, cz-d, d, iterations-1);
		computeCube(v,cx+d, cy, cz+d, d, iterations-1);
		computeCube(v,cx+d, cy, cz-d, d, iterations-1);
		computeCube(v,cx-d, cy, cz+d, d, iterations-1);

		computeCube(v,cx+d, cy+d, cz+d, d, iterations-1);
		computeCube(v,cx-d, cy-d, cz-d, d, iterations-1);
		computeCube(v,cx+d, cy+d, cz-d, d, iterations-1);
		computeCube(v,cx+d, cy-d, cz+d, d, iterations-1);
		computeCube(v,cx+d, cy-d, cz-d, d, iterations-1);
		computeCube(v,cx-d, cy+d, cz+d, d, iterations-1);
		computeCube(v,cx-d, cy+d, cz-d, d, iterations-1);
		computeCube(v,cx-d, cy-d, cz+d, d, iterations-1);
	}
}

void GLWidget::generateVBO(vector<Point>*p)
{
	float * tva = (float *) malloc(numPointsInVBO*3*sizeof(float));
	for(int i=0;i<numPointsInVBO;i++)
	{
		tva[i*3] =p->at(i).v[0];
		tva[i*3+1] =p->at(i).v[1];
		tva[i*3+2] =p->at(i).v[2];
	}
	
	if(!vbufferID)
		glGenBuffers(1,&vbufferID);
	glBindBuffer(GL_ARRAY_BUFFER,vbufferID);
	glBufferData(GL_ARRAY_BUFFER,3*sizeof(float)*numPointsInVBO,tva,GL_STATIC_DRAW);
	
	free(tva);
}

void GLWidget::paintGL()
{
    makeCurrent();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    set2DCamera();
	// Dibujamos todo el contenido
	float lightpos[3];

	lightpos[2] = 2.0;
	lightpos[0] = sin(timer.animationTimeValue*10.0)*2.0;
	lightpos[1] = cos(timer.animationTimeValue*10.0)*2.0;

	glLightfv(GL_LIGHT0,GL_POSITION,lightpos);       //updates the light's position

	glUniform1f(cubeSizeLoc,  1.0/(pow(3,fractalIterations)*2));

	glLoadIdentity();
	glTranslatef(0.0,0.0,-14.0);
	glRotatef(rotY,1.0,0.0,0.0);
	glRotatef(rotX,0.0,-1.0,0.0);
	glScalef(zoom,zoom,zoom);

	std::set<Voxel::Face> & faces = m_voxelSpace->triangulated();
	std::set<Voxel::Point> & points = m_voxelSpace->triangulationPoints();
	std::set<Voxel::Face>::iterator iter;
	std::set<Voxel::Face>::iterator end = faces.end();
	glBegin(GL_TRIANGLES);
	for(iter = faces.begin(); iter!=end;iter++)
	{
		const Voxel::Face &f = *iter;
		glColor3f(f[0][0]/80.0,f[0][1]/80.0,f[0][2]/80.0);
		glVertex3f(f[0][0],f[0][1],f[0][2]);
		glVertex3f(f[1][0],f[1][1],f[1][2]);
		glVertex3f(f[2][0],f[2][1],f[2][2]);
	}
	glEnd();
}

void GLWidget::set2DCamera()
{
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity ();
	glEnable(GL_DEPTH_TEST);
	gluPerspective (100.0, (GLfloat)widgetWidth/(GLfloat)widgetHeight, 0.1, 10000.0);
	glMatrixMode (GL_MODELVIEW);

}

void GLWidget::resizeGL(int width, int height)
{
    widgetWidth = width;
    widgetHeight = height;
    int side = qMin(width, height);
    glViewport(0, 0, width, height);

    set2DCamera();
}

void GLWidget::wheelEvent(QWheelEvent * event)
{
	zoom*=1+event->delta()/12000.0;
    updateGL();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::keyPressEvent ( QKeyEvent * event )
{
    updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

	lastPos = event->pos();
	rotX-=dx/2.0;
	rotY+=dy/2.0;

    updateGL();
}

void GLWidget::changeMorphSpeed(int newSpeed)
{
	morphSpeed = newSpeed / 100.0;
	updateGL();
}

void GLWidget::centerFractal()
{
	mustShowCube = !mustShowCube;
	updateGL();
}
void GLWidget::changeZoomValue(int newZoom)
{
	zoom = newZoom/100.0;
	updateGL();
}

void GLWidget::changeFractalIterations(int iters)
{
	if(iters == -1)
		fractalIterations = 0;
	else
		fractalIterations = iters;
	vector<Point> p;
	computeCube(&p, 0,0,0,1,fractalIterations);
	numPointsInVBO = p.size();
	generateVBO(&p);

	if(iters != -1)
		updateGL();
}

void GLWidget::setGammaValue(int gamma)
{
	updateGL();
}

void GLWidget::setFractal(QString fractal)
{
}

void GLWidget::setShowCube(int doShow)
{
	mustShowCube = doShow;
	updateGL();
}
