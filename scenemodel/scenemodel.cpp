//Project
#include "scenemodel.h"
#include "container.h"
#include "element.h"
#include "point.h"
#include "property.h"
#include "cgt/cgt.h"

//STL

//Native
#include <windows.h>

//Qt
#include <QDebug>

SceneModel::SceneModel(QObject *parent):
    QObject(parent)
{
    //ru Собираем данные о среде
    collectingData(cgt::getMainSDK());

    //ru Запуск процесса сборка данных о схеме
    m_container = new Container(cgt::getMainSDK(), this);
}

SceneModel::SceneModel(const QJsonDocument &doc, QObject *parent):
    QObject(parent)
{
    deserialize(doc);
}

SceneModel::~SceneModel()
{
    compileResources();
    deleteResources();
}

void SceneModel::collectingData(quintptr id_sdk)
{
    m_isDebug = cgt::isDebug(id_sdk);
    quintptr id_element = cgt::sdkGetElement(id_sdk, 0);

    //ru Тут мощная магия, однако;D
    int iBuf{};
    QByteArray buf("", 512);

    buf.fill('\0');
    reinterpret_cast<quintptr *>(buf.data())[0] = id_element;
    cgt::GetParam(PARAM_CODE_PATH, buf.data());
    m_cgtParams.CODE_PATH = QString::fromLocal8Bit(buf);

    cgt::GetParam(PARAM_DEBUG_MODE, &iBuf);
    m_cgtParams.DEBUG_MODE = iBuf;

    cgt::GetParam(PARAM_DEBUG_SERVER_PORT, &iBuf);
    m_cgtParams.DEBUG_SERVER_PORT = iBuf;

    cgt::GetParam(PARAM_DEBUG_CLIENT_PORT, &iBuf);
    m_cgtParams.DEBUG_CLIENT_PORT = iBuf;

    buf.fill('\0');
    reinterpret_cast<quintptr *>(buf.data())[0] = id_element;
    cgt::GetParam(PARAM_PROJECT_PATH, buf.data());
    m_cgtParams.PROJECT_PATH = QString::fromLocal8Bit(buf);

    const char f[] = "%mj.%mn.%bl";
    char *tmpBuf = new char[strlen(f) + 1];
    strcpy(tmpBuf, f);
    cgt::GetParam(PARAM_HIASM_VERSION, tmpBuf);
    m_cgtParams.HIASM_VERSION = QString::fromLatin1(tmpBuf);
    delete[] tmpBuf;

    buf.fill('\0');
    cgt::GetParam(PARAM_USER_NAME, buf.data());
    m_cgtParams.USER_NAME = QString::fromLocal8Bit(buf);

    buf.fill('\0');
    cgt::GetParam(PARAM_USER_MAIL, buf.data());
    m_cgtParams.USER_MAIL = QString::fromLocal8Bit(buf);

    buf.fill('\0');
    reinterpret_cast<quintptr *>(buf.data())[0] = id_element;
    cgt::GetParam(PARAM_PROJECT_NAME, buf.data());
    m_cgtParams.PROJECT_NAME = QString::fromLocal8Bit(buf);

    uint tmpW[1] = {reinterpret_cast<uint>(id_element)};
    cgt::GetParam(PARAM_SDE_WIDTH, tmpW);
    m_cgtParams.SDE_WIDTH = tmpW[0];

    uint tmpH[1] = {reinterpret_cast<uint>(id_element)};
    cgt::GetParam(PARAM_SDE_HEIGHT, tmpH);
    m_cgtParams.SDE_HEIGHT = tmpH[0];

    buf.fill('\0');
    reinterpret_cast<quintptr *>(buf.data())[0] = id_element;
    cgt::GetParam(PARAM_COMPILER, buf.data());
    m_cgtParams.COMPILER = QString::fromLocal8Bit(buf);
}

void SceneModel::deleteResources()
{
    for (const auto &filePath : m_resourcesToDelete) {
        QFile::remove(filePath);
    }
    m_resourcesToDelete.clear();
}

void SceneModel::compileResources()
{
    static const QString NAME_DIR_OUTPUT_RESOURCES = "compiler";
    static const QString NAME_FILE_RC = "allres.rc";
    static const QString NAME_FILE_RES = "allres.res";
    static const QString NAME_FILE_GORC = "GoRC.exe";
    QString current = QDir::currentPath();
    QString outputResPath = current + QDir::separator() + NAME_DIR_OUTPUT_RESOURCES;
    QDir::setCurrent(outputResPath);

    QFile file(NAME_FILE_RC);
    file.open(QIODevice::WriteOnly);
    QTextStream write(&file);

    for (const auto &nameRes : m_resourcesForCompile.keys()) {
        write << QString("%1 %2 %1.dat\r\n").arg(nameRes).arg(m_resourcesForCompile[nameRes]);
    }

    write << "ASMA ICON \"..\\int\\main.ico\"";
    file.close();

    //QProcess::execute(QString("%1 /r %2").arg(NAME_FILE_GORC).arg(NAME_FILE_RC));

    //QFile::copy(NAME_FILE_RES, m_cgtParams.CODE_PATH + QDir::separator() + NAME_FILE_RES);

    QDir::setCurrent(current);
}

