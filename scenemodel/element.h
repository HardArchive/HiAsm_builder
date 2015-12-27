#pragma once

//Project
#include "types.h"
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>
#include <QString>

class Element: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Element)

private:
    //Self
    quintptr m_id{};
    quintptr m_userData{};
    ElementClass m_classIndex{};
    ElementFlgs m_flags{};
    int m_group{};
    bool m_linkIs{};
    quintptr m_linkMain{};
    int m_posX{};
    int m_posY{};
    int m_sizeW{};
    int m_sizeH{};
    QString m_className;
    QString m_codeName;
    QString m_interface;
    QString m_inherit;
    QString m_infSub;

    //CGT
    PCodeGenTools m_cgt{};

    //Model
    PSceneModel m_model{};

    //Container
    Containers m_containers;

    //Point
    Points m_points;

    //Property
    Properties m_properties;

private:
    Q_PROPERTY(PSceneModel model READ getModel)
    Q_PROPERTY(PCodeGenTools cgt READ getCgt)

public:
    explicit Element(quintptr id_element, QObject *parent);
    explicit Element(const QJsonObject &object, QObject *parent);

private:
    void collectingData();

public:
    //Serialize
    QVariantMap serialize();
    void deserialize(const QJsonObject &object);

    //Self
    quintptr getId() const;
    PContainer getParent() const;

    void setUserData(quintptr userData);
    quintptr getUserData() const;

    void setClassIndex(ElementClass classIndex);
    ElementClass getClassIndex();

    void setFlags(const ElementFlgs &flags);
    ElementFlags getFlags() const;

    void setGroup(int group);
    int getGroup() const;

    void setLinkIs(bool linkIs);
    bool getLinkIs() const;

    void setLinkMain(quintptr linkMain);
    quintptr getLinkMain() const;

    void setPosX(int posX);
    int getPosX() const;

    void setPosY(int posY);
    int getPosY() const;

    void setSizeW(int sizeW);
    int getSizeW() const;

    void setSizeH(int sizeH);
    int getSizeH() const;

    void setClassName(const QString &className);
    QString getClassName() const;

    void setCodeName(const QString &name);
    QString getCodeName() const;

    void setInterface(const QString &interface);
    QString getInterface() const;

    void setInherit(const QString &inherit);
    QString getInherit() const;

    void setInfSub(const QString &infSub);
    QString getInfSub() const;

    //CGT
    PCodeGenTools getCgt();

    //Model
    PSceneModel getModel();

    //Container
    size_t getCountContainers() const;
    PContainer getContainer() const;
    quintptr getIdContainer() const;
    PContainer getContainerByIndex(uint index);
    quintptr getIdContainerByIndex(uint index);
    PContainer addContainer(PContainer container);
    void removeContainer(uint index);

    //Point
    size_t getCountPoints() const;
    PPoint getPointByIndex(uint index) const;
    quintptr getIdPointByIndex(uint index) const;
    PPoint getPointByName(const QString &name) const;
    quintptr getIdPointByName(const QString &name) const;
    PPoint addPoint(PPoint point);
    void removePoint(uint index);

    //Property
    size_t getCountProps() const;
    PProperty getPropertyByIndex(uint index) const;
    quintptr getIdPropertyByIndex(uint index) const;
    PProperty getPropertyById(quintptr id_prop) const;
    PProperty getPropertyByName(const QString &name) const;
    quintptr getIdPropertyByName(const QString &name) const;
    PProperty addProperty(PProperty property);
    void removeProperty(uint index);
};
