
#include <application.h>

int main(int argc, char *argv[]) {
    Application app(argc, argv);

    qDebug() << "Application";

    return app.exec();
}
