#pragma once

//Project
#include "types.h"

//STL

//Qt
#include <QObject>

class Container: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Container)

private:
    //Self
    quintptr m_id{};
    QString m_name;

    //Model
    PSceneModel const m_model;

    //Element
    Elements m_elements;

private:
    Q_PROPERTY(PSceneModel model READ getModel)

public:
    explicit Container(quintptr id_sdk, QObject *parent);
    explicit Container(const QJsonObject &object, QObject *parent);

private:
    void collectingData();

public:
    //Serialize
    QVariantMap serialize();
    void deserialize(const QJsonObject &object);

    //Self
    quintptr getId() const;
    PElement getParent() const;
    void setName(const QString &name);
    QString getName() const;

    //Model
    PSceneModel getModel() const;

    //Element
    size_t getCountElements() const;
    PElement getElementByIndex(uint index) const;
    quintptr getIdElementByIndex(uint index) const;
    PElement getElementByName(const QString &name) const;
    quintptr getIdElementByName(const QString &name) const;
    PElement addElement(PElement element);
    void removeElement(uint index);
};
