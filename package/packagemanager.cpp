//Project
#include "packagemanager.h"
#include "package.h"


//STL

//Native

//Qt
#include <QtCore>
#include <QDebug>

PackageManager::PackageManager(QObject *parent)
    : QObject(parent)
{
    initPackages();
}

void PackageManager::initPackages()
{
    QDir packagesDir(m_packagesDir);
    if (!packagesDir.exists()) {
        return;
    }

    packagesDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &packInfo : packagesDir.entryInfoList()) {
        auto pack = new Package(packInfo.absoluteFilePath(), this);
        if (pack->getSuccess()) {
            m_packages.append(pack);
        } else {
            delete pack;
        }
    }
}
