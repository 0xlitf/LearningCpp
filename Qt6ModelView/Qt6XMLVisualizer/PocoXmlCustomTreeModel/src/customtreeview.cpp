#include "customtreeview.h"
#include <QItemDelegate>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <Poco/AutoPtr.h>
#include <Poco/DOM/NodeList.h>

PocoXmlTreeViewCustom::PocoXmlTreeViewCustom(QWidget* parent)
    : QTreeView(parent) {

}

void PocoXmlTreeViewCustom::test() {

}

void PocoXmlTreeViewCustom::displayXmlElement(Poco::XML::Node* element, CustomItemModel* model, CustomItem* parentItem, int depth) {
    ++depth;
    if (!element) {
        return;
    }

    CustomItem* item = new CustomItem();
    item->setItemType(CustomItem::ItemType::ELEMENT);

    item->setElementName(QString::fromStdString(element->nodeName()));
    qDebug().noquote() << QString("  ").repeated(depth) + QString(" * ") << QString::fromStdString(element->nodeName());

    if (parentItem) {
        parentItem->addChild(item);
    } else {
        model->getRootItem()->addChild(item);
    }
    /*
        ELEMENT_NODE = 1,             /// The node is an Element.
        ATTRIBUTE_NODE,               /// The node is an Attr.
        TEXT_NODE,                    /// The node is a Text node.
        CDATA_SECTION_NODE,           /// The node is a CDATASection.
        ENTITY_REFERENCE_NODE,        /// The node is an EntityReference.
        ENTITY_NODE,                  /// The node is an Entity.
        PROCESSING_INSTRUCTION_NODE,  /// The node is a ProcessingInstruction.
        COMMENT_NODE,                 /// The node is a Comment.
        DOCUMENT_NODE,                /// The node is a Document.
        DOCUMENT_TYPE_NODE,           /// The node is a DocumentType.
        DOCUMENT_FRAGMENT_NODE,       /// The node is a DocumentFragment.
        NOTATION_NODE                 /// The node is a Notation.
*/
    switch (element->nodeType()) {
    case Poco::XML::Node::ATTRIBUTE_NODE: {

    } break;
    case Poco::XML::Node::ELEMENT_NODE: {
        Poco::XML::Element* pElement = dynamic_cast<Poco::XML::Element*>(element);
        std::string elementName = pElement->nodeName();
        // std::cout << "Element Name: " << elementName << std::endl;

        if (elementName != "#text") {
            // Process attributes of the element if needed
            Poco::XML::NamedNodeMap* pAttributes = pElement->attributes();
            for (int i = 0; i < pAttributes->length(); ++i) {
                Poco::XML::Node* pAttribute = pAttributes->item(i);
                std::string attributeName = pAttribute->nodeName();
                std::string attributeValue = pAttribute->nodeValue();

                std::cout << "attributeName: " << attributeName << ", attributeValue: " << attributeValue << std::endl;

                CustomItem* childItem = new CustomItem();
                childItem->setItemType(CustomItem::ItemType::ATTRIBUTE);

                childItem->addAttribute(QString::fromStdString(attributeName), QString::fromStdString(attributeValue));

                item->addChild(childItem);
            }
        }

        Poco::XML::NodeList* pChildren = pElement->childNodes();
        for (int i = 0; i < pChildren->length(); ++i) {
            if (pChildren->item(i)->nodeName() != "#text") {
                displayXmlElement(pChildren->item(i), model, item, depth);
            }
        }

    } break;
    case Poco::XML::Node::TEXT_NODE: {
        // std::string text = element->nodeValue();
        // std::cout << "Text: " << text << std::endl;
    } break;
    case Poco::XML::Node::CDATA_SECTION_NODE: {
    } break;
    case Poco::XML::Node::ENTITY_REFERENCE_NODE: {
    } break;
    case Poco::XML::Node::ENTITY_NODE: {
    } break;
    case Poco::XML::Node::PROCESSING_INSTRUCTION_NODE: {
    } break;
    case Poco::XML::Node::COMMENT_NODE: {
    } break;
    case Poco::XML::Node::DOCUMENT_NODE: {
    } break;
    case Poco::XML::Node::DOCUMENT_TYPE_NODE: {
    } break;
    case Poco::XML::Node::DOCUMENT_FRAGMENT_NODE: {
    } break;
    case Poco::XML::Node::NOTATION_NODE: {
    } break;
    }
}

