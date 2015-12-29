//Project
#include "confelement.h"
#include "package.h"
#include "cgt/CGTShare.h"

//STL

//Native

//Qt

QString ConfElement::getName() const
{
    return m_name;
}

void ConfElement::setName(const QString &name)
{
    m_name = name;
}

QString ConfElement::getVersion() const
{
    return m_version;
}

void ConfElement::setVersion(const QString &version)
{
    m_version = version;
}

QString ConfElement::getAuthor() const
{
    return m_author;
}

void ConfElement::setAuthor(const QString &author)
{
    m_author = author;
}

QString ConfElement::getMail() const
{
    return m_mail;
}

void ConfElement::setMail(const QString &mail)
{
    m_mail = mail;
}

QString ConfElement::getClassName() const
{
    return m_class;
}

void ConfElement::setClass(const QString &nameClass)
{
    m_class = nameClass;
}

QStringList ConfElement::getInherit() const
{
    return m_inherit;
}

void ConfElement::setInherit(const QStringList &inherit)
{
    m_inherit = inherit;
}

QString ConfElement::getSub() const
{
    return m_sub;
}

void ConfElement::setSub(const QString &sub)
{
    m_sub = sub;
}

QString ConfElement::getInfo() const
{
    return m_info;
}

void ConfElement::setInfo(const QString &info)
{
    m_info = info;
}

QString ConfElement::getPropIcon() const
{
    return m_propIcon;
}

void ConfElement::setPropIcon(const QString &propIcon)
{
    m_propIcon = propIcon;
}

QString ConfElement::getView() const
{
    return m_view;
}

void ConfElement::setView(const QString &view)
{
    m_view = view;
}

QString ConfElement::getTab() const
{
    return m_tab;
}

void ConfElement::setTab(const QString &tab)
{
    m_tab = tab;
}

QString ConfElement::getInterface() const
{
    return m_interface;
}

void ConfElement::setInterface(const QString &interface)
{
    m_interface = interface;
}

QString ConfElement::getEditClass() const
{
    return m_editClass;
}

void ConfElement::setEditClass(const QString &editClass)
{
    m_editClass = editClass;
}

void ConfElement::addInheritableData(PPackage pack)
{
    if (m_isInherited && !pack)
        return;

    ListConfProps props;
    auto containsProp = [&props](const QString & name) {
        for (const SharedConfProp &prop : props)
            if (prop->name == name)
                return true;

        return false;
    };
    auto inheritProps = [&props, containsProp](const ListConfProps & inheritProps) {
        for (const SharedConfProp &prop : inheritProps)
            if (!containsProp(prop->name))
                props.append(prop);
    };

    auto containsPoint = [](const ListConfPoints & points, const QString & name) {
        for (const SharedConfPoint &point : points)
            if (point->name == name)
                return true;
        return false;
    };
    auto inheritPoints = [containsPoint](const ListConfPoints & inheritPoints, ListConfPoints & points) {
        for (const SharedConfPoint &point : inheritPoints)
            if (!containsPoint(points, point->name))
                points.append(point);
    };

    ListConfPoints hiddenPoints;
    ListConfPoints points;
    for (const QString &name : m_inherit) {
        SharedConfElement e = pack->getElementByName(name);
        e->addInheritableData(pack);

        m_sub = e->getSub();
        m_interface = e->getInterface();

        inheritProps(e->getProperties());
        inheritPoints(e->getPoints(), points);
        inheritPoints(e->getHiddenPoints(), hiddenPoints);
    }

    inheritProps(m_properties);
    inheritPoints(m_hiddenPoints, hiddenPoints);
    inheritPoints(m_points, points);

    m_properties = props;
    m_hiddenPoints = hiddenPoints;
    m_points = points;

    m_isInherited = true;
}

ListConfProps ConfElement::getProperties() const
{
    return m_properties;
}

ListConfPoints ConfElement::getPoints() const
{
    return m_points;
}

ListConfPoints ConfElement::getHiddenPoints() const
{
    return m_hiddenPoints;
}

