#define USING_WIDGET // USING_WIDGET USING_QML TEST

#ifdef TEST

#include <QApplication>
#include <QtWidgets>
#include <QXmlStreamWriter>
#include <QFile>

// �ݹ麯�������ڱ�����д������ģ�����ݵ� XML
void writeModelDataToXML(const QAbstractItemModel* model, const QModelIndex& parentIndex, QXmlStreamWriter& xmlWriter) {
    int rowCount = model->rowCount(parentIndex);
    qDebug() << "rowCount: " << rowCount;
    for (int row = 0; row < rowCount; ++row) {
        QModelIndex index = model->index(row, 0, parentIndex);
        QString itemName = index.data(Qt::DisplayRole).toString();
        QString itemAttribute = index.data(Qt::UserRole).toString();  // �����������ݴ洢�� UserRole ��

        xmlWriter.writeStartElement("Item");
        xmlWriter.writeAttribute("Name", itemName);
        xmlWriter.writeAttribute("Attribute", itemAttribute);

        // �ݹ鴦������
        writeModelDataToXML(model, index, xmlWriter);

        xmlWriter.writeEndElement(); // �ر� Item Ԫ��
    }
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // ����һ���Զ���ģ��
    QStandardItemModel model;

    // ��ģ���������Ŀ��������
    QStandardItem* rootItem = new QStandardItem("Root Item");
    rootItem->setData("Root Attribute", Qt::UserRole);
    model.appendRow(rootItem);

    QStandardItem* childItem = new QStandardItem("Child Item");
    childItem->setData("Child Attribute", Qt::UserRole);
    rootItem->appendRow(childItem);

    // ���� XML �ļ�
    QFile file("treeview_data.xml");
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        // д���Ԫ��
        xmlWriter.writeStartElement("TreeViewData");

        // �ݹ������д��ģ������
        writeModelDataToXML(&model, QModelIndex(), xmlWriter);

        // д���Ԫ�صĽ�����ǩ
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
