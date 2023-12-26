#include "mainwindow.h"
#include <QAction>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow{parent} {
    this->setCentralWidget(m_mdiArea);
    this->setMinimumSize(1300, 900);

    this->createComponents();
}

void MainWindow::createComponents() {
    QMdiSubWindow *subWindow = new QMdiSubWindow(m_mdiArea);
    subWindow->showMaximized();
    subWindow->setWidget(m_treeView);
    m_treeView->setWindowTitle("TreeView using CustomItemModel");

    connect(m_mdiArea, &QMdiArea::subWindowActivated, this, [=](QMdiSubWindow *window){
        qDebug() << "Activated windowTitle: " << window->windowTitle();
    });
}
