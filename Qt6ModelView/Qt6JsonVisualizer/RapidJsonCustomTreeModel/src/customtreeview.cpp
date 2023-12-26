#include <QItemDelegate>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QResource>
#include <QThread>
#include "customtreeview.h"

RapidJsonTreeView::RapidJsonTreeView(QWidget* parent)
    : QTreeView(parent) {

}

void RapidJsonTreeView::test() {

}

void RapidJsonTreeView::parseJsonFile(const char* jsonFile, CustomItemModel* model) {
    QFile file(jsonFile);
    rapidjson::Document jdoc;
    if (file.open(QIODevice::ReadOnly)) {
        jdoc.Parse(file.readAll());
        file.close();
        qDebug() << "Json parsing success.";
    } else {
        qDebug() << "Json parsing failed.";
    }

    CustomItem* rootItem;

    if (jdoc.IsArray()) {
        rootItem = parseJsonElement(jdoc, m_model);
        rootItem->setType(rapidjson::Type::kArrayType);
    } else if (jdoc.IsObject()) {
        rootItem = parseJsonElement(jdoc, m_model);
        rootItem->setType(rapidjson::Type::kObjectType);
    } else if (jdoc.IsNull()) {

    }

    model->setRootItem(rootItem);
}

void RapidJsonTreeView::openJson() {
    if (m_model) {
        this->setModel(nullptr);
        delete m_model;
    }
    m_model = new CustomItemModel();
    const char* jsonFile = "C:/0xlitf/Qt6ModelView/Qt6JsonVisualizer/twitter_simple.json";

    qDebug() << "jsonFile: " << QUrl(jsonFile);

    parseJsonFile(jsonFile, m_model);

    this->setModel(m_model);

    this->expandAll();

    this->setEditTriggers(QTreeView::AllEditTriggers);
    this->setSelectionBehavior(QTreeView::SelectRows);
    this->setSelectionMode(QTreeView::SingleSelection);

    this->setColumnWidth(0, 350);
    this->setColumnWidth(1, 350);
}

void RapidJsonTreeView::writeJson()
{
    QFile writeFile("RapidJsonWriteFile.json");

    if (writeFile.open(QIODevice::Truncate | QIODeviceBase::WriteOnly)) {
        qDebug() << "writeFile open success";

        rapidjson::Document document;
        document.SetObject();

        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

        writeDataToJsonObject(m_model->getRootItem(), document.GetObject(), document);

        rapidjson::StringBuffer buffer;
        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        std::cout << buffer.GetString() << std::endl;

        writeFile.write(buffer.GetString());
        writeFile.close();
    } else {
        qDebug() << "writeFile open failed";
    }
}

CustomItem* RapidJsonTreeView::parseJsonElement(const rapidjson::GenericValue<rapidjson::UTF8<>>& value, CustomItemModel* model, CustomItem* parent, int depth) {
    CustomItem* self = new CustomItem(parent);
    if (parent) {

    }

    rapidjson::Type type = value.GetType();

    switch (type) {
        case rapidjson::Type::kFalseType: {
            self->setValue(1, value.GetBool());
        } break;
        case rapidjson::Type::kTrueType: {
            self->setValue(1, value.GetBool());
        } break;
        case rapidjson::Type::kNullType: {
            self->setValue(1, QVariant());
        } break;
        case rapidjson::Type::kStringType: {
            self->setValue(1, value.GetString());
        } break;
        case rapidjson::Type::kNumberType: {
            if (value.IsDouble()) {
                self->setValue(1, value.GetDouble());
                self->setType(value.GetType());
                self->setNumberType(CustomItem::kDoubleFlag);
            } else if (value.IsInt()) {
                self->setValue(1, value.GetInt());
                self->setType(value.GetType());
                self->setNumberType(CustomItem::kIntFlag);
            } else if (value.IsUint()) {
                self->setValue(1, value.GetUint());
                self->setType(value.GetType());
                self->setNumberType(CustomItem::kUintFlag);
            } else if (value.IsInt64()) {
                self->setValue(1, value.GetInt64());
                self->setType(value.GetType());
                self->setNumberType(CustomItem::kInt64Flag);
            } else {
                self->setValue(1, value.GetUint64());
                self->setType(value.GetType());
                self->setNumberType(CustomItem::kUint64Flag);
            }
        } break;
        case rapidjson::Type::kArrayType: {
            const rapidjson::Value&  array = value.GetArray();
            for (rapidjson::SizeType i = 0; i < array.Size(); i++) {
                const rapidjson::Value& element = array[i];

                CustomItem* child = parseJsonElement(element, model, self, depth + 1);
                child->setValue(0, "[" + QString::number(i) + "]");
                child->setType(element.GetType());
                self->addChild(child);
            }
        } break;
        case rapidjson::Type::kObjectType: {
            for (rapidjson::Value::ConstMemberIterator it = value.MemberBegin(); it != value.MemberEnd(); ++it) {
                const char* key = it->name.GetString();
                const rapidjson::Value& v = it->value;
                CustomItem* child = parseJsonElement(v, model, self, depth + 1);
                child->setValue(0, key);
                child->setType(v.GetType());
                self->addChild(child);
            }
        } break;
        default: {

        } break;
    }

    return self;
}
