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
    QDir dirPackages(DIR_PACKAGES);
    if (!dirPackages.exists()) {
        qCritical() << trUtf8("Каталог \"%1\" не найден").arg(DIR_PACKAGES);
        return;
    }

    dirPackages.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);
    for (const QFileInfo &dir : dirPackages.entryInfoList()) {
        QFile fileInfo(dir.filePath() + QDir::separator() + PackageInfo::FILE_NAME_INFO);

        if (fileInfo.exists()) {
            auto tmp = new Package(dir.absoluteFilePath(), this);

            if (tmp->getSuccess()) {
                m_packages.append(tmp);
            }
        }
    }

    return;
}
