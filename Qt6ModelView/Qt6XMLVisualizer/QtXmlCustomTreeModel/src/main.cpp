#define USING_WIDGET // USING_WIDGET USING_QML TEST

#ifdef TEST

#include <QApplication>
#include <QtWidgets>
#include <QXmlStreamWriter>
#include <QFile>

// 递归函数，用于遍历并写入树形模型数据到 XML
void writeModelDataToXML(const QAbstractItemModel* model, const QModelIndex& parentIndex, QXmlStreamWriter& xmlWriter) {
    int rowCount = model->rowCount(parentIndex);
    qDebug() << "rowCount: " << rowCount;
    for (int row = 0; row < rowCount; ++row) {
        QModelIndex index = model->index(row, 0, parentIndex);
        QString itemName = index.data(Qt::DisplayRole).toString();
        QString itemAttribute = index.data(Qt::UserRole).toString();  // 假设属性数据存储在 UserRole 中

        xmlWriter.writeStartElement("Item");
        xmlWriter.writeAttribute("Name", itemName);
        xmlWriter.writeAttribute("Attribute", itemAttribute);

        // 递归处理子项
        writeModelDataToXML(model, index, xmlWriter);

        xmlWriter.writeEndElement(); // 关闭 Item 元素
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // 创建一个自定义模型
    QStandardItemModel model;

    // 在模型中添加项目及其属性
    QStandardItem* rootItem = new QStandardItem("Root Item");
    rootItem->setData("Root Attribute", Qt::UserRole);
    model.appendRow(rootItem);

    QStandardItem* childItem = new QStandardItem("Child Item");
    childItem->setData("Child Attribute", Qt::UserRole);
    rootItem->appendRow(childItem);

    // 创建 XML 文件
    QFile file("treeview_data.xml");
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        // 写入根元素
        xmlWriter.writeStartElement("TreeViewData");

        // 递归遍历并写入模型数据
        writeModelDataToXML(&model, QModelIndex(), xmlWriter);

        // 写入根元素的结束标签
        xmlWriter.writeEndElement();

        xmlWriter.writeEndDocument();
        file.close();
    } else {
        qDebug() << "Failed to open the XML file for writing.";
    }

    return app.exec();
}

#endif // TEST


#ifdef USING_WIDGET

#include "mainwindow.h"
#include "tinyxml2.h"
#include <QApplication>
#include <QDebug>
#include <QQmlContext>
#include <QStandardItemModel>
#include <QSysInfo>
#include <QTreeView>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineView>
#include <iostream>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    MainWindow m;
    m.show();

    //    QWebEngineView view;
    //    view.setMinimumSize(1366, 900);

    //    auto settings = view.page()->settings();

    //    settings->setAttribute(QWebEngineSettings::ShowScrollBars, true);
    //    settings->setAttribute(QWebEngineSettings::WebGLEnabled, true);
    //    settings->setAttribute(QWebEngineSettings::ScrollAnimatorEnabled, true);

    //    view.load(QUrl("https://verytoolz.com/xml-parser.html"));

    //    QObject::connect(&view, &QWebEngineView::loadFinished, [&]() {
    //        qDebug() << "QWebEngineView::loadFinished";
    //    });

    //    view.show();

    return app.exec();
}

#endif


#ifdef USING_QML


#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char* argv[]) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() {
            QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load("qrc:/qml/Main.qml");

    return app.exec();
}

#endif
