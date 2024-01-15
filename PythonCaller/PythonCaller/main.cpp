
#include "Python.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    Py_Initialize(); // 初始化

    PyRun_SimpleString("print('Hello python, i am cpp.')");

    Py_Finalize(); // 释放

    return app.exec();
}
