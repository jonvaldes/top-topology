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
		void openOBJFile();

    protected:
        void initializeGL();
        void paintGL();
		void setGLLight();
		void resizeGL(int width, int height);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent * event);
        void keyPressEvent ( QKeyEvent * event );

    private:
        int widgetWidth;
        int widgetHeight;
		float m_lightAngle;
	
		QPoint lastPos;

		surface::Surface m_surface;

		int lastButton;
		bool m_wireframe;
		glutil::FreeCamera * m_camera;
};

#endif 

