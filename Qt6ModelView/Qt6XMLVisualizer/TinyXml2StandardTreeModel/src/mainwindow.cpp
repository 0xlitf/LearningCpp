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
    QToolBar *toolBar = addToolBar(tr("Open"));
    QAction *open = new QAction(tr("&Open"), this);
    open->setShortcuts(QKeySequence::Open);
    connect(open, &QAction::triggered, this, [=]() {
        m_treeView->openXML();
    });
    toolBar->addAction(open);

    QAction *expand = new QAction(tr("&Expand"), this);
    expand->setShortcuts(QKeySequence::Open);
    connect(expand, &QAction::triggered, this, [this] {
        m_treeView->expandAll();
    });

    toolBar->addAction(expand);

    // set parent or will be stuck when drag the window
    QMdiSubWindow *subWindow = new QMdiSubWindow(m_mdiArea);
    subWindow->showMaximized();
    subWindow->setWidget(m_treeView);
    m_treeView->setWindowTitle("TinyXML TreeView using QStandardItemModel");
    m_treeView->openXML();
    m_treeView->expandAll();

    connect(m_mdiArea, &QMdiArea::subWindowActivated, this, [=](QMdiSubWindow *window){
        qDebug() << "Activated windowTitle: " << window->windowTitle();
    });
}
