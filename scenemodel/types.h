#pragma once

//Project

//STL

//Qt
#include <QVector>
#include <QMap>
#include <QMetaType>

class Value;
typedef Value *PValue;
typedef QMap<quintptr, PValue> MapValues;
typedef QSharedPointer<Value> SharedValue;
typedef QVector<SharedValue> Values;

class Property;
typedef Property *PProperty;
typedef QMap<quintptr, PProperty> MapProperties;
typedef QVector<PProperty> Properties;

class Point;
typedef Point *PPoint;
typedef QVector<PPoint> Points;
typedef QMap<quintptr, PPoint> MapPoints;

class Element;
typedef Element *PElement;
typedef QVector<PElement> Elements;
typedef QMap<quintptr, PElement> MapElements;

class Container;
typedef Container *PContainer;
typedef QVector<PContainer> Containers;
typedef QMap<quintptr, PContainer> MapContainers;

class SceneModel;
typedef SceneModel *PSceneModel;
