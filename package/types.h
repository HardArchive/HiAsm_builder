#pragma once

//Project
#include "cgt/CGTShare.h"

//STL

//Native

//Qt
#include "QtCore"
#include <QSharedPointer>

struct ConfPoint {
    QString name;
    QString prop;
    QString desc;
    PointType pointType;
    DataType dataType;
};
typedef QSharedPointer<ConfPoint> SharedConfPoint;
typedef QList<SharedConfPoint> ListConfPoints;

struct ConfProp { //-V802
    QString name;
    QString desc;
    QString value;
    QStringList listValues;
    QString group;
    DataType type{};
    bool makePoint = false;
    bool activated = false;
};
typedef QSharedPointer<ConfProp> SharedConfProp;
typedef QList<SharedConfProp> ListConfProps;

typedef QMap<QString, QString> MapPropGroup;

struct ProjectInfo {
    QString name;
    QString shortDescription;
    QString description;
    QString entryElement;
    QString ext;
    QString make;
};
typedef QSharedPointer<ProjectInfo> SharedProjectInfo;
typedef QList<SharedProjectInfo> ListProjects;

struct CompilerInfo {
    QString name;
    QString path;
    QString command_line;
    QString ext;
};
typedef QSharedPointer<CompilerInfo> SharedCompilerInfo;
typedef QList<SharedCompilerInfo> ListCompilers;

class Package;
typedef Package *PPackage;
typedef QVector<PPackage> Packages;
