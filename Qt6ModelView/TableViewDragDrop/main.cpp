
#include <QHeaderView>
#include <QApplication>
#include <QTableView>
#include <QStandardItemModel>
#include <QDrag>
#include <QMimeData>
#include <QDataStream>
#include <QMouseEvent>

class InternalDragDropTableView : public QTableView {
public:
    InternalDragDropTableView(QWidget* parent = nullptr) : QTableView(parent) {
        setAcceptDrops(true);
        setDragEnabled(true);

        // 创建模型和填充数据
        model = new QStandardItemModel(4, 4, this);
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                model->setItem(row, col, new QStandardItem(QString("Row %1, Col %2").arg(row).arg(col)));
            }
        }
        setModel(model);

        // 隐藏行头和列表头
        horizontalHeader()->hide();
        verticalHeader()->hide();

        // 设置表格可编辑
        setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);
    }

protected:
    void mousePressEvent(QMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            QModelIndex index = indexAt(event->pos());
            if (index.isValid()) {
                QByteArray itemData;
                QDataStream dataStream(&itemData, QIODevice::WriteOnly);
                dataStream << index.row(); // 存储源行索引
                dataStream << index.column(); // 存储源列索引

                QMimeData* mimeData = new QMimeData;
                mimeData->setData("application/x-qabstractitemmodeldatalist", itemData);

                QDrag* drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->exec(Qt::CopyAction);
            }
        }

        QTableView::mousePressEvent(event);
    }

    void dragEnterEvent(QDragEnterEvent* event) override {
        if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
            event->acceptProposedAction();
        }
    }

    void dragMoveEvent(QDragMoveEvent* event) override {
        if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
            event->acceptProposedAction();
        }
    }

    void dropEvent(QDropEvent* event) override {
        if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist")) {
            QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
            QDataStream dataStream(&itemData, QIODevice::ReadOnly);
            int sourceRow, sourceCol;

            dataStream >> sourceRow; // 从拖拽数据中获取源行索引
            dataStream >> sourceCol; // 从拖拽数据中获取源列索引

            // 获取目标索引
            QModelIndex targetIndex = indexAt(event->pos());

            // 移动数据到目标位置
            if (sourceRow >= 0 && sourceCol >= 0 && targetIndex.isValid()) {
                QModelIndex sourceIndex = model->index(sourceRow, sourceCol);
                QString sourceData = model->data(sourceIndex, Qt::DisplayRole).toString();
                model->setData(sourceIndex, model->data(targetIndex, Qt::DisplayRole));
                model->setData(targetIndex, sourceData);
                event->acceptProposedAction();
            }
        }
    }

private:
    QStandardItemModel* model;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    InternalDragDropTableView tableView;
    tableView.setWindowTitle("Internal Drag and Drop Example");
    tableView.setGeometry(100, 100, 400, 300);
    tableView.show();

    return app.exec();
}
