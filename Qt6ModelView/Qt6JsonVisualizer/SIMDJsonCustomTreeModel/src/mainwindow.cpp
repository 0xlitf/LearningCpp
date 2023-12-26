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
            { m_treeView->openJson(); });
    toolBar->addAction(open);

    // Expand ALL Button Implementation

    QAction *expand = new QAction(tr("&Expand"), this);
    expand->setShortcuts(QKeySequence::Open);
    connect(expand, &QAction::triggered, this, [this]
            { m_treeView->expandAll(); });

    toolBar->addAction(expand);

    QAction* write = new QAction(tr("&Write"), this);
    connect(write, &QAction::triggered, this, [this] {
        qDebug() << "writeJson";
        m_treeView->writeJson();
    });
    toolBar->addAction(write);

    this->setCentralWidget(m_treeView);
    this->setWindowTitle("Json");

    connect(m_mdiArea, &QMdiArea::subWindowActivated, this, [=](QMdiSubWindow *window){
        if (window) {
            qDebug() << "Activated windowTitle: " << window->windowTitle();
        }
    });


    m_treeView->openJson();
    m_treeView->test();
    m_treeView->writeJson();
}
