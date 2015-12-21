//Project
#include "value.h"
#include "cgt/CGTShare.h"

//STL

//Qt


Value::Value(quintptr id_value, DataTypes type, const QVariant &value, const QString &name, DataTypes subType):
    m_id(id_value),
    m_type(type),
    m_value(value),
    m_name(name),
    m_subType(subType)
{

}

Value::Value(const QJsonObject &object)
{
    deserialize(object);
}

QVariantMap Value::serialize()
{
    QVariantMap data;
    data.insert("id", m_id);
    data.insert("name", m_name);
    data.insert("type", m_type);
    data.insert("subType", m_subType);

    switch (m_type) {
    case data_int:
    case data_color:
    case data_flags: {
        data.insert("value", m_value.toInt());
        break;
    }
    case data_real: {
        data.insert("value", m_value.toReal());
        break;
    }
    case data_data: {
        data.insert("value", m_value);
        break;
    }
    case data_icon:
    case data_stream:
    case data_bitmap:
    case data_jpeg:
    case data_wave: {
        data.insert("value", m_value.toByteArray().toHex());
        break;
    }
    case data_array: {
        QVariantList array;
        for (const SharedValue &v : m_value.value<Values>()) {
            array.append(v->serialize());
        }

        data.insert("ArrayValues", array);
        break;
    }
    case data_font: {
        const SharedValueFont font = m_value.value<SharedValueFont>();
        QVariantMap fontMap;
        fontMap.insert("name", font->name);
        fontMap.insert("size", font->size);
        fontMap.insert("style", font->style);
        fontMap.insert("color", font->color);
        fontMap.insert("charset", font->charset);

        data.insert("value", fontMap);
        break;
    }
    case data_element: {
        const SharedLinkedElementInfo info = m_value.value<SharedLinkedElementInfo>();
        QVariantMap infoMap;
        infoMap.insert("id", info->id);
        infoMap.insert("interface", info->interface);

        data.insert("value", infoMap);
        break;
    }
    default: {
        data.insert("value", m_value);
    }
    }

    return data;
}

void Value::deserialize(const QJsonObject &object)
{
    m_id = object["id"].toVariant().toUInt();
    m_type = DataTypes(object["type"].toInt());
    m_name = object["name"].toString();
    m_subType = DataTypes(object["subType"].toInt());

    switch (m_type) {
    case data_int:
    case data_color:
    case data_flags: {
        m_value = object["value"].toInt();
        break;
    }
    case data_real: {
        m_value = object["value"].toVariant().toReal();
        break;
    }
    case data_data: {
        const QVariant var = object["value"].toVariant();
        switch (m_subType) {
        case data_int:
            m_value = var.toInt();
            break;
        case data_str:
            m_value = var.toString();
            break;
        case data_real:
            m_value = var.toReal();
            break;
        default:
            m_value = var;
            break;
        }
        break;
    }
    case data_icon:
    case data_stream:
    case data_bitmap:
    case data_jpeg:
    case data_wave: {
        m_value = QByteArray::fromHex(object["value"].toVariant().toByteArray());
        break;
    }
    case data_array: {
        QJsonArray array = object["ArrayValues"].toArray();
        Values arrayItem;

        int arrCount = array.size();
        for (int i = 0; i < arrCount; ++i) {
            const QJsonObject item = array[i].toObject();

            QString name = item["name"].toString();
            QVariant data;
            switch (m_subType) {
            case data_int:
                data = item["value"].toInt();
                break;
            case data_str:
                data = item["value"].toString();
                break;
            case data_real:
                data = item["value"].toVariant().toReal();
                break;
            default: break;
            }

            arrayItem.append(SharedValue::create(0, m_subType, data, name));
        }

        m_value = QVariant::fromValue(arrayItem);
        break;
    }
    case data_font: {
        QJsonObject value = object["value"].toObject();
        SharedValueFont font = SharedValueFont::create();
        font->name = value["name"].toString();
        font->size = value["size"].toVariant().toUInt();
        font->style = value["style"].toVariant().value<uchar>();
        font->color = value["color"].toVariant().toUInt();
        font->charset = value["charset"].toVariant().value<uchar>();

        m_value = QVariant::fromValue(font);
        break;
    }
    case data_element: {
        QJsonObject value = object["value"].toObject();
        SharedLinkedElementInfo elementInfo = SharedLinkedElementInfo::create();
        elementInfo->id = value["id"].toVariant().toUInt();
        elementInfo->interface = value["id"].toString();

        m_value = QVariant::fromValue(elementInfo);
        break;
    }
    default: {
        m_value = object["value"].toVariant();
    }

    }
}

void Value::setId(quintptr id)
{
    m_id = id;
}

quintptr Value::getId() const
{
    return m_id;
}

void Value::setType(DataTypes type)
{
    m_type = type;
}

DataTypes Value::getType() const
{
    return m_type;
}

void Value::setName(const QString &name)
{
    m_name = name;
}

QString Value::getName() const
{
    return m_name;
}

void Value::setValue(const QVariant &value)
{
    m_value = value;
}

QVariant Value::getValue() const
{
    return m_value;
}

uchar Value::toByte() const
{
    if (!m_value.canConvert<uchar>())
        return uchar();

    return m_value.value<uchar>();
}

int Value::toInt() const
{
    if (!m_value.canConvert<int>())
        return int();

    return m_value.value<int>();
}

qreal Value::toReal() const
{
    if (!m_value.canConvert<qreal>())
        return qreal();

    return m_value.value<qreal>();
}

QString Value::toString() const
{
    if (!m_value.canConvert<QString>())
        return QString();

    return m_value.value<QString>();
}

DataTypes Value::getDataType() const
{
    return m_subType;
}

size_t Value::getArraySize() const
{
    if (!m_value.canConvert<Values>())
        return 0;

    return m_value.value<Values>().size();
}

void Value::setSubType(DataTypes type)
{
    m_subType = type;
}

DataTypes Value::getSubType() const
{
    return m_subType;
}

SharedValue Value::getArrayItemByIndex(uint index) const
{
    if (!m_value.canConvert<Values>())
        return SharedValue();

    const Values arrayValues = m_value.value<Values>();
    if (index < uint(arrayValues.size()))
        return arrayValues[index];

    return SharedValue();
}

QString Value::getArrayItemName(uint index) const
{
    const SharedValue arrValue = getArrayItemByIndex(index);
    if (!arrValue)
        return QString();

    return arrValue->getName();
}

SharedValueFont Value::toFont() const
{
    if (!m_value.canConvert<SharedValueFont>())
        return SharedValueFont();

    return m_value.value<SharedValueFont>();
}

const SharedLinkedElementInfo Value::toLinkedElementInfo() const
{
    if (!m_value.canConvert<SharedLinkedElementInfo>())
        return SharedLinkedElementInfo();

    return m_value.value<SharedLinkedElementInfo>();
}
