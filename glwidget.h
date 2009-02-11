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
	enum keys{KEY_W=0,KEY_A,KEY_S,KEY_D};

    public:
        GLWidget(QWidget *parent = 0);
        ~GLWidget();

    public slots:
		void setWireframe(bool showWire);
		void setShowFaces(bool showFaces);
		void setLightAngle(int angle);
		void setMergeSpeed(int speed);
		void resetCamera();
		void resetSurface();
		void openOBJFile();
		void mustSpherify(bool must);
	signals:
		void surfacePoints(QString);
		void surfaceFaces(QString);
		void surfaceEdges(QString);

    protected:
		void queueUpdate();
        void initializeGL();
        void paintGL();
		void setGLLight();
		void resizeGL(int width, int height);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent * event);
        void keyPressEvent ( QKeyEvent * event );
        void keyReleaseEvent ( QKeyEvent * event );
		void focusOutEvent ( QFocusEvent * event );
		void timerEvent( QTimerEvent *e );


    private:
        int widgetWidth;
        int widgetHeight;
		float m_lightAngle;
	
		QPoint lastPos;

		surface::Surface m_surface;
		surface::Surface m_backupSurface;

		float m_mergeTime;
		float m_mergeSpeed;
		int m_timer;

		int lastButton;
		bool m_wireframe;
		bool m_showFaces;
		bool m_keysStatus[4];
		bool m_mustSpherify;
		glutil::FreeCamera * m_camera;
};

#endif 

