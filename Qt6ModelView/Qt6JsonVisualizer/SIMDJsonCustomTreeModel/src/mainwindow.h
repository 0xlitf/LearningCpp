#pragma once

#include <QMainWindow>
#include <QMdiArea>
#include <QMdiSubWindow>
#include "customtreeview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    void createComponents();

signals:

private:
    SIMDJsonTreeView* m_treeView = new SIMDJsonTreeView(this);

    QMdiArea* m_mdiArea = new QMdiArea(this);
};

