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
    QDir dirPackages(PackageManagerInfo::PACKAGES_DIR);
    if (!dirPackages.exists()) {
        qCritical() << trUtf8("Каталог \"%1\" не найден").arg(PackageManagerInfo::PACKAGES_DIR);
        return;
    }

    dirPackages.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &dir : dirPackages.entryInfoList()) {
        QFile fileInfo(dir.filePath() + QDir::separator() + PackageInfo::INFO_FILE_NAME);

        if (fileInfo.exists()) {
            auto pack = new Package(dir.absoluteFilePath(), this);

            if (pack->getSuccess()) {
                m_packages.append(pack);
            } else {
                delete pack;
            }
        }
    }

    return;
}