void PocoXmlTreeViewCustom::parseGDTF2StandardItemModel(std::string xml, CustomItemModel* model) {
    Poco::XML::DOMParser parser;
    qDebug() << QString::fromStdString(xml);
    Poco::AutoPtr<Poco::XML::Document> pDoc = parser.parse("C:/Users/Calcifer/Downloads/Nanlite@Pavotube_II_15X@DMX_V1.0_12.Jun.2023/description.xml");
    qDebug() << pDoc;
    if (pDoc) {
        Poco::XML::Element* pRootElement = pDoc->documentElement();

        Poco::XML::NodeIterator it(pRootElement, Poco::XML::NodeFilter::SHOW_ALL);
        Poco::XML::Node* pNode = it.nextNode();

        if (pNode) {
            Poco::XML::Node* pFirstChildElement = dynamic_cast<Poco::XML::Node*>(pNode);
            std::string elementName = pFirstChildElement->nodeName();
            std::string elementValue = pFirstChildElement->nodeValue();

            std::cout << "Element Name: " << elementName << std::endl;
            std::cout << "Element Value: " << elementValue << std::endl;

            CustomItem* rootItem = new CustomItem();
            model->setRootItem(rootItem);

            displayXmlElement(pFirstChildElement, model, rootItem);
        } else {
            std::cerr << "No child elements found." << std::endl;
        }
    } else {
        qDebug() << "xml parse error";
    }
}

void PocoXmlTreeViewCustom::openXML() {
    // C:/0xlitf/build-Qt6ModelView-Desktop_Qt_6_5_2_MSVC2019_64bit-Debug/bin/customtreeview.xml
    // C:/Users/Calcifer/Downloads/Nanlite@Pavotube_II_15X@DMX_V1.0_12.Jun.2023/description_simple.xml
    if (m_model) {
        this->setModel(nullptr);
        delete m_model;
    }
    m_model = new CustomItemModel();
    std::string xml = "C:/Users/Calcifer/Downloads/Nanlite@Pavotube_II_15X@DMX_V1.0_12.Jun.2023/description.xml"; // description_simple.xml description.xml
    // const char* xml = ":/description.xml";
    qDebug() << "xml: " << QUrl(QString::fromStdString(xml));
    QFile file(QString::fromStdString(xml));
    if (file.open(QIODevice::ReadOnly)) {
        // std::string content = file.readAll().toStdString();
        // qDebug() << "content: " << content;
        parseGDTF2StandardItemModel(xml, m_model);

        this->setModel(m_model);

        this->expandAll();

        this->setEditTriggers(QTreeView::AllEditTriggers);
        this->setSelectionBehavior(QTreeView::SelectRows);
        this->setSelectionMode(QTreeView::SingleSelection);

        this->setColumnWidth(0, 350);
    }
}

void PocoXmlTreeViewCustom::writeFile() {
    QFile file("customtreeview.xml");
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);

        xmlWriter.writeStartDocument();
        writeModelDataToXML(m_model, QModelIndex(), xmlWriter);
        xmlWriter.writeEndDocument();

        file.close();
    } else {
        qDebug() << "Failed to open the XML file for writing.";
    }
}

void PocoXmlTreeViewCustom::writeModelDataToXML(const QAbstractItemModel* model, const QModelIndex& parentIndex, QXmlStreamWriter& xmlWriter) {
    int rowCount = model->rowCount(parentIndex);
    qDebug() << "rowCount: " << rowCount;

    for (int i = 0; i < rowCount; ++i) {
        QModelIndex index0 = model->index(i, 0, parentIndex);
        QModelIndex index1 = model->index(i, 1, parentIndex);
        CustomItem* childItem = static_cast<CustomItem*>(index0.internalPointer());
        qDebug() << "index0, index1: " << index0.data(Qt::DisplayRole).toString() << " " << index1.data(Qt::DisplayRole).toString();

        if ("Wheels" == index0.data(Qt::DisplayRole).toString()) {
            int a = 1;
        }
        QString itemName = index0.data(Qt::DisplayRole).toString();
        QString itemAttribute = index1.data(Qt::DisplayRole).toString();

        if (!childItem->hasChildren()) {
            xmlWriter.writeAttribute(itemName, itemAttribute);
        } else {
            xmlWriter.writeStartElement(itemName);
            writeModelDataToXML(m_model, index0, xmlWriter);
            xmlWriter.writeEndElement();
        }

    }
}
