TEMPLATE = app
TARGET = GeoFrac 
QT += core \
    gui \
    opengl

LIBS += -lfl

QMAKE_CXXFLAGS += -O3

HEADERS += GeoFracGUI.h \
		glwidget.h 
 
SOURCES += main.cpp \
	GeoFracGUI.cpp \
	voxel/voxelSpace.cpp \
	glutil/FreeCamera.cpp \
	geom/Point3D.cpp \
	geom/Vector3D.cpp \
	glwidget.cpp 

FORMS           = GeoFracGUI.ui