ConfElement::ConfElement(const QString &pathConf)
    : m_pathConf(pathConf)
{
    initConfElement();
    loadConf();
}

void ConfElement::initConfElement()
{
    QFileInfo file(m_pathConf);
    m_name = file.baseName();
}

void ConfElement::loadConf()
{
    QFile file(m_pathConf);
    file.open(QIODevice::ReadOnly);
    const QString data = QString::fromLocal8Bit(file.readAll());
    file.close();

    TypeSection section = ts_undefine;
    QStringList secAbouts;
    QStringList secTypes;
    QStringList secProperties;
    QStringList secPoints;

    for (QString line : data.split("\r\n")) {
        line = line.trimmed();
        size_t size = line.size();
        if (size < 2)
            continue;

        if (line.at(0) == QLatin1Char('[')) {
            const QChar c = line.at(1).toLower();

            if (c == QLatin1Char('a')) {
                section = ts_abouts;
            } else if (c == QLatin1Char('t')) {
                section = ts_types;
            } else if (c == QLatin1Char('e')) {
                section = ts_edit;
            } else if (c == QLatin1Char('p')) {
                section = ts_properties;
            } else if (c == QLatin1Char('m')) {
                section = ts_points;
            } else {
                section = ts_undefine;
            }

            continue;
        }

        switch (section) {
        case ts_abouts:
            secAbouts << line;
            break;
        case ts_types:
            secTypes << line;
            break;
        case ts_edit: {
            if (!m_editClass.isEmpty())
                continue;
            if (line.section('=', 0, 0).toLower() == QLatin1String("class"))
                m_editClass = line.section('=', 1, 1);
            break;
        }
        case ts_properties:
            secProperties << line;
            break;
        case ts_points:
            secPoints << line;
            break;
        default: {
            //qWarning(qUtf8Printable("При загрузке конфигурации элемента, найден неизвестный параметр."));
            //qWarning("Файл: \"%s\"", qUtf8Printable(m_pathConf));
            //qWarning("Параметр: \"%s\"", qUtf8Printable(line));
            break;
        }
        }
    }

    parseAbout(secAbouts);
    parseTypes(secTypes);
    parseProperties(secProperties);
    parsePoints(secPoints);
}

void ConfElement::parseAbout(const QStringList &list)
{
    for (const QString &line : list) {
        const QString &name = line.section(QLatin1Char('='), 0, 0).toLower();
        const QString &value = line.section(QLatin1Char('='), 1, 1).toLower();

        if (name == QLatin1String("version")) {
            m_version = value;

        } else if (name == QLatin1String("author")) {
            m_author = value;

        } else if (name == QLatin1String("mail")) {
            m_mail = value;
        }
    }
}

void ConfElement::parseTypes(const QStringList &list)
{
    for (const QString &line : list) {
        QString sec0 = line.section("=", 0, 0).toLower();
        QString sec1 = line.section("=", 1, 1);
        if (sec0 == QLatin1String("class")) {
            m_class = sec1;
        } else if (sec0 == QLatin1String("inherit")) {
            m_inherit = sec1.split(QLatin1Char(','), QString::SkipEmptyParts);
        } else if (sec0 == QLatin1String("sub")) {
            m_sub = sec1;
        } else if (sec0 == QLatin1String("info")) {
            m_info = sec1;
        } else if (sec0 == QLatin1String("icon")) {
            m_propIcon = sec1;
        } else if (sec0 == QLatin1String("view")) {
            m_view = sec1;
        } else if (sec0 == QLatin1String("tab")) {
            m_tab = sec1;
        } else if (sec0 == QLatin1String("interfaces")) {
            m_interface = sec1;
        }
    }
}

