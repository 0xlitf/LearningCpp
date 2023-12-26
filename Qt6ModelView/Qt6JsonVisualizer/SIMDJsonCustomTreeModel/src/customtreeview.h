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
#include "simdjson.h"
#include "customitem.h"
#include "customitemmodel.h"

class SIMDJsonTreeView : public QTreeView {
public:
    SIMDJsonTreeView(QWidget* parent = nullptr);

    void test();

    void openJson();

    void writeJson();

    void writeDataToJsonObject(const CustomItem* rootItem, simdjson::ondemand::value& parentObject, simdjson::dom::document& document) {
//        QHash<int, CustomItem*> childrenItems = rootItem->childrenItems();
//        for (int i = 0; i < childrenItems.count(); i++) {
//            CustomItem* item = childrenItems[i];

//            switch (item->itemType()) {
//                case simdjson::ondemand::json_type::boolean: {
//                    // qDebug() << "kTrueType";
//                    simdjson::ondemand::value element;
//                    element.SetBool(item->value(1).toBool());
//                    parentObject.AddMember(simdjson::ondemand::value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
//                } break;
//                case simdjson::ondemand::json_type::null: {
//                    // qDebug() << "Null";
//                    simdjson::ondemand::value element;
//                    element.SetNull();
//                    parentObject.AddMember(simdjson::ondemand::value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
//                } break;
//                case simdjson::ondemand::json_type::string: {
//                    // qDebug() << "String";
//                    simdjson::ondemand::value element;
//                    element.SetString(item->value(1).toString().toStdString().c_str(), document.GetAllocator());
//                    parentObject.AddMember(simdjson::ondemand::value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
//                } break;
//                case simdjson::ondemand::json_type::number: {
//                    // qDebug() << "Double";
//                    simdjson::ondemand::value element;
//                    switch (item->numberType()) {
//                        case CustomItem::kDoubleFlag: {
//                            element.SetDouble(item->value(1).toDouble());
//                        } break;
//                        case CustomItem::kIntFlag: {
//                            element.SetInt(item->value(1).toInt());
//                        } break;
//                        case CustomItem::kUintFlag: {
//                            element.SetUint(item->value(1).toUInt());
//                        } break;
//                        case CustomItem::kInt64Flag: {
//                            element.SetInt64(item->value(1).toLongLong());
//                        } break;
//                        case CustomItem::kUint64Flag: {
//                            element.SetUint64(item->value(1).toULongLong());
//                        } break;
//                        default: {
//                        }
//                    }
//                    parentObject.AddMember(simdjson::ondemand::value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
//                } break;
//                case simdjson::ondemand::json_type::array: {
//                    // qDebug() << "Array";
//                    simdjson::ondemand::value element;
//                    element.SetArray();
//                    writeDataToJsonArray(item, element, document);

//                    parentObject.AddMember(simdjson::ondemand::value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
//                } break;
//                case simdjson::ondemand::json_type::object: {
//                    // qDebug() << "Object";
//                    simdjson::ondemand::value element;
//                    element.SetObject();

//                    writeDataToJsonObject(item, element, document);

//                    parentObject.AddMember(simdjson::ondemand::value(item->value(0).toString().toStdString().c_str(), document.GetAllocator()), element, document.GetAllocator());
//                } break;
//                default: {
//                    // qDebug() << "default";
//                } break;
//            }
//        }
    }

    void writeDataToJsonArray(const CustomItem* rootItem, simdjson::ondemand::value& parentArray, simdjson::dom::document& document) {
//        QHash<int, CustomItem*> childrenItems = rootItem->childrenItems();
//        for (int i = 0; i < childrenItems.count(); i++) {
//            CustomItem* item = childrenItems[i];

//            switch (item->itemType()) {
//                case simdjson::ondemand::json_type::boolean: {
//                    // qDebug() << "Bool";
//                    simdjson::ondemand::value element;
//                    element.SetBool(item->value(1).toBool());
//                    parentArray.PushBack(element, document.GetAllocator());
//                } break;
//                case simdjson::ondemand::json_type::null: {
//                    // qDebug() << "Null";
//                    simdjson::ondemand::value element;
//                    element.SetNull();
//                    parentArray.PushBack(element, document.GetAllocator());
//                } break;
//                case simdjson::ondemand::json_type::string: {
//                    // qDebug() << "String";
//                    simdjson::ondemand::value element;
//                    element.SetString(item->value(1).toString().toStdString().c_str(), document.GetAllocator());
//                    parentArray.PushBack(element, document.GetAllocator());
//                } break;
//                case simdjson::ondemand::json_type::number: {
//                    // qDebug() << "Double";
//                    simdjson::ondemand::value element;
//                    switch (item->numberType()) {
//                        case CustomItem::kDoubleFlag: {
//                            element.SetDouble(item->value(1).toDouble());
//                        } break;
//                        case CustomItem::kIntFlag: {
//                            element.SetInt(item->value(1).toInt());
//                        } break;
//                        case CustomItem::kUintFlag: {
//                            element.SetUint(item->value(1).toUInt());
//                        } break;
//                        case CustomItem::kInt64Flag: {
//                            element.SetInt64(item->value(1).toLongLong());
//                        } break;
//                        case CustomItem::kUint64Flag: {
//                            element.SetUint64(item->value(1).toULongLong());
//                        } break;
//                        default: {
//                        }
//                    }
//                    parentArray.PushBack(element, document.GetAllocator());
//                } break;
//                case simdjson::ondemand::json_type::array: {
//                    // qDebug() << "Array";
//                    simdjson::ondemand::value element;
//                    element.SetArray();
//                    parentArray.PushBack(element, document.GetAllocator());
//                } break;
//                case simdjson::ondemand::json_type::object: {
//                    // qDebug() << "Object";
//                    simdjson::ondemand::value element;
//                    element.SetObject();

//                    writeDataToJsonObject(item, element, document);

//                    parentArray.PushBack(element, document.GetAllocator());
//                } break;
//                default: {
//                    // qDebug() << "default";
//                } break;
//            }
//        }
    }

    void parseJsonFile(const char* jsonFile, CustomItemModel* CustomModel);

    CustomItem* parseJsonElement(simdjson::ondemand::value& obj, CustomItemModel* model, CustomItem* parentItem = nullptr, int depth = 0);

private:
    CustomItemModel* m_model = nullptr;
};
