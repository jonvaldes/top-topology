TEMPLATE = app
TARGET = TopTopology
QT += core \
    gui \
    opengl

QMAKE_CXXFLAGS += -O3

HEADERS += TopTopologyGUI.h \
	glwidget.h 
 
SOURCES += main.cpp \
	TopTopologyGUI.cpp \
	util/OBJparser.cpp \
	glutil/FreeCamera.cpp \
	geom/Point3D.cpp \
	geom/Vector3D.cpp \
	surface/Surface.cpp \
	surface/EdgeSet.cpp \
	glwidget.cpp 

FORMS           = TopTopology.ui
