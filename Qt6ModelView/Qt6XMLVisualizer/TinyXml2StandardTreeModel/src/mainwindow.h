#pragma once

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include "tinyxmltreeview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void createComponents();
signals:

private:
    TinyXMLTreeView* m_treeView = new TinyXMLTreeView(this);
    QMdiArea* m_mdiArea = new QMdiArea(this);
};