void ConfElement::parseProperties(const QStringList &list)
{
    //##имя_группы=описание
    //## закрываем группу

    //+ activated - свойсво по-умолчанию, т.е. открывается редактор
    //при двойном клике.

    //@ makePoint - Позволяет активировать свойство в виде метода с префиксом do*


    //PropGroup
    QString nameGroup;

    //Parser
    bool beginGroup = false;

    for (const QString &line : list) {
        //ConfProp
        QString name;
        QString desc;
        QString value;
        QString listValues;
        QString type;
        bool makePoint = false;
        bool activated = false;

        //PropGroup
        QString descGroup;

        //Parser
        bool equalSign = false;
        bool beginGroupLine = false;
        uchar countPipe = 0;
        uchar countSharp = 0;

        size_t outIndex = line.size();

        for (size_t i = 0; i <= outIndex; ++i) {
            if (i == outIndex) {
                if (beginGroupLine) {
                    m_group.insert(nameGroup, descGroup);
                    continue;
                }
                if (countSharp == 2) {
                    if (beginGroup) {
                        beginGroup = false;
                        nameGroup.clear();
                        continue;
                    }
                }

                SharedConfProp prop = SharedConfProp::create();
                prop->name = name;
                prop->desc = desc;
                prop->type = DataType(type.toInt());
                prop->value = value;
                prop->listValues = listValues.split(QLatin1Char(','), QString::SkipEmptyParts);
                if (beginGroup)
                    prop->group = nameGroup;
                prop->activated = activated;
                prop->makePoint = makePoint;

                m_properties << prop;
                break;
            }
            const QChar &c = line[i];

            if (i < 2) {
                if (c == QLatin1Char('#')) {
                    ++countSharp;
                    continue;
                } else if (c == QLatin1Char('@')) {
                    makePoint = true;
                    continue;
                } else if (!makePoint && c == QLatin1Char('+')) {
                    activated = true;
                    continue;
                }
            }

            if (c == QLatin1Char('=')) {
                equalSign = true;
                continue;
            }
            if (c == QLatin1Char('|')) {
                ++countPipe;
                continue;
            }

            if (countSharp == 2) {
                if (!beginGroup) {
                    beginGroup = true;
                    beginGroupLine = true;
                }
            }

            if (beginGroupLine) {
                if (!equalSign)
                    nameGroup += c; //Имя группы
                else
                    descGroup += c; //Описание

                continue;
            }

            if (countPipe) {
                if (countPipe == 1)  //Тип
                    type += c;
                else if (countPipe == 2) //Значение
                    value += c;
                else if (countPipe == 3) //Список значений
                    listValues += c;

                continue;
            }

            if (!equalSign)
                name += c; //Имя свойства
            else
                desc += c; //Описание свойства
        }
    }
}

void ConfElement::parsePoints(const QStringList &list)
{
    //* - скрытая точка
    for (const QString &line : list) {
        //ConfPoint
        QString name;
        QString desc;
        QString prop;
        QString dataType;
        QString pointType;
        bool hidden = false;

        //Parser
        bool propBegin = false;
        bool propEnd = false;
        bool equalSign = false;
        uchar countPipe = 0;

        const size_t outIndex = line.size();
        for (size_t i = 0; i <= outIndex; ++i) {
            if (i == outIndex) {
                SharedConfPoint point = SharedConfPoint::create();
                point->name = name;
                point->desc = desc;
                point->prop = prop;
                point->pointType = PointType(pointType.toInt());
                point->dataType = DataType(dataType.toInt());

                if (hidden)
                    m_hiddenPoints << point;
                else
                    m_points << point;

                break;
            }
            const QChar &c = line[i];

            if (i == 0 && c == QLatin1Char('*')) {
                hidden = true;
                continue;
            }
            if (c == QLatin1Char('%')) {
                if (!propBegin)
                    propBegin = true;
                else
                    propEnd = true;

                continue;
            }
            if (c == QLatin1Char('=')) {
                equalSign = true;
                continue;
            }
            if (c == QLatin1Char('|')) {
                ++countPipe;
                continue;
            }
            if (propBegin && !propEnd) {
                prop += c;
                continue;
            }
            if (countPipe) {
                if (countPipe == 1)  //Тип точки
                    pointType += c;
                if (countPipe == 2)  //Тип данных
                    dataType += c;

                continue;
            }
            if (!equalSign)
                name += c; //Имя точки
            else
                desc += c; //Описание точки
        }
    }
}
