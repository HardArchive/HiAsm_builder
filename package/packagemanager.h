#pragma once

//Project
#include "package.h"

//STL

//Native

//Qt
#include <QtCore>

class PackageManager : public QObject
{
    Q_OBJECT

private:
    const QString m_packagesDir = "packages";
    Packages m_packages;
    bool m_sucess = true;

public:
    explicit PackageManager(QObject *parent = 0);

private:
    void initPackages();
};
