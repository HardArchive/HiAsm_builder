#pragma once

//Project
#include "types.h"
#include "value.h"
#include "cgt/CGTShare.h"

//STL

//Qt
#include <QObject>
#include <QSet>

class SceneModel: public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SceneModel)

private:
    //Map
    MapContainers m_mapContainers;
    MapElements m_mapElements;
    MapProperties m_mapProperties;
    MapPoints m_mapPoints;
    MapValues m_mapValues;

    //Container
    PContainer m_container{};

    //Property
    SharedValue m_propArrayValue;

    //Resource
    QSet<QString> m_resourcesToDelete;
    QMap<QString, QString> m_resourcesForCompile;

    //Параметры CGT
    bool m_isDebug{};
    TCgtParams m_cgtParams;

private:
    Q_PROPERTY(PSceneModel model READ getModel)

public:
    explicit SceneModel(QObject *parent = 0);
    explicit SceneModel(const QJsonDocument &doc, QObject *parent = 0);
    virtual ~SceneModel();

private:
    void collectingData(quintptr id_sdk);
    void deleteResources();
    void compileResources();

public:
    //Serialize
    QJsonDocument serialize();
    void deserialize(const QJsonDocument &doc);

    //Model
    PSceneModel getModel();
    void save();

    //Map
    void addContainerToMap(PContainer id_sdk);
    void addElementToMap(PElement id_element);
    void addPropertyToMap(PProperty id_prop);
    void addPointToMap(PPoint id_point);
    void addValueToMap(PValue value);

    //Container
    PContainer getContainerById(quintptr id_sdk) const;
    size_t getCountElementsInContainer(quintptr id_sdk) const;

    //Element
    PElement getElementById(quintptr id_element) const;
    PElement getElementFromSDKByIndex(quintptr id_sdk, int index) const;
    quintptr getIdElementFromSDKByIndex(quintptr id_sdk, int index) const;

    //Property
    PProperty getPropertyById(quintptr id_prop) const;
    void setPropArrayValue(const SharedValue &value);
    const SharedValue &getPropArrayValue();

    //Point
    PPoint getPointById(quintptr id_point) const;

    //Value
    PValue getValueById(quintptr id_value) const;

    //Resource
    const char *addStreamRes(quintptr id_prop);
    const char *addStringRes(const QString &str);

    int addResList(const QString &filePath);
    bool resIsEmpty() const;

    //Параметры CGT
    bool isDebug() const;
    void getCgtParam(CgtParams index, void *buf) const;
};

Q_DECLARE_METATYPE(PSceneModel)
