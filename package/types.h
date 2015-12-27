#pragma once

//Project
#include "cgt/CGTShare.h"

//STL

//Native

//Qt
#include "QtCore"
#include <QSharedPointer>

struct ConfProp {
    QString name;
    QString value;
    DataTypes type{};
    int groupIndex = -1;
    bool makePoint = false;
    bool activated = false;
};
typedef QSharedPointer<ConfProp> SharedConfProp;
typedef QList<SharedConfProp> ListConfProps;

struct ConfPropGroup {
    QString name;
    QString desc;

    ConfPropGroup(const QString &_name, const QString &_desc)
        : name(_name)
        , desc(_desc)
    {

    }
};
typedef QSharedPointer<ConfPropGroup> SharedConfPropGroup;
typedef QList<SharedConfPropGroup> ListConfPropGroup;

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
