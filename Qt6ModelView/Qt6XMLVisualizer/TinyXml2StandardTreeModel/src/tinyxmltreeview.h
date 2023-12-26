#pragma once

#include "tinyxml2.h"
#include <QStandardItemModel>
#include <QTreeView>
#include <iostream>

class TinyXMLTreeView : public QTreeView {

public:
    TinyXMLTreeView(QWidget* parent = nullptr);

    void displayXmlElement(const tinyxml2::XMLElement* element, QStandardItemModel* UserDefinedModel, QStandardItem* parentItem = nullptr);

    tinyxml2::XMLElement* findElementByPath(tinyxml2::XMLElement* root, const std::string& path);

    void find(const char* xml, std::string path);

    void parseGDTF2StandardItemModel(const char* xml, QStandardItemModel* UserDefinedModel);

    void openXML();
private:

    QStandardItemModel* m_model = new QStandardItemModel(1, 2, this);
};
