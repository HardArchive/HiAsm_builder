#pragma once
//Project
#include "valuetypes.h"
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>
#include <QVariant>

class Value
{

private:
    //Self
    quintptr m_id;
    DataTypes m_type = data_null;
    QVariant m_value;
    QString m_name;
    DataTypes m_subType = data_null;

public:
    explicit Value(quintptr id_value = 0,
                   DataTypes type = data_null,
                   const QVariant &value = QVariant(),
                   const QString &name = QString(),
                   DataTypes subType = data_null);

    explicit Value(const QJsonObject &object);


public:
    //Serialize
    QVariantMap serialize();
    void deserialize(const QJsonObject &object);

    //Self
    void setId(quintptr id);
    quintptr getId() const;
    void setType(DataTypes type);
    DataTypes getType() const;
    void setName(const QString &name);
    QString getName() const;

    //Value
    void setValue(const QVariant &value);
    QVariant getValue() const;

    //Byte
    uchar toByte() const;

    //Int
    int toInt() const;

    //Real
    qreal toReal() const;

    //String
    QString toString() const;

    //SubType
    void setSubType(DataTypes type);
    DataTypes getSubType() const;

    //Data
    DataTypes getDataType() const;

    //Array
    size_t getArraySize() const;
    SharedValue getArrayItemByIndex(uint index) const;
    QString getArrayItemName(uint index) const;

    //Font
    SharedValueFont toFont() const;

    //LinkedElementInfo
    const SharedLinkedElementInfo toLinkedElementInfo() const;
};

Q_DECLARE_METATYPE(Values)
