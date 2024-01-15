
#include "Python.h"
#include "PythonCaller.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QString appDirPath = QCoreApplication::applicationDirPath();
    qDebug() << "应用程序目录：" << appDirPath;

    Py_Initialize(); // 初始化
    int result = PyRun_SimpleString("print('Hello, World!')");

    if (result != 0) {
        PyErr_Print();
    }

    Py_Finalize(); // 释放

    PythonCaller cp("PyFunc");

    cp.setArgc(2);
    for (int i = 0; i < 2; ++i) {
        cp.feedArgv(i, i + 5);
    }

    cp.executePythonFunction("func");

    return app.exec();
}
