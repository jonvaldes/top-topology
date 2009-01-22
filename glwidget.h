#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTime>
#include <vector>
#include <memory>
#include "voxel/voxelSpace.h"
#include "voxel/spaceTriangulation.h"
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
		auto_ptr<voxel::VoxelSpace> m_voxelSpace;
		auto_ptr<voxel::SpaceTriangulation> m_triangulation;

		int lastButton;
		bool m_wireframe;
		glutil::FreeCamera * m_camera;
};

#endif /*GLWIDGET_H*/

