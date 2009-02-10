#ifndef _TOP_TOPOLOGY_GUI_H
#define _TOP_TOPOLOGY_GUI_H

#include <QtGui/QMainWindow>
#include "ui_TopTopology.h"

class TopTopologyGUI: public QMainWindow
{
    Q_OBJECT

public:
    TopTopologyGUI(QWidget *parent = 0);
    ~TopTopologyGUI();

private:
    Ui::TopTopology ui;
    
};

#endif 
