#pragma once

#include <QStandardItemModel>
#include <QTreeView>
#include <QXmlStreamWriter>
#include <iostream>
#include "customitem.h"
#include "customitemmodel.h"
#include "pugixml.hpp"

class PugiXmlTreeViewCustom : public QTreeView {
public:
    PugiXmlTreeViewCustom(QWidget* parent = nullptr);

    void test();

    void writeModelDataToXML(const QAbstractItemModel* model, const QModelIndex& parentIndex, QXmlStreamWriter& xmlWriter);
    void writeFile();

    void displayXmlElement(pugi::xml_node& node, CustomItemModel* model, CustomItem* parentItem = nullptr, int depth = 0);

    pugi::xml_node* findElementByPath(pugi::xml_node* root, const std::string& path);

    std::string getAttributeXPath(const pugi::xml_node& node, const pugi::xml_attribute& attribute);
    std::string getNodeXPath(const pugi::xml_node& node);

    void find(const char* xml, std::string path);

    void parseGDTF2StandardItemModel(const char* xml, CustomItemModel* CustomModel);

    void openXML();

private:
    // CustomItemModel CustomModel
    CustomItemModel* m_model = nullptr;
};
