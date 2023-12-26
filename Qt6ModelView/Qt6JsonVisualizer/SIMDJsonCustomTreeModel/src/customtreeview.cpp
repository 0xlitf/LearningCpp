#include <QItemDelegate>
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QFileInfo>
#include <QResource>
#include <QThread>
#include "customtreeview.h"

SIMDJsonTreeView::SIMDJsonTreeView(QWidget* parent)
    : QTreeView(parent) {

}

void SIMDJsonTreeView::test() {

}

void SIMDJsonTreeView::parseJsonFile(const char* filename, CustomItemModel* model) {
    QFile file(filename);

    simdjson::ondemand::parser parser;
    if (file.open(QIODevice::ReadOnly)) {
        auto json = simdjson::padded_string::load(filename);
        simdjson::ondemand::document doc = parser.iterate(json);
        simdjson::ondemand::value value = doc;
        CustomItem* rootItem = nullptr;

        if (value.type() == simdjson::ondemand::json_type::array) {
            rootItem = parseJsonElement(value, m_model);
            rootItem->setType(simdjson::ondemand::json_type::array);
        } else if (value.type() == simdjson::ondemand::json_type::object) {
            rootItem = parseJsonElement(value, m_model);
            rootItem->setType(simdjson::ondemand::json_type::object);
        }

        model->setRootItem(rootItem);

        file.close();
        qDebug() << "Json parsing success.";
    } else {
        qDebug() << "Json parsing failed.";
    }
}

void SIMDJsonTreeView::openJson() {
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

void SIMDJsonTreeView::writeJson()
{

}

CustomItem* SIMDJsonTreeView::parseJsonElement(simdjson::ondemand::value& value, CustomItemModel* model, CustomItem* parent, int depth) {
    CustomItem* self = new CustomItem(parent);
    if (parent) {

    }

    simdjson::ondemand::json_type type = value.type();

    switch (type) {
        case simdjson::ondemand::json_type::boolean: {
            bool v = value.get_bool();
            self->setValue(1, v);
        } break;
        case simdjson::ondemand::json_type::null: {
            self->setValue(1, QVariant());
        } break;
        case simdjson::ondemand::json_type::string: {
            std::string_view v = value.get_string();
            QString qstr = QString::fromUtf8(v.data(), static_cast<int>(v.length()));
            self->setValue(1, QVariant(qstr));
        } break;
        case simdjson::ondemand::json_type::number: {
            simdjson::fallback::number_type numberType = value.get_number_type();
            switch (numberType) {
                case simdjson::fallback::number_type::signed_integer: {
                    int64_t v = value.get_int64();
                    self->setValue(1, v);
                } break;
                case simdjson::fallback::number_type::unsigned_integer: {
                    uint64_t v = value.get_uint64();
                    self->setValue(1, v);
                } break;
                case simdjson::fallback::number_type::floating_point_number: {
                    double v = value.get_double();
                    self->setValue(1, v);
                } break;
                default: {
                } break;
            }

            self->setType(value.type());
            self->setNumberType(numberType);
        } break;
        case simdjson::ondemand::json_type::array: {
            auto array = value.get_array();

            int i = 0;
            for (simdjson::ondemand::value value : array) {
                CustomItem* child = parseJsonElement(value, model, self, depth + 1);
                child->setValue(0, "[" + QString::number(i) + "]");
                child->setType(value.type());
                self->addChild(child);

                ++i;
            }
        } break;
        case simdjson::ondemand::json_type::object: {
            simdjson::ondemand::object obj = value.get_object();

            for (simdjson::ondemand::field field : obj) {
                auto key = field.key();
                QString qstr = QString::fromUtf8(key.raw());
                simdjson::ondemand::value v = field.value();

                CustomItem* child = parseJsonElement(v, model, self, depth + 1);
                child->setValue(0, qstr);
                child->setType(v.type());
                self->addChild(child);
            }
        } break;
        default: {

        } break;
    }

    return self;
}
