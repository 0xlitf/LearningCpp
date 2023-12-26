#pragma once

#include <QStandardItemModel>
#include <QTreeView>
#include <iostream>
#include <string>
#include <cstring>
#include <set>
#include <exception>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
#include "customitem.h"
#include "customitemmodel.h"

class RapidJsonTreeView : public QTreeView {
public:
    RapidJsonTreeView(QWidget* parent = nullptr);

    void test();

    void openJson();

    void writeJson();

    void writeDataToJsonObject(const CustomItem* rootItem, rapidjson::Value& parentObject, rapidjson::Document& document) {
        QHash<int, CustomItem*> childrenItems = rootItem->childrenItems();
        for (int i = 0; i < childrenItems.count(); i++) {
            CustomItem* item = childrenItems[i];

            switch (item->itemType()) {
                case rapidjson::Type::kFalseType: {
                    // qDebug() << "kFalseType";
                    rapidjson::Value element;
                    element.SetBool(item->value(1).toBool());
                    parentObject.AddMember(rapidjson::Value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
                } break;
                case rapidjson::Type::kTrueType: {
                    // qDebug() << "kTrueType";
                    rapidjson::Value element;
                    element.SetBool(item->value(1).toBool());
                    parentObject.AddMember(rapidjson::Value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
                } break;
                case rapidjson::Type::kNullType: {
                    // qDebug() << "Null";
                    rapidjson::Value element;
                    element.SetNull();
                    parentObject.AddMember(rapidjson::Value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
                } break;
                case rapidjson::Type::kStringType: {
                    // qDebug() << "String";
                    rapidjson::Value element;
                    element.SetString(item->value(1).toString().toStdString().c_str(), document.GetAllocator());
                    parentObject.AddMember(rapidjson::Value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
                } break;
                case rapidjson::Type::kNumberType: {
                    // qDebug() << "Double";
                    rapidjson::Value element;
                    switch (item->numberType()) {
                        case CustomItem::kDoubleFlag: {
                            element.SetDouble(item->value(1).toDouble());
                        } break;
                        case CustomItem::kIntFlag: {
                            element.SetInt(item->value(1).toInt());
                        } break;
                        case CustomItem::kUintFlag: {
                            element.SetUint(item->value(1).toUInt());
                        } break;
                        case CustomItem::kInt64Flag: {
                            element.SetInt64(item->value(1).toLongLong());
                        } break;
                        case CustomItem::kUint64Flag: {
                            element.SetUint64(item->value(1).toULongLong());
                        } break;
                        default: {
                        }
                    }
                    parentObject.AddMember(rapidjson::Value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
                } break;
                case rapidjson::Type::kArrayType: {
                    // qDebug() << "Array";
                    rapidjson::Value element;
                    element.SetArray();
                    writeDataToJsonArray(item, element, document);

                    parentObject.AddMember(rapidjson::Value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
                } break;
                case rapidjson::Type::kObjectType: {
                    // qDebug() << "Object";
                    rapidjson::Value element;
                    element.SetObject();

                    writeDataToJsonObject(item, element, document);

                    parentObject.AddMember(rapidjson::Value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
                } break;
                default: {
                    // qDebug() << "default";
                } break;
            }
        }
    }

    void writeDataToJsonArray(const CustomItem* rootItem, rapidjson::Value& parentArray, rapidjson::Document& document) {
        QHash<int, CustomItem*> childrenItems = rootItem->childrenItems();
        for (int i = 0; i < childrenItems.count(); i++) {
            CustomItem* item = childrenItems[i];

            switch (item->itemType()) {
                case rapidjson::Type::kFalseType: {
                    // qDebug() << "Undefined";
                    rapidjson::Value element;
                    element.SetBool(item->value(1).toBool());
                    parentArray.PushBack(element, document.GetAllocator());
                } break;
                case rapidjson::Type::kTrueType: {
                    // qDebug() << "Bool";
                    rapidjson::Value element;
                    element.SetBool(item->value(1).toBool());
                    parentArray.PushBack(element, document.GetAllocator());
                } break;
                case rapidjson::Type::kNullType: {
                    // qDebug() << "Null";
                    rapidjson::Value element;
                    element.SetNull();
                    parentArray.PushBack(element, document.GetAllocator());
                } break;
                case rapidjson::Type::kStringType: {
                    // qDebug() << "String";
                    rapidjson::Value element;
                    element.SetString(item->value(1).toString().toStdString().c_str(), document.GetAllocator());
                    parentArray.PushBack(element, document.GetAllocator());
                } break;
                case rapidjson::Type::kNumberType: {
                    // qDebug() << "Double";
                    rapidjson::Value element;
                    switch (item->numberType()) {
                        case CustomItem::kDoubleFlag: {
                            element.SetDouble(item->value(1).toDouble());
                        } break;
                        case CustomItem::kIntFlag: {
                            element.SetInt(item->value(1).toInt());
                        } break;
                        case CustomItem::kUintFlag: {
                            element.SetUint(item->value(1).toUInt());
                        } break;
                        case CustomItem::kInt64Flag: {
                            element.SetInt64(item->value(1).toLongLong());
                        } break;
                        case CustomItem::kUint64Flag: {
                            element.SetUint64(item->value(1).toULongLong());
                        } break;
                        default: {
                        }
                    }
                    parentArray.PushBack(element, document.GetAllocator());
                } break;
                case rapidjson::Type::kArrayType: {
                    // qDebug() << "Array";
                    rapidjson::Value element;
                    element.SetArray();
                    parentArray.PushBack(element, document.GetAllocator());
                } break;
                case rapidjson::Type::kObjectType: {
                    // qDebug() << "Object";
                    rapidjson::Value element;
                    element.SetObject();

                    writeDataToJsonObject(item, element, document);

                    parentArray.PushBack(element, document.GetAllocator());
                } break;
                default: {
                    // qDebug() << "default";
                } break;
            }
        }
    }

    void parseJsonFile(const char* jsonFile, CustomItemModel* CustomModel);

    CustomItem* parseJsonElement(const rapidjson::GenericValue<rapidjson::UTF8<>>& obj, CustomItemModel* model, CustomItem* parentItem = nullptr, int depth = 0);

private:
    CustomItemModel* m_model = nullptr;
};
