#include "customtreeview.h"
#include <QItemDelegate>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QResource>
#include <QThread>
#include <QFile>

QJsonTreeView::QJsonTreeView(QWidget* parent)
    : QTreeView(parent) {

}

void QJsonTreeView::test() {

}

CustomItem* QJsonTreeView::parseJsonElement(const QJsonValue& value, CustomItemModel* model, CustomItem* parent, int depth) {
    CustomItem* rootItem = new CustomItem(parent);
    rootItem->setValue(0, "root");

    QJsonValue::Type type = value.type();

    switch (type) {
        case QJsonValue::Undefined: {
            rootItem->setValue(1, value.toVariant());
        } break;
        case QJsonValue::Bool: {
            rootItem->setValue(1, value.toBool());
        } break;
        case QJsonValue::Null: {
            rootItem->setValue(1, value.toVariant());
        } break;
        case QJsonValue::String: {
            rootItem->setValue(1, value.toString());
        } break;
        case QJsonValue::Double: {
            rootItem->setValue(1, value.toDouble());
            rootItem->setType(value.type());
        } break;
        case QJsonValue::Array: {
            int index = 0;
            for (auto v : value.toArray()) {
                CustomItem* child = parseJsonElement(v, model, rootItem, depth + 1);
                child->setValue(0, QString::number(index));
                child->setType(v.type());
                rootItem->addChild(child);
                ++index;
            }
        } break;

        case QJsonValue::Object: {
            for (QString key : value.toObject().keys()) {
                QJsonValue v = value.toObject().value(key);
                CustomItem* child = parseJsonElement(v, model, rootItem, depth + 1);
                child->setValue(0, key);
                child->setType(v.type());
                rootItem->addChild(child);
            }

        } break;
        default: {
            qDebug() << "default";
        } break;
    }

    return rootItem;
}

void QJsonTreeView::parseJsonFile(const char* jsonFile, CustomItemModel* model) {
    QFile file(jsonFile);
    QJsonDocument jdoc;
    if (file.open(QIODevice::ReadOnly)) {
        jdoc = QJsonDocument::fromJson(file.readAll());
        file.close();
        qDebug() << "Json parsing success.";
    } else {
        qDebug() << "Json parsing failed.";
    }

    CustomItem* rootItem;

    if (jdoc.isArray()) {
        qDebug() << "root isArray";
        auto arr = QJsonValue(jdoc.array());
        rootItem = parseJsonElement(arr, m_model);
        rootItem->setType(QJsonValue::Type::Array);
    } else if (jdoc.isObject()) {
        qDebug() << "root isObject";
        auto obj = QJsonValue(jdoc.object());
        rootItem = parseJsonElement(obj, m_model);
        rootItem->setType(QJsonValue::Type::Object);
    } else if (jdoc.isNull()) {
        qDebug() << "root isNull";
    }

    model->setRootItem(rootItem);
}

void QJsonTreeView::openJson() {
    if (m_model) {
        this->setModel(nullptr);
        delete m_model;
    }
    m_model = new CustomItemModel();
    const char* jsonFile = "C:/0xlitf/Qt6ModelView/Qt6JsonVisualizer/twitter_simple.json";

    qDebug() << "jsonFile: " << QUrl(jsonFile);

    parseJsonFile(jsonFile, m_model);

    this->setModel(m_model);

    this->expandAll();

    this->setEditTriggers(QTreeView::AllEditTriggers);
    this->setSelectionBehavior(QTreeView::SelectRows);
    this->setSelectionMode(QTreeView::SingleSelection);

    this->setColumnWidth(0, 350);
    this->setColumnWidth(1, 350);
}

void QJsonTreeView::writeJson()
{
    QFile writeFile("QJsonWriteFile.json");

    if (writeFile.open(QIODevice::Truncate | QIODeviceBase::WriteOnly)) {
        qDebug() << "writeFile open success";

        if (m_model->getRootItem()->itemType() == QJsonValue::Object) {
            QJsonObject jsonObject;

            writeDataToJsonObject(m_model->getRootItem(), jsonObject);

            QJsonDocument jsonDocument(jsonObject);
            QString jsonString = jsonDocument.toJson(QJsonDocument::Indented);
            qDebug() << jsonString;

            writeFile.write(jsonDocument.toJson(QJsonDocument::Indented));
            writeFile.close();
        } else if (m_model->getRootItem()->itemType() == QJsonValue::Array) {
            QJsonArray jsonArray;

            writeDataToJsonArray(m_model->getRootItem(), jsonArray);

            QJsonDocument jsonDocument(jsonArray);
            QString jsonString = jsonDocument.toJson(QJsonDocument::Indented);
            qDebug() << jsonString;

            writeFile.write(jsonDocument.toJson(QJsonDocument::Indented));
            writeFile.close();
        }
    } else {
        qDebug() << "writeFile open failed";
    }
}
