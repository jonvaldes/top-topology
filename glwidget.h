#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QTime>
#include <vector>
#include "voxelSpace.h"
#include "glutil/FreeCamera.h"

using namespace std;

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
		void setWireframe(bool showWire);
		void setShowNonManifold(bool showEdges);

    protected:
        void initializeGL();
        void paintGL();
		void set2DCamera();
		void resizeGL(int width, int height);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void wheelEvent(QWheelEvent * event);
        void keyPressEvent ( QKeyEvent * event );

	
    private:
        int widgetWidth;
        int widgetHeight;
	
		GLfloat rotX;
		GLfloat rotY;
		QPoint lastPos;
		Point camPos;
		double zoom;
		Voxel::VoxelSpace * m_voxelSpace;
		int lastButton;
		bool m_wireframe;
		bool m_showNonManifold;
		//glutil::ShaderProgram * m_surfaceShader;
		glutil::FreeCamera * m_camera;
};

#endif /*GLWIDGET_H*/

