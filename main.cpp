#include <QtCore/QCoreApplication>
#include <QtWidgets/QApplication>

#include "SimplePlayer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    SimplePlayer mainWindow;
    mainWindow.show();

    return app.exec();
}
