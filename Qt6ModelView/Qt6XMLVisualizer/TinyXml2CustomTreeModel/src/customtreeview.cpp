#include "customtreeview.h"
#include <QItemDelegate>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QResource>
#include <QXmlStreamWriter>

TinyXMLTreeViewCustom::TinyXMLTreeViewCustom(QWidget* parent)
    : QTreeView(parent) {

}

void TinyXMLTreeViewCustom::test() {

}

void TinyXMLTreeViewCustom::displayXmlElement(const tinyxml2::XMLElement* element, CustomItemModel* model, CustomItem* parentItem, int depth) {
    if (!element) {
        return;
    }

    CustomItem* item = new CustomItem();
    item->setItemType(CustomItem::ItemType::ELEMENT);

    item->setElementName(QString::fromStdString(element->Name()));
    // qDebug().noquote() << QString("  ").repeated(depth) + QString(" * ") << QString::fromStdString(element->Name());

    if (parentItem) {
        parentItem->addChild(item);
    } else {
        model->getRootItem()->addChild(item);
    }

    const tinyxml2::XMLAttribute* attribute = element->FirstAttribute();

    while (attribute) {
        // qDebug().noquote() << QString("  ").repeated(depth) + QString(" - ") + QString::fromStdString(attribute->Name()) << ", " << QString::fromStdString(attribute->Value());

        CustomItem* childItem = new CustomItem();
        childItem->setItemType(CustomItem::ItemType::ATTRIBUTE);

        childItem->addAttribute(attribute->Name(), attribute->Value());

        item->addChild(childItem);
        attribute = attribute->Next();
    }

    const tinyxml2::XMLElement* childElement = element->FirstChildElement();
    while (childElement) {
        displayXmlElement(childElement, model, item, depth + 1);
        childElement = childElement->NextSiblingElement();
    }

    // qDebug() << "item: " << item->elementName() << ", attributeCount: " << item->attributeCount() << ", childrenCount:" << item->childrenCount();
}

void TinyXMLTreeViewCustom::parseGDTF2StandardItemModel(const char* xml, CustomItemModel* model) {
    tinyxml2::XMLDocument doc;
    if (doc.Parse(xml) == tinyxml2::XML_SUCCESS) {
        while (const tinyxml2::XMLNode* node = doc.FirstChild()) {
            if (node->ToDeclaration()) {
                const tinyxml2::XMLDeclaration* declaration = node->ToDeclaration();

                const char* declarationText = declaration->Value();
                qDebug() << "declarationText: " << declarationText;
                if (declarationText) {
                    std::string declarationStr(declarationText);

                    size_t versionPos = declarationStr.find("version=\"");
                    if (versionPos != std::string::npos) {
                        size_t versionEndPos = declarationStr.find("\"", versionPos + 9);
                        if (versionEndPos != std::string::npos) {
                            std::string versionValue = declarationStr.substr(versionPos + 9, versionEndPos - versionPos - 9);
                            qDebug() << "XML Version: " << versionValue;
                        }
                    }

                    size_t encodingPos = declarationStr.find("encoding=\"");
                    if (encodingPos != std::string::npos) {
                        size_t encodingEndPos = declarationStr.find("\"", encodingPos + 10);
                        if (encodingEndPos != std::string::npos) {
                            std::string encodingValue = declarationStr.substr(encodingPos + 10, encodingEndPos - encodingPos - 10);
                            qDebug() << "XML encoding: " << encodingValue;
                        }
                    }

                    size_t standalonePos = declarationStr.find("standalone=\"");
                    if (standalonePos != std::string::npos) {
                        size_t standaloneEndPos = declarationStr.find("\"", standalonePos + 12);
                        if (standaloneEndPos != std::string::npos) {
                            std::string standaloneValue = declarationStr.substr(standalonePos + 12, standaloneEndPos - standalonePos - 12);
                            qDebug() << "XML standalone: " << standaloneValue;
                        }
                    }

                    break;
                }
            }
        }

        if (const tinyxml2::XMLElement* root = doc.FirstChildElement()) {
            CustomItem* rootItem = new CustomItem();
            model->setRootItem(rootItem);

            displayXmlElement(root, model, rootItem);
        }
    } else {
        qDebug() << "xml parse error";
    }
}

void TinyXMLTreeViewCustom::openXML() {
    // C:/0xlitf/build-Qt6ModelView-Desktop_Qt_6_5_2_MSVC2019_64bit-Debug/bin/customtreeview.xml
    // C:/Users/Calcifer/Downloads/Nanlite@Pavotube_II_15X@DMX_V1.0_12.Jun.2023/description_simple.xml
    if (m_model) {
        this->setModel(nullptr);
        delete m_model;

    }
    m_model = new CustomItemModel();
    //        const char* xml = "C:/Users/Calcifer/Downloads/Nanlite@Pavotube_II_15X@DMX_V1.0_12.Jun.2023/description.xml"; // description_simple.xml description.xml
    const char* xml = ":/description.xml";
    qDebug() << "xml: " << QUrl(xml);
    QFile file(xml);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray content = file.readAll();
        parseGDTF2StandardItemModel(content, m_model);

        this->setModel(m_model);

        this->expandAll();

        this->setEditTriggers(QTreeView::AllEditTriggers);
        this->setSelectionBehavior(QTreeView::SelectRows);
        this->setSelectionMode(QTreeView::SingleSelection);

        this->setColumnWidth(0, 350);
    }
}

void TinyXMLTreeViewCustom::writeFile() {
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

void TinyXMLTreeViewCustom::writeModelDataToXML(const QAbstractItemModel* model, const QModelIndex& parentIndex, QXmlStreamWriter& xmlWriter) {
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
