#include "TopTopologyGUI.h"

#include <QtGui>
#include <QApplication>
#include <QString>
#include <iostream>

#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv); 

    TopTopologyGUI w;
    w.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
}

