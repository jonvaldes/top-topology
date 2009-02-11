#include "GeoFracGUI.h"
#include <QFileDialog>
GeoFracGUI::GeoFracGUI(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);
}

//Scale Window
void GeoFracGUI::resizeEvent (QResizeEvent *) {	
	
}

GeoFracGUI::~GeoFracGUI()
{

}
