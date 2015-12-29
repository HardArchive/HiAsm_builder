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

SceneModel::SceneModel(QObject *parent)
    : QObject(parent)
{

}

SceneModel::~SceneModel()
{
    compileResources();
    deleteResources();
}

void SceneModel::collectingData(quintptr id_sdk)
{
    m_isDebug = m_cgt->isDebug(id_sdk);
    quintptr id_element = m_cgt->sdkGetElement(id_sdk, 0);

    //ru Тут мощная магия, однако;D
    int iBuf{};
    QByteArray buf("", 512);

    buf.fill('\0');
    reinterpret_cast<quintptr *>(buf.data())[0] = id_element; //-V206
    m_cgt->GetParam(PARAM_CODE_PATH, buf.data());
    m_codePath = QString::fromLocal8Bit(buf);

    m_cgt->GetParam(PARAM_DEBUG_MODE, &iBuf);
    m_debugMode = iBuf;

    m_cgt->GetParam(PARAM_DEBUG_SERVER_PORT, &iBuf);
    m_debugServerPort = iBuf;

    m_cgt->GetParam(PARAM_DEBUG_CLIENT_PORT, &iBuf);
    m_debugClientPort = iBuf;

    buf.fill('\0');
    reinterpret_cast<quintptr *>(buf.data())[0] = id_element; //-V206
    m_cgt->GetParam(PARAM_PROJECT_PATH, buf.data());
    m_projectPath = QString::fromLocal8Bit(buf);

    const char f[] = "%mj.%mn.%bl";
    char *tmpBuf = new char[strlen(f) + 1];
    strcpy(tmpBuf, f);
    m_cgt->GetParam(PARAM_HIASM_VERSION, tmpBuf);
    m_hiasmVersion = QString::fromLatin1(tmpBuf);
    delete[] tmpBuf;

    buf.fill('\0');
    m_cgt->GetParam(PARAM_USER_NAME, buf.data());
    m_userName = QString::fromLocal8Bit(buf);

    buf.fill('\0');
    m_cgt->GetParam(PARAM_USER_MAIL, buf.data());
    m_userMail = QString::fromLocal8Bit(buf);

    buf.fill('\0');
    reinterpret_cast<quintptr *>(buf.data())[0] = id_element; //-V206
    m_cgt->GetParam(PARAM_PROJECT_NAME, buf.data());
    m_projectName = QString::fromLocal8Bit(buf);

    uint tmpW[1] = {reinterpret_cast<uint>(id_element)};
    m_cgt->GetParam(PARAM_SDE_WIDTH, tmpW);
    m_sdeWidth = tmpW[0];

    uint tmpH[1] = {reinterpret_cast<uint>(id_element)};
    m_cgt->GetParam(PARAM_SDE_HEIGHT, tmpH);
    m_sdeHeight = tmpH[0];

    buf.fill('\0');
    reinterpret_cast<quintptr *>(buf.data())[0] = id_element; //-V206
    m_cgt->GetParam(PARAM_COMPILER, buf.data());
    m_compiler = QString::fromLocal8Bit(buf);
}

QJsonDocument SceneModel::serialize()
{
    QVariantMap cgtParams;
    cgtParams.insert("CODE_PATH", m_codePath);
    cgtParams.insert("DEBUG_MODE", m_debugMode);
    cgtParams.insert("DEBUG_SERVER_PORT", m_debugServerPort);
    cgtParams.insert("DEBUG_CLIENT_PORT", m_debugClientPort);
    cgtParams.insert("PROJECT_PATH", m_projectPath);
    cgtParams.insert("HIASM_VERSION", m_hiasmVersion);
    cgtParams.insert("USER_NAME", m_userName);
    cgtParams.insert("USER_MAIL", m_userMail);
    cgtParams.insert("PROJECT_NAME", m_projectName);
    cgtParams.insert("SDE_WIDTH", m_sdeWidth);
    cgtParams.insert("SDE_HEIGHT", m_sdeHeight);
    cgtParams.insert("COMPILER", m_compiler);

    QVariantMap model;
    model.insert("CGTParams", cgtParams);
    model.insert("Container", m_container->serialize());

    return QJsonDocument::fromVariant(model);
}

PCodeGenTools SceneModel::getCgt()
{
    return m_cgt;
}