QJsonDocument SceneModel::serialize()
{
    QVariantMap cgtParams;
    cgtParams.insert("CODE_PATH", m_cgtParams.CODE_PATH);
    cgtParams.insert("DEBUG_MODE", m_cgtParams.DEBUG_MODE);
    cgtParams.insert("DEBUG_SERVER_PORT", m_cgtParams.DEBUG_SERVER_PORT);
    cgtParams.insert("DEBUG_CLIENT_PORT", m_cgtParams.DEBUG_CLIENT_PORT);
    cgtParams.insert("PROJECT_PATH", m_cgtParams.PROJECT_PATH);
    cgtParams.insert("HIASM_VERSION", m_cgtParams.HIASM_VERSION);
    cgtParams.insert("USER_NAME", m_cgtParams.USER_NAME);
    cgtParams.insert("USER_MAIL", m_cgtParams.USER_MAIL);
    cgtParams.insert("PROJECT_NAME", m_cgtParams.PROJECT_NAME);
    cgtParams.insert("SDE_WIDTH", m_cgtParams.SDE_WIDTH);
    cgtParams.insert("SDE_HEIGHT", m_cgtParams.SDE_HEIGHT);
    cgtParams.insert("COMPILER", m_cgtParams.COMPILER);

    QVariantMap model;
    model.insert("CGTParams", cgtParams);
    model.insert("Container", m_container->serialize());

    return QJsonDocument::fromVariant(model);
}

void SceneModel::deserialize(const QJsonDocument &doc)
{
    const QJsonObject model = doc.object();

    const QJsonObject cgtParams = model["CGTParams"].toObject();
    m_cgtParams.CODE_PATH = cgtParams["CODE_PATH"].toString();
    m_cgtParams.DEBUG_MODE = cgtParams["DEBUG_MODE"].toInt();
    m_cgtParams.DEBUG_SERVER_PORT = cgtParams["DEBUG_SERVER_PORT"].toInt();
    m_cgtParams.DEBUG_CLIENT_PORT = cgtParams["DEBUG_CLIENT_PORT"].toInt();;
    m_cgtParams.PROJECT_PATH = cgtParams["PROJECT_PATH"].toString();
    m_cgtParams.HIASM_VERSION = cgtParams["HIASM_VERSION"].toString();
    m_cgtParams.USER_NAME = cgtParams["USER_NAME"].toString();
    m_cgtParams.USER_MAIL = cgtParams["USER_MAIL"].toString();
    m_cgtParams.PROJECT_NAME = cgtParams["PROJECT_NAME"].toString();
    m_cgtParams.SDE_WIDTH = cgtParams["SDE_WIDTH"].toInt();
    m_cgtParams.SDE_HEIGHT = cgtParams["SDE_HEIGHT"].toInt();
    m_cgtParams.COMPILER = cgtParams["COMPILER"].toString();

    QJsonObject container = model["Container"].toObject();
    m_container = new Container(container, this);
}

PSceneModel SceneModel::getModel()
{
    return this;
}

void SceneModel::save()
{
    QJsonDocument doc = serialize();

    QFile file("test.json");
    file.open(QIODevice::WriteOnly);
    file.write(doc.toJson());
}

void SceneModel::addContainerToMap(PContainer id_sdk)
{
    if (id_sdk)
        m_mapContainers.insert(id_sdk->getId(), id_sdk);
}

void SceneModel::addElementToMap(PElement id_element)
{
    if (id_element)
        m_mapElements.insert(id_element->getId(), id_element);
}

void SceneModel::addPropertyToMap(PProperty id_prop)
{
    if (id_prop)
        m_mapProperties.insert(id_prop->getId(), id_prop);
}

void SceneModel::addPointToMap(PPoint id_point)
{
    if (id_point)
        m_mapPoints.insert(id_point->getId(), id_point);
}

void SceneModel::addValueToMap(PValue value)
{
    if (value)
        m_mapValues.insert(value->getId(), value);
}

PContainer SceneModel::getContainerById(quintptr id_sdk) const
{
    return m_mapContainers[id_sdk];
}

size_t SceneModel::getCountElementsInContainer(quintptr id_sdk) const
{
    const PContainer c = getContainerById(id_sdk);
    if (!c)
        return 0;

    return c->getCountElements();
}

quintptr SceneModel::getIdRootContainer() const
{
    if(!m_container)
        return 0;

    return m_container->getId();
}

PElement SceneModel::getElementById(quintptr id_element) const
{
    return m_mapElements[id_element];
}

