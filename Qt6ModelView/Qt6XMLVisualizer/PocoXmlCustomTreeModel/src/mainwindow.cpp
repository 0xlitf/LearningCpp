#include "mainwindow.h"
#include <QAction>
#include <QToolBar>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>

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
    connect(open, &QAction::triggered, this, [=]()
            { m_treeView->openXML(); });
    toolBar->addAction(open);

    QToolBar *toolBar2 = addToolBar(tr("Expand"));
    QAction *expand = new QAction(tr("&Expand"), this);
    connect(expand, &QAction::triggered, this, [this]
            { m_treeView->expandAll(); });

    toolBar2->addAction(expand);

    QAction* write = new QAction(tr("&Write"), this);
    connect(write, &QAction::triggered, this, [this] {
        m_treeView->writeFile();
    });
    toolBar2->addAction(write);

    this->setCentralWidget(m_treeView);
    this->setWindowTitle("XML");

    connect(m_mdiArea, &QMdiArea::subWindowActivated, this, [=](QMdiSubWindow *window){
        if (window) {
            qDebug() << "Activated windowTitle: " << window->windowTitle();
        }
    });


    m_treeView->openXML();
    m_treeView->test();
//    m_treeView->writeFile();
}
