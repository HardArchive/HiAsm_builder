#pragma once

//Project
#include "cgt/CGTShare.h"

//STL

//Native

//Qt
#include "QtCore"
#include <QSharedPointer>

struct ConfMethod {
    QString name;   /**< method name */
    QString prop;   /**< combo or comboex property */
    QString desc;   /**< method description */
    PointType pointType;      /**< method type pt_xxx */
    DataType dataType;   /**< point type data_xxx */
};
typedef QSharedPointer<ConfMethod> SharedConfMethod;
typedef QList<SharedConfMethod> ListConfMethods;

struct ConfProp {
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
