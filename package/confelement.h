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
    QString m_inherit;
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

public:
    QString getName() const;
    void setName(const QString &name);

    QString getVersion() const;
    void setVersion(const QString &version);

    QString getAuthor() const;
    void setAuthor(const QString &author);

    QString getMail() const;
    void setMail(const QString &mail);

    QString getClass() const;
    void setClass(const QString &nameClass);

    QString getInherits() const;
    void setInherits(const QString &inherit);

    QString getSub() const;
    void setSub(const QString &sub);

    QString getInfo() const;
    void setInfo(const QString &info);

    QString getPropIcon() const;
    void setPropIcon(const QString &propIcon);

    QString getView() const;
    void setView(const QString &view);

    QString getTab() const;
    void setTab(const QString &tab);

    QString getInterfaces() const;
    void setInterfaces(const QString &interfaces);

    QString getEditClass() const;
    void setEditClass(const QString &editClass);

};
typedef QSharedPointer<ConfElement> SharedConfElement;
typedef QList<SharedConfElement> ListConfElements;
