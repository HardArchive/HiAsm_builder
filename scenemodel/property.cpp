//Project
#include "property.h"
#include "element.h"
#include "valuetypes.h"
#include "scenemodel.h"
#include "cgt/cgt.h"

//STL

//Qt
#include <QTemporaryFile>
#include <QDebug>
#include <QUuid>

Property::Property(quintptr id_prop, QObject *parent)
    : QObject(parent)
    , m_id(id_prop)
    , m_cgt(parent->property("cgt").value<PCodeGenTools>())
    , m_model(parent->property("model").value<PSceneModel>())
{
    m_model->addPropertyToMap(this);
    collectingData();
}

Property::Property(const QJsonObject &object, QObject *parent)
    : QObject(parent)
    , m_model(parent->property("model").value<PSceneModel>())
{
    deserialize(object);
}

Property::Property(quintptr id, DataType type, const QVariant &data, const QString &name)
{
    m_id = id;
    m_type = type;
    m_name = name;
    m_value.setId(id);
    m_value.setType(type);
    m_value.setValue(data);
    m_value.setName(name);
}

void Property::collectingData()
{
    m_name = QString::fromLocal8Bit(m_cgt->propGetName(m_id));
    m_type = m_cgt->propGetType(m_id);
    quintptr id_value = m_cgt->propGetValue(m_id);

    switch (m_type) {
    case data_int:
    case data_color:
    case data_flags: {
        setValue(id_value, m_type, m_cgt->propToInteger(m_id));
        break;
    }
    case data_real: {
        setValue(id_value, m_type, m_cgt->propToReal(m_id));
        break;
    }
    case data_str:
    case data_comboEx:
    case data_list:
    case data_script:
    case data_code: {
        setValue(id_value, m_type, QString::fromLocal8Bit(m_cgt->propToString(m_id)));
        break;
    }
    case data_data: {
        const DataType dataType = m_cgt->dtType(id_value);
        switch (dataType) {
        case data_int:
            setValue(id_value, m_type, m_cgt->dtInt(id_value), QString(), dataType);
            break;
        case data_str:
            setValue(id_value, m_type, m_cgt->dtStr(id_value), QString(), dataType);
            break;
        case data_real:
            setValue(id_value, m_type, m_cgt->dtReal(id_value), QString(), dataType);
            break;
        default:
            setValue(id_value, m_type);
            break;
        }
        break;
    }
    case data_combo: {
        setValue(id_value, m_type, m_cgt->propToByte(m_id));
        break;
    }
    case data_icon: {
        if (!strcmp(m_cgt->resAddIcon(m_id), "ASMA")) {
            break;
        }
    }
    case data_stream:
    case data_bitmap:
    case data_jpeg:
    case data_wave: {
        static QString nameRandom = QUuid::createUuid().toString() + ".wtf";
        static QString filePath = QDir::toNativeSeparators(QDir::tempPath() + QDir::separator() + nameRandom);

        m_cgt->propSaveToFile(m_id, filePath.toStdString().data());
        QFile file(filePath);
        if (file.size()) {
            file.open(QIODevice::ReadOnly);
            setValue(id_value, m_type, file.readAll());
            file.close();
        }
        file.remove();

        break;
    }
    case data_array: {
        int arrCount = m_cgt->arrCount(id_value);
        DataType arrItemType = m_cgt->arrType(id_value);
        Values arrayItems;

        for (int i = 0; i < arrCount; ++i) {
            const quintptr id_prop = m_cgt->arrGetItem(id_value, i);

            QString name = QString::fromLocal8Bit(m_cgt->arrItemName(id_value, i));
            QVariant data;
            switch (arrItemType) {
            case data_int:
                data = m_cgt->propToInteger(id_prop);
                break;
            case data_str:
                data = QString::fromLocal8Bit(m_cgt->propToString(id_prop));
                break;
            case data_real:
                data = m_cgt->propToReal(id_prop);
                break;
            default: break;
            }

            arrayItems.append(SharedValue::create(1, arrItemType, data, name));
        }

        setValue(id_value, m_type, QVariant::fromValue(arrayItems), QString(), arrItemType);
        break;
    }
    case data_font: {
        SharedValueFont font = SharedValueFont::create();
        font->name = QString::fromLocal8Bit(m_cgt->fntName(id_value));
        font->size = m_cgt->fntSize(id_value);
        font->style = m_cgt->fntStyle(id_value);
        font->color = m_cgt->fntColor(id_value);
        font->charset = m_cgt->fntCharSet(id_value);

        setValue(id_value, m_type, QVariant::fromValue(font));
        break;
    }
    case data_element: {
        const PElement e = qobject_cast<PElement>(parent());
        if (!e)
            return;

        char buf[PATH_MAX];
        quintptr linkedElement = m_cgt->propGetLinkedElementInfo(e->getId(), m_id, buf);
        if (linkedElement) {
            SharedLinkedElementInfo elementInfo = SharedLinkedElementInfo::create();
            elementInfo->id = linkedElement;
            elementInfo->interface = QString::fromLocal8Bit(buf);

            setValue(id_value, m_type, QVariant::fromValue(elementInfo));
        }
        break;
    }
    default: break;
    }
}

QVariantMap Property::serialize()
{
    QVariantMap data;
    data.insert("id", m_id);
    data.insert("name", m_name);
    data.insert("type", m_type);
    data.insert("isDefProp", m_isDefProp);
    data.insert("value", m_value.serialize());

    return data;
}

void Property::deserialize(const QJsonObject &object)
{
    m_id = object["id"].toVariant().value<quintptr>();
    m_model->addPropertyToMap(this);

    m_name = object["name"].toString();
    m_type = DataType(object["type"].toInt());
    m_isDefProp = object["isDefProp"].toBool();
    m_value.deserialize(object["value"].toObject());
    m_model->addValueToMap(&m_value);
}

quintptr Property::getId() const
{
    return m_id;
}

void Property::setName(const QString &name)
{
    m_name = name;
}

QString Property::getName() const
{
    return m_name;
}

void Property::setType(DataType type)
{
    m_type = type;
}

DataType Property::getType() const
{
    return m_type;
}

void Property::setIsDefProp(bool value)
{
    m_isDefProp = value;
}

bool Property::getIsDefProp() const
{
    return m_isDefProp;
}

void Property::setValue(quintptr id, DataType type, const QVariant &data, const QString &name, DataType arrayType)
{
    m_value.setId(id);
    m_value.setType(type);
    m_value.setValue(data);
    m_value.setName(name);
    m_value.setSubType(arrayType);
    m_model->addValueToMap(&m_value);
}

PValue Property::getValue()
{
    return &m_value;
}

uchar Property::toByte() const
{
    return m_value.toByte();
}

int Property::toInt() const
{
    return m_value.toInt();
}

qreal Property::toReal() const
{
    return m_value.toReal();
}

QString Property::toString() const
{
    return m_value.toString();
}

const SharedLinkedElementInfo Property::toLinkedElementInfo() const
{
    return m_value.toLinkedElementInfo();
}

PCodeGenTools Property::getCgt()
{
    return m_cgt;
}

PSceneModel Property::getModel()
{
    return m_model;
}
