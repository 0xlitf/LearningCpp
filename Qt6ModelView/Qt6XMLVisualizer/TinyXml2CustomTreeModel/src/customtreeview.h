#pragma once

#include "tinyxml2.h"
#include <QStandardItemModel>
#include <QTreeView>
#include <iostream>
#include <QXmlStreamWriter>
#include "customitem.h"
#include "customitemmodel.h"

class TinyXMLTreeViewCustom : public QTreeView {
public:
    TinyXMLTreeViewCustom(QWidget* parent = nullptr);

    void test();

    void writeModelDataToXML(const QAbstractItemModel* model, const QModelIndex& parentIndex, QXmlStreamWriter& xmlWriter);
    void writeFile();

    void displayXmlElement(const tinyxml2::XMLElement* element, CustomItemModel* model, CustomItem* parentItem = nullptr, int depth = 0);

    tinyxml2::XMLElement* findElementByPath(tinyxml2::XMLElement* root, const std::string& path);

    void find(const char* xml, std::string path);

    void parseGDTF2StandardItemModel(const char* xml, CustomItemModel* CustomModel);

    void openXML();

private:
    // CustomItemModel CustomModel
    CustomItemModel* m_model = nullptr;
};