PElement SceneModel::getElementFromSDKByIndex(quintptr id_sdk, int index) const
{
    const PContainer c = getContainerById(id_sdk);
    if (!c)
        return nullptr;
    return c->getElementByIndex(index);
}

quintptr SceneModel::getIdElementFromSDKByIndex(quintptr id_sdk, int index) const
{
    const PContainer c = getContainerById(id_sdk);
    if (!c)
        return 0;
    return c->getIdElementByIndex(index);
}

PProperty SceneModel::getPropertyById(quintptr id_prop) const
{
    return m_mapProperties[id_prop];
}

void SceneModel::setPropArrayValue(const SharedValue &value)
{
    m_propArrayValue = value;
}

const SharedValue &SceneModel::getPropArrayValue()
{
    return m_propArrayValue;
}

PPoint SceneModel::getPointById(quintptr id_point) const
{
    return m_mapPoints[id_point];
}

PValue SceneModel::getValueById(quintptr id_value) const
{
    return m_mapValues[id_value];
}

const char *SceneModel::addStreamRes(quintptr id_prop)
{
    PProperty p = getPropertyById(id_prop);
    if (!p)
        return nullptr;

    const PValue v = p->getValue();
    if (p->getType() == data_icon)
        if (v->getValue().isNull())
            return fcgt::strToPChar(QString("ASMA"));

    const QByteArray byteArray = v->getValue().toByteArray();
    static const QString nameDir = "compiler";
    static const QString name = "STREAM";
    static const QString ext = ".dat";
    QString suffix = QString::number(m_resourcesForCompile.size());
    QString fileName = name + suffix;

    QString resFilePath = QDir::toNativeSeparators(
                              QDir::currentPath() + QDir::separator() +
                              nameDir + QDir::separator() + fileName + ext
                          );
    QFile file(resFilePath);
    if (!file.open(QIODevice::WriteOnly))
        return nullptr;

    file.write(byteArray);
    file.close();
    m_resourcesToDelete.insert(resFilePath);
    m_resourcesForCompile.insert(fileName, "100");

    return fcgt::strToPChar(fileName);
}

const char *SceneModel::addStringRes(const QString &str)
{
    if (str.isEmpty())
        return nullptr;

    static const QString nameCompilerDir = "compiler";
    static const QString nameFileRes = "STREAM";
    QString suffix = QString::number(m_resourcesToDelete.size());
    QString fileName = nameFileRes + suffix;
    QString resFilePath = QDir::toNativeSeparators(
                              QDir::currentPath() + QDir::separator() +
                              nameCompilerDir + QDir::separator() + fileName
                          );
    QFile file(resFilePath);
    if (!file.open(QIODevice::WriteOnly))
        return nullptr;

    file.write(str.toLocal8Bit());
    file.close();
    m_resourcesToDelete.insert(resFilePath);


    return fcgt::strToPChar(fileName);
}

int SceneModel::addResList(const QString &filePath)
{
    m_resourcesToDelete.insert(filePath);
    return 0;
}

bool SceneModel::resIsEmpty() const
{
    return m_resourcesToDelete.isEmpty();
}

bool SceneModel::isDebug() const
{
    return m_isDebug;
}

void SceneModel::getCgtParam(CgtParams index, void *buf) const
{
    auto writeString = [buf](const QString & str) {
        strcpy(reinterpret_cast<char *>(buf), str.toStdString().c_str());
    };
    auto writeInt = [buf](int value) {
        *reinterpret_cast<int *>(buf) = value;
    };

    switch (index) {
    case PARAM_CODE_PATH :
        writeString(m_cgtParams.CODE_PATH);
        break;
    case PARAM_DEBUG_MODE:
        writeInt(m_cgtParams.DEBUG_MODE);
        break;
    case PARAM_DEBUG_SERVER_PORT:
        writeInt(m_cgtParams.DEBUG_SERVER_PORT);
        break;
    case PARAM_DEBUG_CLIENT_PORT:
        writeInt(m_cgtParams.DEBUG_CLIENT_PORT);
        break;
    case PARAM_PROJECT_PATH:
        writeString(m_cgtParams.PROJECT_PATH);
        break;
    case PARAM_HIASM_VERSION:
        writeString(m_cgtParams.HIASM_VERSION);
        break;
    case PARAM_USER_NAME:
        writeString(m_cgtParams.USER_NAME);
        break;
    case PARAM_USER_MAIL:
        writeString(m_cgtParams.USER_MAIL);
        break;
    case PARAM_PROJECT_NAME:
        writeString(m_cgtParams.PROJECT_NAME);
        break;
    case PARAM_SDE_WIDTH:
        writeInt(m_cgtParams.SDE_WIDTH);
        break;
    case PARAM_SDE_HEIGHT:
        writeInt(m_cgtParams.SDE_HEIGHT);
        break;
    case PARAM_COMPILER:
        writeString(m_cgtParams.COMPILER);
        break;
    }
}

