#include "customtreeview.h"
#include <QRandomGenerator>

CustomTreeView::CustomTreeView(QWidget *parent)
    : QTreeView(parent) {
    for (int i = 0; i < nClass; i++) {
        ParentItem *c = new ParentItem;
        c->name = QString("Parent %1").arg(i);
        for (int j = 0; j < nStudent; j++) {
            ChildItem *s = new ChildItem;
            s->name = QString("name %1").arg(j);
            s->value_1 = QRandomGenerator::global()->bounded(0, 100);
            s->value_2 = QRandomGenerator::global()->bounded(0, 100);
            s->value_3 = QRandomGenerator::global()->bounded(0, 100);
            c->childItem.append(s);
        }
        mClasses.append(c);
    }

    this->setEditTriggers(QTreeView::NoEditTriggers);
    this->setSelectionBehavior(QTreeView::SelectRows);
    this->setSelectionMode(QTreeView::SingleSelection);
    this->setAlternatingRowColors(true);
    this->setFocusPolicy(Qt::NoFocus);

    this->header()->setHighlightSections(true);
    this->header()->setDefaultAlignment(Qt::AlignCenter);
    this->header()->setDefaultSectionSize(100);
    this->header()->setStretchLastSection(true);
    this->header()->setSortIndicator(0, Qt::AscendingOrder);

    QStandardItemModel *model = new QStandardItemModel(this);
    model->setHorizontalHeaderLabels({"1", "2", "3", "4", "5", "6", "7", "8"});

    foreach (ParentItem *c, mClasses) {
        QStandardItem *itemClass = new QStandardItem(c->name);
        model->appendRow(itemClass);

        foreach (ChildItem *s, c->childItem) {
            int value_1 = s->value_1;
            int score2 = s->value_2;
            int value_3 = s->value_3;
            int total = value_1 + score2 + value_3;
            int average = total / 3;
            bool y = true;
            if (value_1 < 60 || score2 < 60 || value_3 < 60) {
                y = false;
            }
            bool z = false;
            if (value_1 >= 90 && score2 >= 90 && value_3 >= 90) {
                z = true;
            }

            itemClass->appendRow({new QStandardItem(s->name),
                                  new QStandardItem(QString::number(value_1)),
                                  new QStandardItem(QString::number(score2)),
                                  new QStandardItem(QString::number(value_3)),
                                  new QStandardItem(QString::number(total)),
                                  new QStandardItem(QString::number(average)),
                                  new QStandardItem(y ? "Yes" : "no"),
                                  new QStandardItem(z ? "*" : "-")});
        }
    }

    this->setModel(model);
}
