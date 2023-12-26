#pragma once

#include <QDomDocument>
#include <QDomNode>
#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include <QTreeView>
#include <QXmlStreamWriter>
#include "dommodel.h"
#include "domitem.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    virtual ~MainWindow();

public slots:
    void createComponents();
    void openFile();
    void expandChildren(const QModelIndex &index, QTreeView *view);

private:
    DomModel *model = new DomModel(QDomDocument(), this);
    QMenu *fileMenu;
    QString xmlPath;

    QTreeView *tree = new QTreeView(this);

    QMdiArea *m_mdiArea = new QMdiArea(this);
};