void SceneModel::deserialize(const QJsonDocument &doc)
{
    const QJsonObject model = doc.object();

    const QJsonObject cgtParams = model["CGTParams"].toObject();
    m_codePath = cgtParams["CODE_PATH"].toString();
    m_debugMode = cgtParams["DEBUG_MODE"].toInt();
    m_debugServerPort = cgtParams["DEBUG_SERVER_PORT"].toInt();
    m_debugClientPort = cgtParams["DEBUG_CLIENT_PORT"].toInt();;
    m_projectPath = cgtParams["PROJECT_PATH"].toString();
    m_hiasmVersion = cgtParams["HIASM_VERSION"].toString();
    m_userName = cgtParams["USER_NAME"].toString();
    m_userMail = cgtParams["USER_MAIL"].toString();
    m_projectName = cgtParams["PROJECT_NAME"].toString();
    m_sdeWidth = cgtParams["SDE_WIDTH"].toInt();
    m_sdeHeight = cgtParams["SDE_HEIGHT"].toInt();
    m_compiler = cgtParams["COMPILER"].toString();

    QJsonObject container = model["Container"].toObject();
    m_container = new Container(container, this);
}

quintptr SceneModel::genId()
{
    while (true) {
        ++m_genId;
        if (m_mapContainers.contains(m_genId))
            continue;
        if (m_mapElements.contains(m_genId))
            continue;
        if (m_mapProperties.contains(m_genId))
            continue;
        if (m_mapPoints.contains(m_genId))
            continue;
        if (m_mapValues.contains(m_genId))
            continue;

        break;
    }

    return m_genId;
}

PSceneModel SceneModel::getModel()
{
    return this;
}

void SceneModel::initFromCgt(PCodeGenTools cgt, quintptr idMainSDK)
{
    m_cgt = cgt;

    //ru Собираем данные о среде
    collectingData(idMainSDK);

    //ru Запуск процесса сборка данных о схеме
    m_container = new Container(idMainSDK, this);
}

bool SceneModel::saveModel(const QString &filePath)
{
    QJsonDocument doc = serialize();
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    file.write(doc.toJson());
    file.close();
    return true;
}

bool SceneModel::loadModel(const QString &filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    deserialize(QJsonDocument::fromJson(file.readAll()));
    return true;
}

bool SceneModel::loadFromSha(const QString &filePath, PackageManager &manager)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    //file.readAll();

    m_package = manager.getPackage("delphi");
    if (!m_package)
        return false;

    m_container = new Container(this);

    SharedConfElement conf = m_package->getElementByName("MainForm");
    if (!conf)
        return false;

    m_container->addElement(new Element(conf, 2953706, m_container));

    return true;
}

