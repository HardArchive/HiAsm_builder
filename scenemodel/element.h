#pragma once

//Project
#include "types.h"
#include "cgt/CGTShare.h"
#include "package/confelement.h"

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
    ElementFlgs m_flags{};
    bool m_linkIs{};
    quintptr m_linkMain{};
    QString m_codeName;
    int m_posX{};
    int m_posY{};
    int m_sizeW{};
    int m_sizeH{};

    //CGT
    PCodeGenTools m_cgt{};

    //Model
    PSceneModel m_model{};

    //Conf
    SharedConfElement m_conf;

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
    explicit Element(const SharedConfElement &conf, quintptr id_element, QObject *parent);
    explicit Element(quintptr id_element, QObject *parent);
    explicit Element(const QJsonObject &object, QObject *parent);

private:
    void collectingData();
    void loadConf();

public:
    //Serialize
    QVariantMap serialize();
    void deserialize(const QJsonObject &object);

    //Self
    quintptr getId() const;
    PContainer getParent() const;

    QString getName() const;

    void setUserData(quintptr userData);
    quintptr getUserData() const;

    ElementClass getClassIndex();

    void setFlags(const ElementFlgs &flags);
    ElementFlags getFlags() const;

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

    QString getClassName() const;

    void setCodeName(const QString &name);
    QString getCodeName() const;

    QString getInterface() const;

    QString getInherit() const;

    QString getInfSub() const;

    //CGT
    PCodeGenTools getCgt();

    //Model
    PSceneModel getModel();

    //Container
    int getCountContainers() const;
    PContainer getContainer() const;
    quintptr getIdContainer() const;
    PContainer getContainerByIndex(uint index);
    quintptr getIdContainerByIndex(uint index);
    PContainer addContainer(PContainer container);
    void removeContainer(uint index);

    //Point
    int getCountPoints() const;
    PPoint getPointByIndex(uint index) const;
    quintptr getIdPointByIndex(uint index) const;
    PPoint getPointByName(const QString &name) const;
    quintptr getIdPointByName(const QString &name) const;
    PPoint addPoint(PPoint point);
    void removePoint(uint index);

    //Property
    int getCountProps() const;
    PProperty getPropertyByIndex(uint index) const;
    quintptr getIdPropertyByIndex(uint index) const;
    PProperty getPropertyById(quintptr id_prop) const;
    PProperty getPropertyByName(const QString &name) const;
    quintptr getIdPropertyByName(const QString &name) const;
    PProperty addProperty(PProperty property);
    void removeProperty(uint index);
    bool getIsDefPropByIndex(uint index);

};
