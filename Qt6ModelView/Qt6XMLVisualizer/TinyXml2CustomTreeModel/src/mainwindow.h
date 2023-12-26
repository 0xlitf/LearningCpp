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
    TinyXMLTreeViewCustom* m_treeView = new TinyXMLTreeViewCustom(this);

    QMdiArea* m_mdiArea = new QMdiArea(this);
};

