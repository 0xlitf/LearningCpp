#pragma once

#include <Poco/XML/XML.h>
#include <Poco/XML/XMLWriter.h>
#include <Poco/XML/XMLStreamParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/Text.h>
#include <Poco/DOM/CDATASection.h>
#include <Poco/DOM/ProcessingInstruction.h>
#include <Poco/DOM/Comment.h>
#include <Poco/DOM/DOMWriter.h>
#include <Poco/XML/XMLWriter.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/Attr.h>
#include <Poco/DOM/NamedNodeMap.h>
#include <Poco/XML/XMLString.h>
#include <Poco/XML/XMLException.h>
#include <Poco/XML/XMLStream.h>
#include <Poco/DOM/NodeIterator.h>
#include <Poco/DOM/NodeFilter.h>
#include <QStandardItemModel>
#include <QTreeView>
#include <iostream>
#include <QXmlStreamWriter>
#include "customitem.h"
#include "customitemmodel.h"

class PocoXmlTreeViewCustom : public QTreeView {
public:
    PocoXmlTreeViewCustom(QWidget* parent = nullptr);

    void test();

    void writeModelDataToXML(const QAbstractItemModel* model, const QModelIndex& parentIndex, QXmlStreamWriter& xmlWriter);
    void writeFile();

    void displayXmlElement(Poco::XML::Node* element, CustomItemModel* model, CustomItem* parentItem = nullptr, int depth = 0);

    Poco::XML::Element* findElementByPath(Poco::XML::Element* root, const std::string& path);

    void find(const char* xml, std::string path);

    void parseGDTF2StandardItemModel(std::string xml, CustomItemModel* CustomModel);

    void openXML();

private:
    // CustomItemModel CustomModel
    CustomItemModel* m_model = nullptr;
};
