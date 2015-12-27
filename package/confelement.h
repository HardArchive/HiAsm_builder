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
        ts_about,
        ts_type,
        ts_edit,
        ts_property,
        ts_methods,
    };

private:
    QString m_pathFileConf;

    //Type
    QString m_class;
    QString m_inherit;
    QString m_sub;
    QString m_info;
    QString m_icon;
    QString m_view;
    QString m_tab;
    QString m_interfaces;

    //Edit
    QString m_editClass;

    //Group
    ListConfPropGroup m_group;

    //Properties
    ListConfProps m_properties;

    //Sections
    QStringList m_secAbout;
    QStringList m_secType;
    QStringList m_secProperty;
    QStringList m_secMethod;

public:
    explicit ConfElement(const QString &pathFileConf);

private:
    void parseSections();
    void parseAbout();
    void parseType();
    void parseProperty();

};
typedef QSharedPointer<ConfElement> SharedConfElement;
typedef QList<SharedConfElement> ListConfElements;
