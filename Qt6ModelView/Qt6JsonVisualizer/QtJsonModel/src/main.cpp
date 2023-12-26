#include "MainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle("fusion");


    MainWindow mainWindow;

    QFile file("://theme/dark_teal.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream stream(&file);
        mainWindow.setStyleSheet(stream.readAll());
        file.close();
    } else {
        qDebug() << "qss can not open";
    }

    mainWindow.show();
    return a.exec();
}
