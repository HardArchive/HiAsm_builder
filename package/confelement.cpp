//Project
#include "confelement.h"
#include "cgt/CGTShare.h"

//STL

//Native

//Qt

ConfElement::ConfElement(const QString &pathConf)
    : m_pathConf(pathConf)
{
    LoadConf();
}

void ConfElement::LoadConf()
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
            m_inherits = sec1.split(QLatin1Char(','), QString::SkipEmptyParts);
        } else if (sec0 == QLatin1String("sub")) {
            m_sub = sec1;
        } else if (sec0 == QLatin1String("info")) {
            m_info = sec1;
        } else if (sec0 == QLatin1String("icon")) {
            m_icon = sec1;
        } else if (sec0 == QLatin1String("view")) {
            m_view = sec1;
        } else if (sec0 == QLatin1String("tab")) {
            m_tab = sec1;
        } else if (sec0 == QLatin1String("interfaces")) {
            m_interfaces = sec1;
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
                } else if (c == QLatin1Char('@')) {
                    makePoint = true;
                } else if (!makePoint && c == QLatin1Char('+')) {
                    activated = true;
                }
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
