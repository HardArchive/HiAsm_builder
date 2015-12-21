//Project
#include "container.h"
#include "element.h"
#include "scenemodel.h"
#include "cgt/cgt.h"

//STL

//Qt

Container::Container(quintptr id_sdk, QObject *parent)
    : QObject(parent)
    , m_id(id_sdk)
    , m_model(parent->property("model").value<PSceneModel>())
{
    m_model->addContainerToMap(this);
    collectingData();
}

Container::Container(const QJsonObject &object, QObject *parent)
    : QObject(parent)
    , m_model(parent->property("model").value<PSceneModel>())
{
    deserialize(object);
}

void Container::collectingData()
{
    int countElements = cgt::sdkGetCount(m_id);
    for (int i = 0; i < countElements; ++i) {
        quintptr id_element = cgt::sdkGetElement(m_id, i);

        //ru Добавляем элемент в контейнер
        addElement(new Element(id_element, this));
    }
}

QVariantMap Container::serialize()
{
    QVariantMap data;
    data.insert("id", m_id);
    data.insert("name", m_name);

    QVariantList elements;
    for (const PElement e : m_elements) {
        elements.append(e->serialize());
    }

    QVariantMap container;
    container.insert("Data", data);
    container.insert("Elements", elements);

    return container;
}

void Container::deserialize(const QJsonObject &object)
{
    const auto data = object["Data"].toObject();
    m_id = data["id"].toVariant().toUInt();
    m_model->addContainerToMap(this);
    m_name = data["name"].toString();

    const auto elements = object["Elements"].toArray();
    for(const auto e : elements){
       addElement(new Element(e.toObject(), this));
    }
}

quintptr Container::getId() const
{
    return m_id;
}

PElement Container::getParent() const
{
    return qobject_cast<PElement>(parent());
}

QString Container::getName() const
{
    return m_name;
}

PSceneModel Container::getModel() const
{
    return m_model;
}

void Container::setName(const QString &name)
{
    m_name = name;
}

size_t Container::getCountElements() const
{
    return m_elements.size();
}

PElement Container::getElementByIndex(uint index) const
{
    if (index < uint(m_elements.size()))
        return m_elements[index];

    return nullptr;
}

quintptr Container::getIdElementByIndex(uint index) const
{
    const PElement e = getElementByIndex(index);
    if (!e)
        return 0;

    return e->getId();
}

PElement Container::getElementByName(const QString &name) const
{
    for (PElement e : m_elements) {
        if (QString::compare(e->getClassName(), name, Qt::CaseInsensitive) == 0) {
            return e;
        }
    }

    return 0;
}

quintptr Container::getIdElementByName(const QString &name) const
{
    PElement e = getElementByName(name);
    if (e)
        return e->getId();

    return 0;
}

PElement Container::addElement(PElement element)
{
    m_elements.append(element);
    return element;
}

void Container::removeElement(uint index)
{
    m_elements.remove(index);
}
