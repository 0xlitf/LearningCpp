#pragma once

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include "customtreeview.h"
#include <QXmlStreamWriter>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void createComponents();

signals:

private:
    PocoXmlTreeViewCustom* m_treeView = new PocoXmlTreeViewCustom(this);

    QMdiArea* m_mdiArea = new QMdiArea(this);
};

