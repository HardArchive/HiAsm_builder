//Project
#include "element.h"
#include "container.h"
#include "point.h"
#include "property.h"
#include "scenemodel.h"
#include "cgt/cgt.h"

//STL

//Qt
Element::Element(quintptr id_element, QObject *parent)
    : QObject(parent)
    , m_id(id_element)
    , m_model(parent->property("model").value<PSceneModel>())
{
    m_model->addElementToMap(this);
    collectingData();
}

Element::Element(const QJsonObject &object, QObject *parent)
    : QObject(parent)
    , m_model(parent->property("model").value<PSceneModel>())
{
    deserialize(object);
}

void Element::collectingData()
{
    m_classIndex = cgt::elGetClassIndex(m_id);
    m_flags = cgt::elGetFlag(m_id);
    m_group = cgt::elGetGroup(m_id);
    m_linkIs = cgt::elLinkIs(m_id);
    m_linkMain = cgt::elLinkMain(m_id);
    cgt::elGetPos(m_id, m_posX, m_posY);
    cgt::elGetSize(m_id, m_sizeW, m_sizeH);
    m_className = QString::fromLocal8Bit(cgt::elGetClassName(m_id));
    m_codeName = QString::fromLocal8Bit(cgt::elGetCodeName(m_id));
    m_inherit = QString::fromLocal8Bit(cgt::elGetInherit(m_id));
    m_interface = QString::fromLocal8Bit(cgt::elGetInterface(m_id));
    m_infSub = QString::fromLocal8Bit(cgt::elGetInfSub(m_id));
    int ptCount = cgt::elGetPtCount(m_id);
    int propCount = cgt::elGetPropCount(m_id);

    //ru Получаем информацию о точках
    for (int i = 0; i < ptCount; ++i) {
        quintptr pointId = cgt::elGetPt(m_id, i);
        addPoint(new Point(pointId, this));
    }

    //ru Получаем информацию о свойствах
    for (int i = 0; i < propCount; ++i) {
        quintptr propId = cgt::elGetProperty(m_id, i);
        bool defProp = cgt::elIsDefProp(m_id, i);
        addProperty(new Property(propId, this))->setIsDefProp(defProp);
    }

    if (fcgt::isLink(m_flags))
        return;

    //ru Элемент содержит контейнер(ы)
    if (fcgt::isMulti(m_flags)) {
        //ru Элемен содержит полиморфный контейнер
        if (fcgt::isPolyMulti(m_classIndex)) {
            //ru Получаем к-во контейнеров, которое содержит текущий элемент
            int countContainers = cgt::elGetSDKCount(m_id);

            for (int i = 0; i < countContainers; ++i) {
                //ru Получаем ID контейнера
                quintptr id_sdk = cgt::elGetSDKByIndex(m_id, i);
                QString name = QString::fromLocal8Bit(cgt::elGetSDKName(id_sdk, i));

                //ru Добавляем контейнер в элемент
                addContainer(new Container(id_sdk, this))->setName(name);
            }
        } else { //ru Элемент содержит обычный контейнер

            //ru Получаем ID контейнера элемента
            quintptr id_sdk = cgt::elGetSDK(m_id);

            //ru Добавляем контейнер в элемент
            addContainer(new Container(id_sdk, this));
        }
    }
}

QVariantMap Element::serialize()
{
    QVariantMap data;
    data.insert("id", m_id);
    //data.insert("userData", m_userData);
    data.insert("classIndex", m_classIndex);
    data.insert("flags", int(m_flags));
    data.insert("group", m_group);
    data.insert("linkIs", m_linkIs);
    data.insert("linkMain", m_linkMain);
    data.insert("posX", m_posX);
    data.insert("posY", m_posY);
    data.insert("sizeW", m_sizeW);
    data.insert("sizeH", m_sizeH);
    data.insert("className", m_className);
    data.insert("codeName", m_codeName);
    data.insert("interface", m_interface);
    data.insert("inherit", m_inherit);
    data.insert("infSub", m_infSub);

    QVariantList containers;
    for (const PContainer c : m_containers) {
        containers.append(c->serialize());
    }

    QVariantList points;
    for (const PPoint p : m_points) {
        points.append(p->serialize());
    }

    QVariantList properties;
    for (const PProperty p : m_properties) {
        properties.append(p->serialize());
    }

    QVariantMap element;
    element.insert("Data", data);
    element.insert("Containers", containers);
    element.insert("Points", points);
    element.insert("Properties", properties);

    return element;
}

void Element::deserialize(const QJsonObject &object)
{
    const auto data = object["Data"].toObject();
    const auto containers = object["Containers"].toArray();
    const auto properties = object["Properties"].toArray();
    const auto points = object["Points"].toArray();

    m_id = data["id"].toVariant().toUInt();
    m_model->addElementToMap(this);

    //m_userData = data["userData"].toVariant().toUInt();
    m_classIndex = ElementClass(data["classIndex"].toInt());
    m_flags = ElementFlgs(data["flags"].toInt());
    m_group = data["group"].toInt();
    m_linkIs = data["linkIs"].toBool();
    m_linkMain = data["linkMain"].toVariant().toUInt();
    m_posX = data["posX"].toInt();
    m_posY = data["posY"].toInt();
    m_sizeW = data["sizeW"].toInt();
    m_sizeH = data["sizeH"].toInt();
    m_className = data["className"].toString();
    m_codeName = data["codeName"].toString();
    m_interface = data["interface"].toString();
    m_inherit = data["inherit"].toString();
    m_infSub = data["infSub"].toString();

    for (const auto c : containers) {
        addContainer(new Container(c.toObject(), this));
    }
    for (const auto p : properties) {
        addProperty(new Property(p.toObject(), this));
    }
    for(const auto p : points){
       addPoint(new Point(p.toObject(), this));
    }
}

