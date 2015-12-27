#pragma once
//Project
#include "types.h"

//STL

//Native

//Qt
#include <QtCore>

class ConfElement
{
    enum TypeSection {
        ts_undefine,
        ts_abouts,
        ts_types,
        ts_edit,
        ts_properties,
        ts_points,
    };

private:
    //Self
    QString m_pathConf;
    QString m_name;
    //QIcon m_icon;

    //About
    QString m_version;
    QString m_author;
    QString m_mail;

    //Type
    QString m_class;
    QStringList m_inherits;
    QString m_sub;
    QString m_info;
    QString m_propIcon;
    QString m_view;
    QString m_tab;
    QString m_interfaces;

    //Edit
    QString m_editClass;

    //Group
    MapPropGroup m_group;

    //Properties
    ListConfProps m_properties;

    //Points
    ListConfPoints m_points;
    ListConfPoints m_hiddenPoints;

public:
    explicit ConfElement(const QString &pathConf);

private:
    void initConfElement();
    void loadConf();
    void parseAbout(const QStringList &list);
    void parseTypes(const QStringList &list);
    void parseProperties(const QStringList &list);
    void parsePoints(const QStringList &list);

};
typedef QSharedPointer<ConfElement> SharedConfElement;
typedef QList<SharedConfElement> ListConfElements;
