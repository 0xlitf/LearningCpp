#include "customtreeview.h"
#include <QItemDelegate>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QResource>
#include <QXmlStreamWriter>
#include <QThread>

RapidXmlTreeView::RapidXmlTreeView(QWidget* parent)
    : QTreeView(parent) {

}

void RapidXmlTreeView::test() {

}

void RapidXmlTreeView::displayXmlElement(rapidxml::xml_node<>* node, CustomItemModel* model, CustomItem* parentItem, int depth) {
    if (!node) {
        return;
    }

    CustomItem* item = new CustomItem();
    item->setItemType(CustomItem::ItemType::ELEMENT);

    item->setElementName(QString::fromStdString(node->name()));
    item->addAttribute(QStringList{node->name(), "", QString()});

    if (parentItem) {
        parentItem->addChild(item);
    } else {
        model->getRootItem()->addChild(item);
    }

    rapidxml::xml_attribute<>* attribute = node->first_attribute();

    while (attribute) {
        qDebug().noquote() << QString("    ").repeated(depth) + (node->first_attribute() || attribute->next_attribute() ? QString("\u2523\u2501 ") : QString("\u2517\u2501 ")) + "\u25cf " + QString::fromStdString(attribute->name()) + "： " + QString::fromStdString(attribute->value()) + "\t{ xpath -> " + QString() + " }";

        CustomItem* childItem = new CustomItem();
        childItem->setItemType(CustomItem::ItemType::ATTRIBUTE);

        childItem->addAttribute(QStringList{attribute->name(), attribute->value(), QString()});

        item->addChild(childItem);
        attribute = attribute->next_attribute();
    }

    rapidxml::xml_node<>* childElement = node->first_node();
    while (childElement) {
        qDebug().noquote() << QString("    ").repeated(depth) + (childElement->next_sibling() ? QString("\u2523\u2501 ") : QString("\u2517\u2501 ")) + "\u25c6 " + QString::fromStdString(childElement->name()) << "\t{ xpath -> " + QString() + " }";

        displayXmlElement(childElement, model, item, depth + 1);
        childElement = childElement->next_sibling();
    }

    // qDebug() << "item: " << item->elementName() << ", attributeCount: " << item->attributeCount() << ", childrenCount:" << item->childrenCount();
}

void RapidXmlTreeView::parseGDTF2StandardItemModel(const char* xml, CustomItemModel* model) {
    using namespace rapidxml;
    rapidxml::file<> fdoc(xml);
//    std::cout << fdoc.data();

    rapidxml::xml_document<> doc;
    doc.parse<0>(fdoc.data());

    rapidxml::xml_node<>* firstNode = doc.first_node();
    if (firstNode) {
        qDebug() << "First Node Name: " << firstNode->name();

        CustomItem* rootItem = new CustomItem();
        model->setRootItem(rootItem);

        displayXmlElement(firstNode, model, rootItem);

//        while (rapidxml::xml_node firstNode = doc.first_child()) {
//            if (node.ToDeclaration()) {
//                const RapidXml2::XMLDeclaration* declaration = node.ToDeclaration();
//                const char* declarationText = declaration->Value();
//                qDebug() << "declarationText: " << declarationText;
//                if (declarationText) {
//                    std::string declarationStr(declarationText);

//                    size_t versionPos = declarationStr.find("version=\"");
//                    if (versionPos != std::string::npos) {
//                        size_t versionEndPos = declarationStr.find("\"", versionPos + 9);
//                        if (versionEndPos != std::string::npos) {
//                            std::string versionValue = declarationStr.substr(versionPos + 9, versionEndPos - versionPos - 9);
//                            qDebug() << "XML Version: " << versionValue;
//                        }
//                    }

//                    size_t encodingPos = declarationStr.find("encoding=\"");
//                    if (encodingPos != std::string::npos) {
//                        size_t encodingEndPos = declarationStr.find("\"", encodingPos + 10);
//                        if (encodingEndPos != std::string::npos) {
//                            std::string encodingValue = declarationStr.substr(encodingPos + 10, encodingEndPos - encodingPos - 10);
//                            qDebug() << "XML encoding: " << encodingValue;
//                        }
//                    }

//                    size_t standalonePos = declarationStr.find("standalone=\"");
//                    if (standalonePos != std::string::npos) {
//                        size_t standaloneEndPos = declarationStr.find("\"", standalonePos + 12);
//                        if (standaloneEndPos != std::string::npos) {
//                            std::string standaloneValue = declarationStr.substr(standalonePos + 12, standaloneEndPos - standalonePos - 12);
//                            qDebug() << "XML standalone: " << standaloneValue;
//                        }
//                    }

//                    break;
//                }
//            }
//        }

    } else {
        qDebug() << "XML parsing failed. Error description: ";
    }
}

void RapidXmlTreeView::openXML() {
    // C:/0xlitf/build-Qt6ModelView-Desktop_Qt_6_5_2_MSVC2019_64bit-Debug/bin/customtreeview.xml
    // C:/Users/Calcifer/Downloads/Nanlite@Pavotube_II_15X@DMX_V1.0_12.Jun.2023/description_simple.xml
    if (m_model) {
        this->setModel(nullptr);
        delete m_model;
    }
    m_model = new CustomItemModel();
    const char* xml = "C:/Users/Calcifer/Downloads/Nanlite@Pavotube_II_15X@DMX_V1.0_12.Jun.2023/description.xml"; // description_simple.xml description.xml

    qDebug() << "xml: " << QUrl(xml);

    parseGDTF2StandardItemModel(xml, m_model);

    this->setModel(m_model);

    this->expandAll();

    this->setEditTriggers(QTreeView::AllEditTriggers);
    this->setSelectionBehavior(QTreeView::SelectRows);
    this->setSelectionMode(QTreeView::SingleSelection);

    this->setColumnWidth(0, 350);
    this->setColumnWidth(1, 350);
}

void RapidXmlTreeView::writeFile() {
    // 创建 XML 文件
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

void RapidXmlTreeView::writeModelDataToXML(const QAbstractItemModel* model, const QModelIndex& parentIndex, QXmlStreamWriter& xmlWriter) {
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
