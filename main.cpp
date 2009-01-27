#include "GeoFracGUI.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    GeoFracGUI w;
    w.show();
    app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
    return app.exec();
}

// new comment
//  fhg fgfhgfh fdskjh sdkhfksdkghd hk
// anothr comments
// A SUICIDE
// sfsdksdhkfjksdhf
//sd fsdjh fjhsdhf sdhkfhj
