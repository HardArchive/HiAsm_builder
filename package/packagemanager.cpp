//Project
#include "packagemanager.h"
#include "package.h"


//STL

//Native

//Qt
#include <QtCore>

PackageManager::PackageManager(QObject *parent)
    : QObject(parent)
{
    m_pathPackages = QDir::currentPath() + QDir::separator() + m_packagesDir;
}

PPackage PackageManager::getPackage(const QString &namePack)
{
    if (m_packages.contains(namePack))
        return m_packages[namePack];

    const QString pathPackage = m_pathPackages + QDir::separator() + namePack;
    PPackage pack = new Package(pathPackage, this);
    if (pack->getSuccess()) {
        m_packages.insert(namePack, pack);
        return pack;
    } else {
        delete pack;
        return nullptr;
    }
}
