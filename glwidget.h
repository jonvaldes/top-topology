#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTime>
#include <vector>
#include "voxelSpace.h"

using namespace std;

struct glut_timer_t
{
	QTime current_time;
	QTime last_time;
	float animationTimeValue;
};

typedef struct
{
		float v[3];
}Point;

/** GLWidget*/
class GLWidget : public QGLWidget
{
    Q_OBJECT

    public:
        GLWidget(QWidget *parent = 0);
        ~GLWidget();

    public slots:
		void changeMorphSpeed(int newSpeed);
		void centerFractal();
		void changeZoomValue(int newZoom);
		void changeFractalIterations(int iters);
		void setGammaValue(int gamma);
		void setFractal(QString fractal);
		void setShowCube(int doShow);
    signals:
		void zoomValueChanged(int newZoom);

    protected:
        void initializeGL();
        void paintGL();
		void set2DCamera();
		void resizeGL(int width, int height);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent * event);
        void keyPressEvent ( QKeyEvent * event );
		void timerEvent(QTimerEvent * event);
		void computeCube(vector<Point> * v, float cx, float cy, float cz, double side, int iterations);
		void generateVBO(vector<Point>*p);

	
    private:
        int widgetWidth;
        int widgetHeight;
	
		GLfloat rotX;
		GLfloat rotY;
		QPoint lastPos;
		GLuint shaderProgram;
		GLint cubeSizeLoc;
		int mustShowCube;
		double zoom;
		float morphSpeed;
		glut_timer_t timer;
		int fractalIterations; 
		GLuint vbufferID;
		int numPointsInVBO;
		Voxel::VoxelSpace * m_voxelSpace;
};

#endif /*GLWIDGET_H*/

