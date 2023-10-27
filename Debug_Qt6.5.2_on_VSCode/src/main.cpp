#include "Newproject_By_QtConfigure.h"

#include <QApplication>
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Newproject_By_QtConfigure w;
    w.show();
    return a.exec();
}