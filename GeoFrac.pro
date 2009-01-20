TEMPLATE = app
TARGET = GeoFrac 
QT += core \
    gui \
    opengl

LIBS += -lfl

QMAKE_CXXFLAGS += -O3

HEADERS += GeoFracGUI.h \
		GLee.h \
		shaderUtils.h \
		glwidget.h 
 
SOURCES += main.cpp \
	GLee.c \
	shaderUtils.cpp \
	GeoFracGUI.cpp \
	voxelSpace.cpp \
	glutil/FreeCamera.cpp \
	geom/Point3D.cpp \
	geom/Vector3D.cpp \
	glwidget.cpp 

FORMS           = GeoFracGUI.ui