quintptr Element::getId() const
{
    return m_id;
}

PContainer Element::getParent() const
{
    return qobject_cast<PContainer>(parent());
}

void Element::setUserData(quintptr userData)
{
    m_userData = userData;
}

quintptr Element::getUserData() const
{
    return m_userData;
}

void Element::setClassIndex(ElementClass classIndex)
{
    m_classIndex = classIndex;
}

ElementClass Element::getClassIndex()
{
    return m_classIndex;
}

void Element::setFlags(const ElementFlgs &flags)
{
    m_flags = flags;
}

ElementFlags Element::getFlags() const
{
    return ElementFlags(int(m_flags));
}

void Element::setGroup(int group)
{
    m_group = group;
}

int Element::getGroup() const
{
    return m_group;
}

void Element::setLinkIs(bool linkIs)
{
    m_linkIs = linkIs;
}

bool Element::getLinkIs() const
{
    return m_linkIs;
}

void Element::setLinkMain(quintptr linkMain)
{
    m_linkMain = linkMain;
}

quintptr Element::getLinkMain() const
{
    return m_linkMain;
}

void Element::setPosX(int posX)
{
    m_posX = posX;
}

int Element::getPosX() const
{
    return m_posX;
}

void Element::setPosY(int posY)
{
    m_posY = posY;
}

int Element::getPosY() const
{
    return m_posY;
}

void Element::setSizeW(int sizeW)
{
    m_sizeW = sizeW;
}

int Element::getSizeW() const
{
    return m_sizeW;
}

void Element::setSizeH(int sizeH)
{
    m_sizeH = sizeH;
}

int Element::getSizeH() const
{
    return m_sizeH;
}

void Element::setClassName(const QString &className)
{
    m_className = className;
}

QString Element::getClassName() const
{
    return m_className;
}

void Element::setCodeName(const QString &name)
{
    m_codeName = name;
}

QString Element::getCodeName() const
{
    return m_codeName;
}

void Element::setInterface(const QString &interface)
{
    m_interface = interface;
}

QString Element::getInterface() const
{
    return m_interface;
}

void Element::setInherit(const QString &inherit)
{
    m_inherit = inherit;
}

QString Element::getInherit() const
{
    return m_inherit;
}

void Element::setInfSub(const QString &infSub)
{
    m_infSub = infSub;
}

QString Element::getInfSub() const
{
    return m_infSub;
}

PSceneModel Element::getModel()
{
    return m_model;
}

size_t Element::getCountContainers() const
{
    return m_containers.size();
}

PContainer Element::getContainer() const
{
    if (m_containers.isEmpty())
        return nullptr;

    return m_containers[0];
}

quintptr Element::getIdContainer() const
{
    const PContainer c = getContainer();
    if (!c)
        return 0;

    return c->getId();
}

PContainer Element::getContainerByIndex(uint index)
{
    if (index < uint(m_containers.size()))
        return m_containers[index];
    else
        return nullptr;
}

quintptr Element::getIdContainerByIndex(uint index)
{
    const PContainer c = getContainerByIndex(index);
    if (!c)
        return 0;

    return c->getId();
}

PContainer Element::addContainer(PContainer container)
{
    m_containers.append(container);
    return container;
}

void Element::removeContainer(uint index)
{
    m_containers.remove(index);
}

size_t Element::getCountPoints() const
{
    return m_points.size();
}

PPoint Element::getPointByIndex(uint index) const
{
    if (index < uint(m_points.size()))
        return m_points[index];
    else
        return nullptr;
}

quintptr Element::getIdPointByIndex(uint index) const
{
    const PPoint p = getPointByIndex(index);
    if (!p)
        return 0;

    return p->getId();
}

PPoint Element::getPointByName(const QString &name) const
{
    for (const PPoint p : m_points) {
        if (QString::compare(p->getName(), name, Qt::CaseInsensitive) == 0) {
            return p;
        }
    }

    return nullptr;
}

quintptr Element::getIdPointByName(const QString &name) const
{
    const PPoint p = getPointByName(name);
    if (!p)
        return 0;

    return p->getId();
}

PPoint Element::addPoint(PPoint point)
{
    m_points.append(point);
    return point;
}

void Element::removePoint(uint index)
{
    m_points.remove(index);
}

size_t Element::getCountProps() const
{
    return m_properties.size();
}

PProperty Element::getPropertyByIndex(uint index) const
{
    if (index < uint(m_properties.size()))
        return m_properties[index];
    else
        return PProperty();
}

quintptr Element::getIdPropertyByIndex(uint index) const
{
    const PProperty e = getPropertyByIndex(index);
    if (!e)
        return 0;

    return e->getId();
}

PProperty Element::getPropertyById(quintptr id_prop) const
{
    if (!id_prop)
        return PProperty();

    for (PProperty p : m_properties) {
        if (p->getId() == id_prop) {
            return p;
        }
    }

    return PProperty();
}

PProperty Element::getPropertyByName(const QString &name) const
{
    for (PProperty p : m_properties) {
        if (QString::compare(p->getName(), name, Qt::CaseInsensitive) == 0) {
            return p;
        }
    }

    return PProperty();
}

quintptr Element::getIdPropertyByName(const QString &name) const
{
    PProperty p = getPropertyByName(name);
    if (!p)
        return 0;

    return p->getId();
}

PProperty Element::addProperty(PProperty property)
{
    m_properties.append(property);
    return property;
}

void Element::removeProperty(uint index)
{
    m_points.remove(index);
}
