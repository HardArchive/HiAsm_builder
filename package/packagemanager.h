#pragma once

//Project
#include "package.h"

//STL

//Native

//Qt
#include <QtCore>

namespace PackageManagerInfo
{
    static const QString PACKAGES_DIR = "packages";
}

class PackageManager : public QObject
{
    Q_OBJECT

private:
    Packages m_packages;
    bool m_sucess = true;

public:
    explicit PackageManager(QObject *parent = 0);

private:
    void initPackages();
};