PPackage SceneModel::getPackage()
{
    return m_package;
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

int SceneModel::getCountElementsInContainer(quintptr id_sdk) const
{
    const PContainer c = getContainerById(id_sdk);
    if (!c)
        return 0;

    return c->getCountElements();
}

quintptr SceneModel::getIdRootContainer() const
{
    if (!m_container)
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

    QString nameTypeRes;
    QString fileName;
    QString ext;
    const PValue v = p->getValue();
    switch (p->getType()) {
    case data_icon: {
        if (v->getValue().isNull())
            return fcgt::strToCString("ASMA");

        nameTypeRes = "ICON";
        fileName = "ICON";
        ext = ".ico";
        break;
    }
    case data_stream: {
        nameTypeRes = "100";
        fileName = "STREAM";
        ext = ".dat";
        break;
    }
    case data_bitmap: {
        if (v->getValue().isNull())
            return nullptr;

        nameTypeRes = "BITMAP";
        fileName = "BITMAP";
        ext = ".bmp";
        break;
    }
    case data_jpeg:
    case data_wave: {
    }
    default:
        return nullptr;
    }

    static const QString SEP = QDir::separator();
    static const QString CURRENT_PATH = QDir::currentPath();

    const QByteArray resData = v->getValue().toByteArray();
    const QString suffix = QString::number(m_resourcesForCompile.size());
    const QString fileNameRes = fileName + suffix;
    const QString fullFileNameRes = fileName + suffix + ext;
    const QString filePathRes = CURRENT_PATH + SEP + m_resourcesDir + SEP + fullFileNameRes;

    QFile file(filePathRes);
    if (!file.open(QIODevice::WriteOnly))
        return nullptr;

    file.write(resData);
    file.close();
    m_resourcesToDelete.insert(filePathRes);
    m_resourcesForCompile.insert(fullFileNameRes, nameTypeRes);

    return fcgt::strToCString(fileNameRes);
}

const char *SceneModel::addStringRes(const QString &str)
{
    if (str.isEmpty())
        return nullptr;
    /*
        static const QString nameCompilerDir = "resources";
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

    */
    return nullptr;
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
    static const QString NAME_FILE_RC = "allres.rc";
    static const QString NAME_FILE_RES = "allres.res";
    static const QString NAME_FILE_GORC = "GoRC.exe";

    const QString currentPath = QDir::currentPath();
    const QString resourcesPath = currentPath + QDir::separator() + m_resourcesDir;
    const QString rcFilePath = resourcesPath + QDir::separator() + NAME_FILE_RC;
    const QString resFilePath = m_codePath + QDir::separator() + NAME_FILE_RES;

    QDir::setCurrent(resourcesPath);
    QFile file(NAME_FILE_RC);
    file.open(QIODevice::WriteOnly);
    QTextStream write(&file);

    for (const auto &filePath : m_resourcesForCompile.keys()) {
        QFileInfo file(filePath);

        write << QString("%1 %2 %3\r\n").arg(file.baseName()).arg(m_resourcesForCompile[filePath]).arg(filePath);
    }

    write << "ASMA ICON main.ico";
    file.close();

    QProcess::execute(QString("%1 /nw /fo \"%2\" %3").arg(NAME_FILE_GORC).arg(resFilePath).arg(NAME_FILE_RC));
    QDir::setCurrent(currentPath);

    addResList(rcFilePath);
    addResList(resFilePath);
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

void SceneModel::getCgtParam(CgtParams index, void *buf) const
{
    auto writeString = [buf](const QString & str) {
        strcpy(reinterpret_cast<char *>(buf), str.toStdString().c_str());
    };
    auto writeInt = [buf](int value) {
        *reinterpret_cast<int *>(buf) = value; //-V206
    };

    switch (index) {
    case PARAM_CODE_PATH :
        writeString(m_codePath);
        break;
    case PARAM_DEBUG_MODE:
        writeInt(m_debugMode);
        break;
    case PARAM_DEBUG_SERVER_PORT:
        writeInt(m_debugServerPort);
        break;
    case PARAM_DEBUG_CLIENT_PORT:
        writeInt(m_debugClientPort);
        break;
    case PARAM_PROJECT_PATH:
        writeString(m_projectPath);
        break;
    case PARAM_HIASM_VERSION:
        writeString(m_hiasmVersion);
        break;
    case PARAM_USER_NAME:
        writeString(m_userName);
        break;
    case PARAM_USER_MAIL:
        writeString(m_userMail);
        break;
    case PARAM_PROJECT_NAME:
        writeString(m_projectName);
        break;
    case PARAM_SDE_WIDTH:
        writeInt(m_sdeWidth);
        break;
    case PARAM_SDE_HEIGHT:
        writeInt(m_sdeHeight);
        break;
    case PARAM_COMPILER:
        writeString(m_compiler);
        break;
    }
}

bool SceneModel::getIsDebug() const
{
    return m_isDebug;
}

void SceneModel::setIsDebug(bool isDebug)
{
    m_isDebug = isDebug;
}

int SceneModel::getDebugMode() const
{
    return m_debugMode;
}

void SceneModel::setDebugMode(int debugMode)
{
    m_debugMode = debugMode;
}

int SceneModel::getDebugServerPort() const
{
    return m_debugServerPort;
}

void SceneModel::setDebugServerPort(int debugServerPort)
{
    m_debugServerPort = debugServerPort;
}

int SceneModel::getDebugClientPort() const
{
    return m_debugClientPort;
}

void SceneModel::setDebugClientPort(int debugClientPort)
{
    m_debugClientPort = debugClientPort;
}

int SceneModel::getSdeWidth() const
{
    return m_sdeWidth;
}

void SceneModel::setSdeWidth(int sdeWidth)
{
    m_sdeWidth = sdeWidth;
}

int SceneModel::getSdeHeight() const
{
    return m_sdeHeight;
}

void SceneModel::setSdeHeight(int sdeHeight)
{
    m_sdeHeight = sdeHeight;
}

QString SceneModel::getCodePath() const
{
    return m_codePath;
}

void SceneModel::setCodePath(const QString &codePath)
{
    m_codePath = codePath;
}

QString SceneModel::getProjectPath() const
{
    return m_projectPath;
}

void SceneModel::setProjectPath(const QString &projectPath)
{
    m_projectPath = projectPath;
}

QString SceneModel::getHiasmVersion() const
{
    return m_hiasmVersion;
}

void SceneModel::setHiasmVersion(const QString &hiasmVersion)
{
    m_hiasmVersion = hiasmVersion;
}

QString SceneModel::getUserName() const
{
    return m_userName;
}

void SceneModel::setUserName(const QString &userName)
{
    m_userName = userName;
}

QString SceneModel::getUserMail() const
{
    return m_userMail;
}

void SceneModel::setUserMail(const QString &userMail)
{
    m_userMail = userMail;
}

QString SceneModel::getProjectName() const
{
    return m_projectName;
}

void SceneModel::setProjectName(const QString &projectName)
{
    m_projectName = projectName;
}

QString SceneModel::getCompiler() const
{
    return m_compiler;
}

void SceneModel::setCompiler(const QString &compiler)
{
    m_compiler = compiler;
}
