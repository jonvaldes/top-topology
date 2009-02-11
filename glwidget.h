#ifndef __GLWIDGET_H__
#define __GLWIDGET_H__

#include <QGLWidget>
#include <QTime>
#include <vector>
#include <memory>
#include "surface/Surface.h"
#include "glutil/FreeCamera.h"

using namespace std;

class GLWidget : public QGLWidget
{
    Q_OBJECT

    public:
        GLWidget(QWidget *parent = 0);
        ~GLWidget();

    public slots:
		void setWireframe(bool showWire);
		void setLightAngle(int angle);
		void setMergeSpeed(int speed);
		void openOBJFile();
	signals:
		void surfacePoints(QString);
		void surfaceFaces(QString);
		void surfaceEdges(QString);

    protected:
        void initializeGL();
        void paintGL();
		void setGLLight();
		void resizeGL(int width, int height);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent * event);
        void keyPressEvent ( QKeyEvent * event );
		void timerEvent( QTimerEvent *e );


    private:
        int widgetWidth;
        int widgetHeight;
		float m_lightAngle;
	
		QPoint lastPos;

		surface::Surface m_surface;
		float m_mergeTime;
		float m_mergeSpeed;
		int m_timer;

		int lastButton;
		bool m_wireframe;
		glutil::FreeCamera * m_camera;
};

#endif 

