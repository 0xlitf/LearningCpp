#pragma once

#include <QStandardItemModel>
#include <QTreeView>
#include <QXmlStreamWriter>
#include <iostream>
#include "customitem.h"
#include "customitemmodel.h"
#include "Rapidxml.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
//#include "rapidxpath.hpp"

using namespace rapidxml;

class RapidXmlTreeView : public QTreeView {
public:
    RapidXmlTreeView(QWidget* parent = nullptr);

    void test();

    void writeModelDataToXML(const QAbstractItemModel* model, const QModelIndex& parentIndex, QXmlStreamWriter& xmlWriter);
    void writeFile();

    void displayXmlElement(rapidxml::xml_node<>* node, CustomItemModel* model, CustomItem* parentItem = nullptr, int depth = 0);

    void find(const char* xml, std::string path);

    void parseGDTF2StandardItemModel(const char* xml, CustomItemModel* CustomModel);

    void openXML();

private:
    // CustomItemModel CustomModel
    CustomItemModel* m_model = nullptr;
};
