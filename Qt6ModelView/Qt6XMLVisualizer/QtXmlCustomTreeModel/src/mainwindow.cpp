#include "mainwindow.h"
#include <QAction>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardItemModel>
#include <QToolBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow{ parent } {
    this->setCentralWidget(m_mdiArea);
    this->setMinimumSize(1300, 900);

    this->createComponents();
}

MainWindow::~MainWindow() {
    if (model) {
        delete model;
        model = nullptr;
    }
}

void MainWindow::createComponents() {
    QToolBar* fileToolBar = addToolBar(tr("Open"));
    QAction* open = new QAction(tr("&Open"), this);
    open->setShortcuts(QKeySequence::Open);
    connect(open, &QAction::triggered, this, [this] {
        this->openFile();
        });
    fileToolBar->addAction(open);

    QAction* expand = new QAction(tr("&Expand"), this);
    expand->setShortcuts(QKeySequence::Open);
    connect(expand, &QAction::triggered, this, [this] {
        tree->expandAll();
        });
    fileToolBar->addAction(expand);

    QAction* write = new QAction(tr("&Write"), this);
    connect(write, &QAction::triggered, this, [this] {

        });
    fileToolBar->addAction(write);

    tree->setModel(model);

    QMdiSubWindow* subWindow = new QMdiSubWindow(m_mdiArea);
    subWindow->showMaximized();
    subWindow->setWidget(tree);
    tree->setWindowTitle("TreeView using CustomItemModel");

    connect(m_mdiArea, &QMdiArea::subWindowActivated, this, [=](QMdiSubWindow* window) {
        qDebug() << "Activated windowTitle: " << window->windowTitle();
        });
}

void MainWindow::openFile() {
    QString filePath;
    if (true) {
        filePath = "treeview_data - Copy.xml"; // description_simple.xml  // description.xml treeview_data - Copy.xml
        if (!QFile(filePath).exists()) {
            QDir currentDir = QDir::current();
            QString currentPath = currentDir.absolutePath();
            qDebug() << "Current Path: " << currentPath;
            qDebug() << "filePath not exist: " << filePath;
        }
    } else {
        QString filePath = QFileDialog::getOpenFileName(this,
            tr("Open File"),
            xmlPath,
            tr("XML files (*.xml);;HTML files (*.html);;"
            "SVG files (*.svg);;User Interface files (*.ui)"));
    }
    if (!filePath.isEmpty()) {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly)) {
            QDomDocument document;
            if (document.setContent(&file)) {
                DomModel* newModel = new DomModel(document, this);
                tree->setModel(newModel);
                delete model;
                model = newModel;
                xmlPath = filePath;
            }
            file.close();
        }

        filePath = "";
    }

    tree->expandAll();
}

void MainWindow::expandChildren(const QModelIndex& index, QTreeView* view) {
    if (!index.isValid()) {
        return;
    }

    int childItem = index.model()->rowCount(index);
    for (int i = 0; i < childItem; i++) {
        const QModelIndex& child = index.sibling(i, 0);
        expandChildren(child, view);
    }

    if (!view->isExpanded(index)) {
        view->expand(index);
    }
}
