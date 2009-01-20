#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTime>
#include <vector>
#include "voxel/voxelSpace.h"
#include "glutil/FreeCamera.h"

using namespace std;

/** GLWidget*/
class GLWidget : public QGLWidget
{
    Q_OBJECT

    public:
        GLWidget(QWidget *parent = 0);
        ~GLWidget();

    public slots:
		void setWireframe(bool showWire);
		void setShowNonManifold(bool showEdges);

    protected:
        void initializeGL();
        void paintGL();
		void resizeGL(int width, int height);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent * event);
        void keyPressEvent ( QKeyEvent * event );

	
    private:
        int widgetWidth;
        int widgetHeight;
	
		QPoint lastPos;
		Voxel::VoxelSpace * m_voxelSpace;
		int lastButton;
		bool m_wireframe;
		bool m_showNonManifold;
		glutil::FreeCamera * m_camera;
};

#endif /*GLWIDGET_H*/

