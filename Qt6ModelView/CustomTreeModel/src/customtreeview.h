#pragma once

#include <QTreeView>
#include <QHeaderView>
#include <QStandardItemModel>
#include "tinyxml2.h"
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QRandomGenerator>
#include <random>

struct ChildItem {
    QString name{};
    int value_1{};
    int value_2{};
    int value_3{};
};

struct ParentItem {
    QString name{};
    QVector<ChildItem*> childItem;
};

class TreeItem
{
public:
    explicit TreeItem(TreeItem *parentItem = 0);
    ~TreeItem();

    void appendChild(TreeItem *child); // 在本节点下增加子节点
    void removeChilds();               // 清空所有节点

    TreeItem *child(int row); // 获取第row个子节点指针
    TreeItem *parentItem();   // 获取父节点指针
    int childCount() const;   // 子节点计数
    int row() const;          // 获取该节点是父节点的第几个子节点

    // 核心函数：获取节点第column列的数据
    QVariant data(int column) const;

    // 设置、获取节点是几级节点（就是树的层级）
    int level() { return mLevel; }
    void setLevel(int level) { mLevel = level; }

    // 设置、获取节点存的数据指针
    void setPtr(void *p) { mPtr = p; }
    void *ptr() { return mPtr; }

    // 保存该节点是其父节点的第几个子节点，查询优化所用
    void setRow(int row)
    {
        mRow = row;
    }

private:
    QList<TreeItem *> mChildItems; // 子节点
    TreeItem *mParentItem;         // 父节点
    int mLevel;                    // 该节点是第几级节点
    void *mPtr;                    // 存储数据的指针
    int mRow;                      // 记录该item是第几个，可优化查询效率
};

class TreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    explicit TreeModel(QStringList headers, QObject *parent = 0);
    ~TreeModel();

    // 以下为自定义model需要实现的一些虚函数，将会被Qt在查询model数据时调用
    // headerData: 获取表头第section列的数据
    // data: 核心函数，获取某个索引index的元素的各种数据
    //       role决定获取哪种数据，常用有下面几种：
    //       DisplayRole（默认）：就是界面显示的文本数据
    //       TextAlignmentRole：就是元素的文本对齐属性
    //       TextColorRole、BackgroundRole：分别指文本颜色、单元格背景色
    // flags: 获取index的一些标志，一般不怎么改
    // index: Qt向你的model请求一个索引为parent的节点下面的row行column列子节点的元素，在本函数里你需要返回该元素的正确索引
    // parent：获取指定元素的父元素
    // rowCount: 获取指定元素的子节点个数（下一级行数）
    // columnCount: 获取指定元素的列数
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

public:
    TreeItem *itemFromIndex(const QModelIndex &index) const;
    TreeItem *root();

private:
    QStringList mHeaders; // 表头内容
    TreeItem *mRootItem;  // 根节点
};

class CustomTreeView : public QTreeView
{
public:
    CustomTreeView(QWidget *parent = nullptr);

    int nClass = 10;
    int nStudent = 10000;
    QVector<ParentItem *> mClasses;
};
