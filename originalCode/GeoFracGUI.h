/***************************************
 *
 * Fractal Mountain Generator
 * Author: Evan Salazar
 * February 2008
 * 
 ***************************************/
#ifndef VRENDERERGUI_H
#define VRENDERERGUI_H

#include <QtGui/QMainWindow>
#include "ui_GeoFracGUI.h"
class GeoFracGUI: public QMainWindow
{
    Q_OBJECT

public:
    GeoFracGUI(QWidget *parent = 0);
    ~GeoFracGUI();

public slots:
    
private:
    Ui::GeoFracGUI ui;
    
protected:
	void resizeEvent ( QResizeEvent * event );


};

#endif // MOUNTAINS_H
