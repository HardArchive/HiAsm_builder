//Project
#include "package.h"

//STL

//Native

//Qt
#include <QDebug>
#include <QDir>
#include <QJsonDocument>

Package::Package(const QString &packagePath, QObject *parent)
    : QObject(parent)
    , m_packagePath(packagePath)
{
    //!!! Назначение путей !!!

    //Задаём путь к файлу информации о пакете
    m_packageFileInfo = m_packagePath + QDir::separator() + PackageInfo::INFO_FILE_NAME;
    if (!QFile::exists(m_packageFileInfo)) {
        setSuccess(false);
        return;
    }

    //Задаём путь к папке с элементами
    m_confPath = m_packagePath + QDir::separator() + PackageInfo::CONF_DIR;

    //!!! Загружаем информацию о пакете/элементах !!!
    
    //Если не удалось по каким либо причинам прочитать информацию о пакете
    if (!loadPackageInfo()) {
        setSuccess(false);
        return;
    }
    
    //Если не удалось найти и загрузить информацию об элементах
    if (!loadElements()) {
        setSuccess(false);
        return;
    }

    assignInherits();

    setSuccess(true);
}

//ru Получаем информацию об пакете и его проектах
//ru из файла описания проекта: package
bool Package::loadPackageInfo()
{
    QFile packageFileInfo(m_packageFileInfo);
    packageFileInfo.open(QIODevice::ReadOnly);
    const QByteArray dataPackageInfo = packageFileInfo.readAll();
    packageFileInfo.close();

    const QJsonDocument info = QJsonDocument::fromJson(dataPackageInfo);
    if (!info.isNull()) {
        const QJsonObject infoPackage = info.object();
        
        //!ru Получаем информацию о проекте
        
        //ru Название пакета - обязательно заполнять.
        if (infoPackage.contains("name")) {
            m_name = infoPackage["name"].toString();
        } else {
            return false;
        }
        
        //ru Короткое описание пакета - необязательно заполнять.
        if (infoPackage.contains("shortDescription")) {
            m_shortDescription = infoPackage["shortDescription"].toString();
        }

        //ru Полное описание пакета - необязательно заполнять.
        if (infoPackage.contains("description")) {
            m_description = infoPackage["description"].toString();
        }

        //ru Видимость пакета - необязательный параметр.
        //ru По умолчанию - видимый.
        if (infoPackage.contains("visible"))
            m_visible = infoPackage["visible"].toBool();
            
        //ru Является ли пакет базовым - необязательный параметр
        //ru По умолчанию - нет
        if (infoPackage.contains("base"))
            m_base = infoPackage["base"].toBool();

        //!ru Получаем информацию о компиляторах
        for (auto json : infoPackage["compilers"].toArray()) {
            QJsonObject object = json.toObject();

            //ru Создаём объект проекта и заполняем его.
            SharedCompilerInfo compiler = SharedCompilerInfo::create();

            //ru Добавляем проект в массив проектов текущего пакета.
            m_compilers.append(compiler);

            //ru Название проекта - обязательно заполнять.
            if (object.contains("name")) {
                compiler->name = object["name"].toString();
            } else {
                return false;
            }

            //ru Путь к файлу компилятора - обязательно заполнять
            if (object.contains("path")) {
                compiler->path = object["path"].toString();
            } else {
                return false;
            }

            //ru Командная строка, которая будет передана компилятору - обязательно заполнять
            if (object.contains("command_line")) {
                compiler->command_line = object["command_line"].toString();
            } else {
                return false;
            }

            //ru Расширение целевого проекта - обязательно заполнять
            if (object.contains("ext")) {
                compiler->ext = object["ext"].toString();
            } else {
                return false;
            }
        }

        //!!!ru Получаем информацию о проектах !!!
        QJsonArray projects = infoPackage["projects"].toArray();
        for (auto json : projects) {
            QJsonObject object = json.toObject();
            
            //ru Создаём объект проекта и заполняем его.
            SharedProjectInfo project = SharedProjectInfo::create();

            //ru Добавляем проект в массив проектов текущего пакета.
            m_projects.append(project);
            
            //ru Название проекта - обязательно заполнять.
            if (object.contains("name")) {
                project->name = object["name"].toString();
            } else {
                return false;
            }
            
            //ru Короткое описание проекта - необязательно заполнять
            if (object.contains("shortDescription")) {
                project->shortDescription = object["shortDescription"].toString();
            } else {
                return false;
            }

            //ru Полное описание проекта - необязательно заполнять
            if (object.contains("description")) {
                project->description = object["description"].toString();
            } else {
                return false;
            }
            
            //ru Название начального элемента - обязательно заполнять
            if (object.contains("entryElement")) {
                project->entryElement = object["entryElement"].toString();
            } else {
                return false;
            }

            //ru Расширение целевого файла - обязательно заполнять
            if (object.contains("ext")) {
                project->ext = object["ext"].toString();
            } else {
                return false;
            }

            //ru Имя make библиотеки - обязательно заполнять
            if (object.contains("make")) {
                project->make = object["make"].toString();
            } else {
                return false;
            }
            
        }
        
        return true;
    }
    
    return false;
}

bool Package::loadElements()
{
    QDir elementsPath(m_confPath);
    elementsPath.setFilter(QDir::Files);
    QFileInfoList listElements = elementsPath.entryInfoList();
    
    if (listElements.empty()) {
        qWarning() << "Элементы отсутствуют!";
        qWarning() << elementsPath.path();
        
        return false;
    }
    
    for (const QFileInfo &element : listElements) {
        m_confElements << SharedConfElement::create(element.filePath());
    }
    
    return true;
}

void Package::assignInherits()
{
    for (SharedConfElement &conf : m_confElements) {
        conf->assignInherit(this);
    }
}

bool Package::getSuccess() const
{
    return m_success;
}

void Package::setSuccess(bool success)
{
    m_success = success;
}

SharedConfElement Package::getElementByName(const QString &name)
{
    for (const SharedConfElement conf :  m_confElements) {
        if (QString::compare(conf->getName(), name, Qt::CaseInsensitive) == 0) {
            return conf;
        }
    }

    return SharedConfElement();
}

