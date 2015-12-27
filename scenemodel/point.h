#pragma once

//Project
#include "types.h"
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>
#include <QDebug>

class Point: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Point)

private:
    //Self
    quintptr m_id{};
    PointType m_type{};
    DataType m_dataType{};
    uint m_index{};
    QString m_name;
    QString m_dpeName;
    QString m_info;
    quintptr m_linkPoint{};
    quintptr m_RLinkPoint{};

    //CGT
    PCodeGenTools m_cgt{};

    //Model
    PSceneModel m_model{};

private:
    Q_PROPERTY(PSceneModel model READ getModel)
    Q_PROPERTY(PCodeGenTools cgt READ getCgt)

public:
    explicit Point(quintptr id_point, QObject *parent);
    explicit Point(const QJsonObject &object, QObject *parent);

private:
    void collectingData();

public:
    //Serialize
    QVariantMap serialize();
    void deserialize(const QJsonObject &object);

    //Self
    quintptr getId() const;
    PElement getParent() const;

    void setType(PointType type);
    PointType getType() const;

    void setDataType(DataType dataType);
    DataType getDataType() const;

    void setIndex(uint index);
    uint getIndex() const;

    void setName(const QString &name);
    QString getName() const;

    void setDpeName(const QString &dpeName);
    QString getDpeName() const;

    void setInfo(const QString &info);
    QString getInfo() const;

    void setLinkPoint(quintptr linkPoint);
    quintptr getLinkPoint() const;

    void setRLinkPoint(quintptr RLinkPoint);
    quintptr getRLinkPoint() const;

    //CGT
    PCodeGenTools getCgt();

    //Model
    PSceneModel getModel();
};
