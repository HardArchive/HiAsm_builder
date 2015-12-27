#pragma once

//Project
#include "package.h"

//STL

//Native

//Qt
#include <QtCore>

class PackageManager : public QObject
{
public:
    const QString DIR_PACKAGES = "packages";
    typedef QVector<PPackage> Packages;

    Q_OBJECT
private:
    Packages m_packages;
    bool m_sucess = true;

public:
    explicit PackageManager(QObject *parent = 0);

private:
    void initPackages();
};
