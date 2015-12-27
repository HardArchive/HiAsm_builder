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
    //CGT
    PCodeGenTools m_cgt{};

    //Map
    MapContainers m_mapContainers;
    MapElements m_mapElements;
    MapProperties m_mapProperties;
    MapPoints m_mapPoints;
    MapValues m_mapValues;

    //Container
    PContainer m_container{};

    //Resource
    QSet<QString> m_resourcesToDelete;
    QMap<QString, QString> m_resourcesForCompile;

    //Параметры CGT
    bool m_isDebug{};
    int m_debugMode{};
    int m_debugServerPort{};
    int m_debugClientPort{};
    int m_sdeWidth{};
    int m_sdeHeight{};
    QString m_codePath;
    QString m_projectPath;
    QString m_hiasmVersion;
    QString m_userName;
    QString m_userMail;
    QString m_projectName;
    QString m_compiler;

private:
    Q_PROPERTY(PCodeGenTools cgt READ getCgt)
    Q_PROPERTY(PSceneModel model READ getModel)

public:
    explicit SceneModel(QObject *parent = 0);
    virtual ~SceneModel();

private:
    void collectingData(quintptr id_sdk);

public:
    //Serialize
    QJsonDocument serialize();
    void deserialize(const QJsonDocument &doc);

    //CGT
    PCodeGenTools getCgt();

    //Model
    PSceneModel getModel();
    void initFromCgt(PCodeGenTools cgt, quintptr idMainSDK);
    bool saveModel(const QString &filePath);
    bool loadModel(const QString &filePath);

    //Map
    void addContainerToMap(PContainer id_sdk);
    void addElementToMap(PElement id_element);
    void addPropertyToMap(PProperty id_prop);
    void addPointToMap(PPoint id_point);
    void addValueToMap(PValue value);

    //Container
    PContainer getContainerById(quintptr id_sdk) const;
    size_t getCountElementsInContainer(quintptr id_sdk) const;
    quintptr getIdRootContainer() const;

    //Element
    PElement getElementById(quintptr id_element) const;
    PElement getElementFromSDKByIndex(quintptr id_sdk, int index) const;
    quintptr getIdElementFromSDKByIndex(quintptr id_sdk, int index) const;

    //Property
    PProperty getPropertyById(quintptr id_prop) const;

    //Point
    PPoint getPointById(quintptr id_point) const;

    //Value
    PValue getValueById(quintptr id_value) const;

    //Resource
    const char *addStreamRes(quintptr id_prop);
    const char *addStringRes(const QString &str);
    void deleteResources();
    void compileResources();

    int addResList(const QString &filePath);
    bool resIsEmpty() const;

    //Параметры CGT
    void getCgtParam(CgtParams index, void *buf) const;

    bool getIsDebug() const;
    void setIsDebug(bool isDebug);

    int getDebugMode() const;
    void setDebugMode(int debugMode);

    int getDebugServerPort() const;
    void setDebugServerPort(int debugServerPort);

    int getDebugClientPort() const;
    void setDebugClientPort(int debugClientPort);

    int getSdeWidth() const;
    void setSdeWidth(int sdeWidth);

    int getSdeHeight() const;
    void setSdeHeight(int sdeHeight);

    QString getCodePath() const;
    void setCodePath(const QString &codePath);

    QString getProjectPath() const;
    void setProjectPath(const QString &projectPath);

    QString getHiasmVersion() const;
    void setHiasmVersion(const QString &hiasmVersion);

    QString getUserName() const;
    void setUserName(const QString &userName);

    QString getUserMail() const;
    void setUserMail(const QString &userMail);

    QString getProjectName() const;
    void setProjectName(const QString &projectName);

    QString getCompiler() const;
    void setCompiler(const QString &compiler);
};

Q_DECLARE_METATYPE(PSceneModel)
