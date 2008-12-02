/***************************************
 *
 * Fractal Mountain Generator
 * Author: Evan Salazar
 * February 2008
 * 
 ***************************************/
#include "GeoFracGUI.h"
#include <QFileDialog>
GeoFracGUI::GeoFracGUI(QWidget *parent) : QMainWindow(parent)
{
	ui.setupUi(this);

	//Connect all the signals from the Dialog
	//connect(ui.actionImage,SIGNAL(activated()),ui.glWindow,SLOT(setWireFrameMode()));
}

//Scale Window
void GeoFracGUI::resizeEvent (QResizeEvent *) {	
	
}

GeoFracGUI::~GeoFracGUI()
{

}
