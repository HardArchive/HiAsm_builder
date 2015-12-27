#pragma once

//Project
#include "types.h"
#include "confelement.h"

//STL

//Native

//Qt
#include <QtCore>

namespace PackageInfo
{
    static const QString INFO_FILE_NAME = "package";
    static const QString CONF_DIR = "conf";
    static const QString ICON_DIR = "icon";
}

class Package : public QObject
{
    Q_OBJECT
    
private:
    //Информация о пакете
    QString m_packagePath; //Путь к пакету
    QString m_packageFileInfo; //Путь к файлу информации о пакете
    QString m_confPath; //Путь к каталогу с конфигурациями элементов
    QString m_name; //Название пакета
    QString m_shortDescription; //Короткое описание пакета
    QString m_description; //Полное описание пакета
    bool m_visible = true; //Видимость пакета
    bool m_base = false; //Базовый пакет
    bool m_success = false; //Информация успешно загружена

    //Compiler
    ListCompilers m_compilers; //Массив компиляторов

    //Project
    ListProjects m_projects; //Массив проектов

    //Compiler
    ListConfElements m_confElements; //Массив компиляторов

public:
    explicit Package(const QString &packagePath, QObject *parent);

private:
    bool loadPackageInfo();
    bool loadElements();

public:
    bool getSuccess() const;
    void setSuccess(bool success);

};
