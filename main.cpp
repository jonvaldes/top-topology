#include "GeoFracGUI.h"

#include <QtGui>
#include <QApplication>
#include <QString>
#include <iostream>

#include <QTextStream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv); 
    QTranslator translator;
    translator.load("vrg_es");
    app.installTranslator(&translator);

     QString locale = QLocale::system().name();
     if(!locale.contains("es_ES"))
     {
         app.removeTranslator(&translator);
         qDebug()<< "Donnit"; 
     }
     qDebug() <<QString("Current locale: ")+ locale;

    GeoFracGUI w;
    w.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
}
