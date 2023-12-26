#include "tinyxmltreeview.h"

TinyXMLTreeView::TinyXMLTreeView(QWidget* parent)
    : QTreeView(parent) {
    //    find(xml, "FixtureType");
}

void TinyXMLTreeView::displayXmlElement(const tinyxml2::XMLElement* element, QStandardItemModel* model, QStandardItem* parentItem) {
    if (!element) {
        return;
    }

    QStandardItem* item = new QStandardItem(QString::fromStdString(element->Name()));

    if (parentItem) {
        parentItem->appendRow(item);
    } else {
        model->appendRow(item);
    }

    const tinyxml2::XMLAttribute* attribute = element->FirstAttribute();
    while (attribute) {
        QStandardItem* attrItem = new QStandardItem(QString("%1").arg(QString::fromStdString(attribute->Name())));
        QStandardItem* valueItem = new QStandardItem(QString("%1").arg(QString::fromStdString(attribute->Value())));

        qDebug() << "QStandardItem* valueItem: " << QString::fromStdString(attribute->Name());
        qDebug() << "QStandardItem* valueItem: " << QString::fromStdString(attribute->Value());

        item->appendRow({attrItem, valueItem});
//        item->appendColumn();
        attribute = attribute->Next();
    }

    const tinyxml2::XMLElement* childElement = element->FirstChildElement();
    while (childElement) {
        displayXmlElement(childElement, model, item);
        childElement = childElement->NextSiblingElement();
    }
}

tinyxml2::XMLElement* TinyXMLTreeView::findElementByPath(tinyxml2::XMLElement* root, const std::string& path) {
    std::vector<std::string> elements;
    size_t start = 0, end;
    while ((end = path.find('/', start)) != std::string::npos) {
        elements.push_back(path.substr(start, end - start));
        start = end + 1;
    }
    elements.push_back(path.substr(start));

    tinyxml2::XMLElement* currentElement = root;
    for (const std::string& elementName : elements) {
        currentElement = currentElement->FirstChildElement(elementName.c_str());
        if (!currentElement) {
            return nullptr;
        }
    }

    return currentElement;
}

void TinyXMLTreeView::find(const char* xml, std::string path) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(xml) == tinyxml2::XML_SUCCESS) {
        tinyxml2::XMLElement* element = findElementByPath(doc.RootElement(), path);
        qDebug() << "find element: " << element;
        if (element) {
            // 获取属性的值
            const char* attributeValue = element->Attribute("attribute_name"); // 替换为您的属性名称
            if (attributeValue) {
                std::cout << "Attribute value: " << attributeValue << std::endl;
            } else {
                std::cout << "Attribute not found." << std::endl;
            }
        } else {
            std::cout << "Element not found." << std::endl;
        }
    } else {
        // 处理解析失败
        std::cerr << "Failed to parse XML file." << std::endl;
    }
}

void TinyXMLTreeView::parseGDTF2StandardItemModel(const char* xml, QStandardItemModel* UserDefinedModel) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(xml) == tinyxml2::XML_SUCCESS) {
        const tinyxml2::XMLNode* node = doc.FirstChild();
        while (node) {
            if (node->ToDeclaration()) {
                const tinyxml2::XMLDeclaration* declaration = node->ToDeclaration();

                const char* declarationText = declaration->Value();
                qDebug() << declarationText;
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
            node = node->NextSibling();
        }

        const tinyxml2::XMLElement* root = doc.FirstChildElement();
        if (root) {
            qDebug() << QString::fromStdString(root->Name());

            displayXmlElement(root, UserDefinedModel);
        }
    } else {
        qDebug() << "xml parse error";
    }
}

void TinyXMLTreeView::openXML() {
    const char* xml = "C:/Users/Calcifer/Downloads/Nanlite@Pavotube_II_15X@DMX_V1.0_12.Jun.2023/description_simple.xml";
    parseGDTF2StandardItemModel(xml, m_model);
    m_model->setHorizontalHeaderLabels({"key", "value"});
    this->setModel(m_model);

    this->setColumnWidth(0, 350);
}
