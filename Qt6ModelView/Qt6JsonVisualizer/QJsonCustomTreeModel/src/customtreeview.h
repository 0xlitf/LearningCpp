#pragma once

#include <QStandardItemModel>
#include <QTreeView>
#include <iostream>
#include <string>
#include <set>
#include <exception>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include "customitem.h"
#include "customitemmodel.h"

class QJsonTreeView : public QTreeView {
public:
    QJsonTreeView(QWidget* parent = nullptr);

    void test();

    void openJson();

    void writeJson();

    void writeDataToJsonObject(const CustomItem* rootItem, QJsonObject& parentObject) {
        QHash<int, CustomItem*> childrenItems = rootItem->childrenItems();
        for (int i = 0; i < childrenItems.count(); i++) {

            CustomItem* item = childrenItems[i];

            switch (item->itemType()) {
                case QJsonValue::Undefined: {
                    // qDebug() << "Undefined";
                } break;
                case QJsonValue::Bool: {
                    // qDebug() << "Bool";
                    parentObject.insert(item->value(0).toString(), QJsonValue(item->value(1).toBool()));
                } break;
                case QJsonValue::Null: {
                    // qDebug() << "Null";
                    parentObject.insert(item->value(0).toString(), QJsonValue());
                } break;
                case QJsonValue::String: {
                    // qDebug() << "String";
                    parentObject.insert(item->value(0).toString(), QJsonValue(item->value(1).toString()));
                } break;
                case QJsonValue::Double: {
                    // qDebug() << "Double";
                    parentObject.insert(item->value(0).toString(), item->value(1).toJsonValue());
                } break;
                case QJsonValue::Array: {
                    // qDebug() << "Array";
                    QJsonArray jsonArray;
                    writeDataToJsonArray(item, jsonArray);

                    parentObject[item->value(0).toString()] = jsonArray;
                } break;
                case QJsonValue::Object: {
                    // qDebug() << "Object";
                    QJsonObject obj;
                    // qDebug() << item->value(0);
                    writeDataToJsonObject(item, obj);

                    parentObject[item->value(0).toString()] = obj;
                } break;
                default: {
                    // qDebug() << "default";
                } break;
            }
        }
    }

    void writeDataToJsonArray(const CustomItem* rootItem, QJsonArray& parentArray) {
        QHash<int, CustomItem*> childrenItems = rootItem->childrenItems();
        for (int i = 0; i < childrenItems.count(); i++) {

            CustomItem* item = childrenItems[i];

            switch (item->itemType()) {
                case QJsonValue::Undefined: {
                    // qDebug() << "Undefined";
                } break;
                case QJsonValue::Bool: {
                    // qDebug() << "Bool";
                    parentArray.insert(i, QJsonValue(item->value(1).toBool()));
                } break;
                case QJsonValue::Null: {
                    // qDebug() << "Null";
                    parentArray.insert(i, QJsonValue());
                } break;
                case QJsonValue::String: {
                    // qDebug() << "String";
                    parentArray.insert(i, QJsonValue(item->value(1).toString()));
                } break;
                case QJsonValue::Double: {
                    // qDebug() << "Double";
                    parentArray.insert(i, item->value(1).toJsonValue());
                } break;
                case QJsonValue::Array: {
                    // qDebug() << "Array";
                    QJsonArray jsonArray;
                    parentArray.append(jsonArray);
                } break;
                case QJsonValue::Object: {
                    // qDebug() << "Object";
                    QJsonObject obj;
                    // qDebug() << item->value(0);
                    writeDataToJsonObject(item, obj);

                    parentArray.append(obj);
                } break;
                default: {
                    // qDebug() << "default";
                } break;
            }
        }
    }

    void parseJsonFile(const char* jsonFile, CustomItemModel* CustomModel);

    CustomItem* parseJsonElement(const QJsonValue& obj, CustomItemModel* model, CustomItem* parentItem = nullptr, int depth = 0);

private:
    CustomItemModel* m_model = nullptr;
};
