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
    RapidJsonTreeView* m_treeView = new RapidJsonTreeView(this);

    QMdiArea* m_mdiArea = new QMdiArea(this);
};

