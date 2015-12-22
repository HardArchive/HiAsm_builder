//Project
#include "emulatecgt.h"
#include "cgt.h"
#include "scenemodel/container.h"
#include "scenemodel/element.h"
#include "scenemodel/point.h"
#include "scenemodel/property.h"
#include "scenemodel/valuetypes.h"

//STL

//Qt

namespace EmulateCgt
{
#define EXPORT static __stdcall

    //ru Храним указатель на модель данных
    static SceneModel *m_model = nullptr;

    //~~~~~~~~~~~~~~~~~ Проксированные функции ~~~~~~~~~~~~~~~~~~~

    //!~~~~~~~~~~~~~~~~~~~~~~~~ контейнер ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает количество элементов в контейнере.
    EXPORT uint sdkGetCount(quintptr id_sdk)
    {
        return m_model->getCountElementsInContainer(id_sdk);
    }

    //ru Возвращает ID элемента по его Z-положению(индексу) в контейнере.
    EXPORT quintptr sdkGetElement(quintptr id_sdk, int index)
    {
        return m_model->getIdElementFromSDKByIndex(id_sdk, index);
    }

    //ru Возвращает ID элемента по имени элемента.
    EXPORT quintptr sdkGetElementName(quintptr id_sdk, char *name)
    {
        const PContainer c = m_model->getContainerById(id_sdk);
        if (!c)
            return 0;

        return c->getIdElementByName(QString::fromLocal8Bit(name));
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ элемент ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает флаги элемента.
    EXPORT ElementFlags elGetFlag(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return ELEMENT_FLG_IS_FREEZE;

        return e->getFlags();
    }

    //ru Возвращает количество свойств элемента.
    EXPORT int elGetPropCount(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        return e->getCountProps();
    }

    //ru Возвращает id свойства элемента по индексу, с порядковым номером из INI.
    EXPORT quintptr elGetProperty(quintptr id_element, int index)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        return e->getIdPropertyByIndex(index);
    }

    //ru Возвращает True, если значение свойства совпадает с заданным в INI файле, иначе False.
    EXPORT bool elIsDefProp(quintptr id_element, int index)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return false;

        PProperty p = e->getPropertyByIndex(index);
        if (!p)
            return false;

        return p->getIsDefProp();
    }

    //ru Присваиваем элементу уникальное имя и возвращаем ID этого элемента.
    EXPORT quintptr elSetCodeName(quintptr id_element, const char *name)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        e->setCodeName(QString::fromLocal8Bit(name));
        return id_element;
    }

    //ru Возвращает уникальное имя элемента
    EXPORT const char *elGetCodeName(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return nullptr;

        return fcgt::strToCString(e->getCodeName());
    }

    //ru Возвращает имя класса элемента
    EXPORT const char *elGetClassName(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return nullptr;

        return fcgt::strToCString(e->getClassName());
    }

    //ru Возвращает водержимое поля Sub из конфигурационного INI-файла элемента.
    EXPORT const char *elGetInfSub(quintptr id_element)
    {
        Q_UNUSED(id_element)
        return 0;
    }

    //ru Возвращает общее количество видимых точек у элемента.
    EXPORT int elGetPtCount(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        return e->getCountPoints();
    }

    //ru Возвращает ID точки по её индексу.
    EXPORT quintptr elGetPt(quintptr id_element, int index)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        return e->getIdPointByIndex(index);
    }

    //ru Возвращает ID точки по её имени.
    EXPORT quintptr elGetPtName(quintptr id_element, const char *name)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        return e->getIdPointByName(QString::fromLocal8Bit(name));
    }

    //ru Возвращает индекс класса элемента.
    EXPORT ElementClass elGetClassIndex(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return CI_Element;

        return e->getClassIndex();
    }

    //ru Получаем ID контейнера из элемента.
    //ru Если элемент не содержит контейнеры, возвращаем ID элемента родителя текущего контейнера.
    EXPORT quintptr elGetSDK(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        const PContainer c = e->getContainer();
        if (!c) {
            const PElement p = qobject_cast<PElement>(e->parent()->parent());
            if (p) {
                return p->getId();
            }
        }

        return c->getId();
    }

    //ru Возвращает True, если данный элемент является ссылкой, либо на него ссылаются.
    EXPORT bool elLinkIs(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return false;

        return e->getLinkIs();
    }

    //ru Возвращает ID главного элемента(тот, на который ссылаются другие).
    EXPORT quintptr elLinkMain(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        return e->getLinkIs();
    }

    //ru Помещает в переменные "X" и "Y", позицию элемента в редакторе схем.
    EXPORT void elGetPos(quintptr id_element, int &X, int &Y)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return;

        X = e->getPosX();
        Y = e->getPosY();
    }

    //ru Помещает в переменные "w" и "h", размеры элемента.
    EXPORT void elGetSize(quintptr id_element, int &W, int &H)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return;

        W = e->getSizeW();
        H = e->getSizeH();
    }

    //ru Возвращает внутренний ID элемента (отличается от внешнего).
    //[deprecated]
    EXPORT int elGetEID(quintptr id_element)
    {
        Q_UNUSED(id_element)
        return 0;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ точки элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Получаем ID точки с которой соединена указанная.
    EXPORT quintptr ptGetLinkPoint(quintptr id_point)
    {
        const PPoint p = m_model->getPointById(id_point);
        if (!p)
            return 0;

        return p->getLinkPoint();
    }

    //ru Получаем ID точки с которой соединена указанная,
    //ru без учета точек разрыва и хабов.
    EXPORT quintptr ptGetRLinkPoint(quintptr id_point)
    {
        const PPoint p = m_model->getPointById(id_point);
        if (!p)
            return 0;

        return p->getRLinkPoint();
    }

    //ru Получаем тип точки.
    EXPORT PointTypes ptGetType(quintptr id_point)
    {
        const PPoint p = m_model->getPointById(id_point);
        if (!p)
            return pt_Work;

        return p->getType();
    }

    //ru Возвращает имя точки.
    EXPORT const char *ptGetName(quintptr id_point)
    {
        const PPoint p = m_model->getPointById(id_point);
        if (!p)
            return nullptr;

        return fcgt::strToCString(p->getName());
    }

    //ru Получаем ID родителя (элемент) точки.
    EXPORT quintptr ptGetParent(quintptr id_point)
    {
        const PPoint p = m_model->getPointById(id_point);
        if (!p)
            return 0;
        const PElement e = qobject_cast<PElement>(p->parent());

        return e->getId();
    }

    //ru Получаем индекс точки относительно точек того же типа.
    EXPORT int ptGetIndex(quintptr id_point)
    {
        const PPoint p = m_model->getPointById(id_point);
        if (!p)
            return 0;

        return p->getIndex();
    }

    //ru Возвращает базовую часть имени динамической точки(для CI_DPElement).
    EXPORT const char *pt_dpeGetName(quintptr id_point)
    {
        const PPoint p = m_model->getPointById(id_point);
        if (!p)
            return nullptr;

        return fcgt::strToCString(p->getDpeName());
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ свойства элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает тип свойства.
    EXPORT DataTypes propGetType(quintptr id_prop)
    {
        PProperty p = m_model->getPropertyById(id_prop);
        if (!p)
            return data_null;

        return p->getType();
    }

    //ru Возвращает имя свойства.
    EXPORT const char *propGetName(quintptr id_prop)
    {
        PProperty p = m_model->getPropertyById(id_prop);
        if (!p)
            return nullptr;

        return fcgt::strToCString(p->getName());
    }

    //ru Возвращает значение свойства в виде указателя на данные.
    EXPORT quintptr propGetValue(quintptr id_prop)
    {
        PProperty p = m_model->getPropertyById(id_prop);
        if (!p)
            return 0;

        const PValue v = p->getValue();
        if (!v)
            return 0;

        return v->getId();
    }

    //ru Возвращает значение свойства в формате uchar.
    EXPORT uchar propToByte(quintptr id_prop)
    {
        if (id_prop == 1) {
            const SharedValue v = m_model->getPropArrayValue();
            if (!v)
                return 0;

            return v->toByte();
        }

        PProperty p = m_model->getPropertyById(id_prop);
        if (!p)
            return 0;

        return p->toByte();
    }

    //ru Возвращает значение свойства в формате int.
    EXPORT int propToInteger(quintptr id_prop)
    {
        if (id_prop == 1) {
            const SharedValue v = m_model->getPropArrayValue();
            if (!v)
                return 0;

            return v->toInt();
        }

        PProperty p = m_model->getPropertyById(id_prop);
        if (!p)
            return 0;

        return p->toInt();
    }

    //ru Возвращает значение свойства в формате float.
    EXPORT qreal propToReal(quintptr id_prop)
    {
        if (id_prop == 1) {
            const SharedValue v = m_model->getPropArrayValue();
            if (!v)
                return 0;

            return v->toReal();
        }

        PProperty p = m_model->getPropertyById(id_prop);
        if (!p)
            return 0;

        return p->toReal();
    }

    //ru Возвращает значение свойства в виде C строки.
    EXPORT const char *propToString(quintptr id_prop)
    {
        if (id_prop == 1) {
            const SharedValue v = m_model->getPropArrayValue();
            if (!v)
                return 0;

            return fcgt::strToCString(v->toString());
        }

        PProperty p = m_model->getPropertyById(id_prop);
        if (!p)
            return 0;

        return fcgt::strToCString(p->toString());
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ ресурсы ~~~~~~~~~~~~~~~~~~~~~~~~~~

    //ru Добавляет имя файла в список временных файлов
    //ru для последующего удаления файла.
    EXPORT int resAddFile(const char *filePath)
    {
        return m_model->addResList(QString::fromLocal8Bit(filePath));
    }

    //ru Добавляет иконку в ресурсы и в список временных файлов,
    //ru и возвращают имя временного файла.
    EXPORT const char *resAddIcon(quintptr id_prop)
    {
        return m_model->addStreamRes(id_prop);
    }

    //ru Добавляет строку в ресурсы и в список временных файлов.
    //ru Возвращают имя временного файла.
    EXPORT const char *resAddStr(const char *string)
    {
        return m_model->addStringRes(QString::fromLocal8Bit(string));
    }

    //ru Добавляет поток (данные) в ресурсы и в список временных файлов,
    //ru и возвращает имя временного файла.
    //ru Временный файл создаётся в папке %HiAsm%\compiler и существует до конца
    //ru работы с библиотекой.
    EXPORT const char *resAddStream(quintptr id_prop)
    {
        return m_model->addStreamRes(id_prop);
    }

    //ru Добавляет звук в ресурсы и в список временных файлов,
    //ru и возвращает имя временного файла.
    //ru Временный файл создаётся в папке %HiAsm%\compiler и существует до конца
    //ru работы с библиотекой.
    EXPORT const char *resAddWave(quintptr id_prop)
    {
        return m_model->addStreamRes(id_prop);
    }

    //ru Добавляет картинку в ресурсы и в список временных файлов,
    //ru и возвращает имя временного файла.
    //ru Временный файл создаётся в папке %HiAsm%\compiler и существует до конца
    //ru работы с библиотекой.
    EXPORT const char *resAddBitmap(quintptr id_prop)
    {
        return m_model->addStreamRes(id_prop);
    }

    //ru Добавляет меню в ресурсы и в список временных файлов.
    //[deprecated]
    EXPORT const char *resAddMenu(quintptr id_prop)
    {
        Q_UNUSED(id_prop)
        return nullptr;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ информационные сообщения ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Выводит строку text в окно Отладка цветом color
    //ru Всего возвращает 0.
    EXPORT int _Debug(const char *text, int color)
    {
        Q_UNUSED(color)

        qDebug("%s", text);
        return 0;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ среда ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru На основе индекса параметра, записываем данные в память value
    EXPORT int GetParam(CgtParams index, void *value)
    {
        m_model->getCgtParam(index, value);
        return 0;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ массив ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Получаем количество элементов в массиве.
    EXPORT int arrCount(quintptr id_array)
    {
        const PValue v = m_model->getValueById(id_array);
        if (!v)
            return 0;

        return v->getArraySize();
    }

    //ru Получаем тип элементов в массиве.
    EXPORT DataTypes arrType(quintptr id_array)
    {
        const PValue v = m_model->getValueById(id_array);
        if (!v)
            return data_null;

        return v->getSubType();
    }

    //ru Получаем имя элемента по индексу.
    EXPORT const char *arrItemName(quintptr id_array, int index)
    {
        const PValue v = m_model->getValueById(id_array);
        if (!v)
            return nullptr;

        return fcgt::strToCString(v->getArrayItemName(index));
    }

    //ru Получаем элемент (id_arrayValue) массива  по индексу.
    //[deprecated]
    EXPORT quintptr arrItemData(quintptr id_array, int index)
    {
        Q_UNUSED(id_array)
        Q_UNUSED(index)
        return 0;
    }

    //ru Получаем элемент массива в виде свойства (id_prop) Оо,
    //ru для дальнейшей работы с ним cgt::prop* функциями.
    EXPORT quintptr arrGetItem(quintptr id_array, int index)
    {
        const PValue v = m_model->getValueById(id_array);
        if (!v)
            return 0;

        const SharedValue arrValue = v->getArrayItemByIndex(index);
        if (arrValue)
            m_model->setPropArrayValue(arrValue);

        return 1;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ среда ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращаем >0, если запускаем схемы в режиме отладки,
    //ru иначе 0.
    EXPORT int isDebug(quintptr id_element)
    {
        Q_UNUSED(id_element)
        return m_model->getIsDebug();
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ работа с данными ~~~~~~~~~~~~~~~~~~~~~~~~~~

    //ru Возвращает тип данных.
    EXPORT DataTypes dtType(quintptr id_value)
    {
        const PValue v = m_model->getValueById(id_value);
        if (!v)
            return data_null;

        return v->getDataType();
    }

    //ru Возвращает данные в формате: строка в стиле C.
    EXPORT const char *dtStr(quintptr id_value)
    {
        const PValue v = m_model->getValueById(id_value);
        if (!v)
            return nullptr;

        return fcgt::strToCString(v->toString());
    }

    //ru Возвращает данные в формате: целое число.
    EXPORT int dtInt(quintptr id_value)
    {
        const PValue v = m_model->getValueById(id_value);
        if (!v)
            return 0;

        return v->toInt();
    }

    //ru Возвращает данные в формате: число с плавающей запятой.
    EXPORT qreal dtReal(quintptr id_value)
    {
        const PValue v = m_model->getValueById(id_value);
        if (!v)
            return 0.0;

        return v->toReal();
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ шрифт ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает название шрифта.
    EXPORT const char *fntName(quintptr id_font)
    {
        PValue v = m_model->getValueById(id_font);
        if (!v)
            return nullptr;

        SharedValueFont font = v->toFont();
        if (!font)
            return nullptr;

        return fcgt::strToCString(font->name);
    }
    //ru Возвращает размер шрифта.
    EXPORT int fntSize(quintptr id_font)
    {
        PValue v = m_model->getValueById(id_font);
        if (!v)
            return 0;

        SharedValueFont font = v->toFont();
        if (!font)
            return 0;

        return font->size;
    }
    //ru Возвращает стиль шрифта.
    EXPORT uchar fntStyle(quintptr id_font)
    {
        PValue v = m_model->getValueById(id_font);
        if (!v)
            return 0;

        SharedValueFont font = v->toFont();
        if (!font)
            return 0;

        return font->style;
    }
    //ru Возвращает цвет шрифта.
    EXPORT uint fntColor(quintptr id_font)
    {
        PValue v = m_model->getValueById(id_font);
        if (!v)
            return 0;

        SharedValueFont font = v->toFont();
        if (!font)
            return 0;

        return font->color;
    }
    //ru Возвращает кодировку шрифта.
    EXPORT uchar fntCharSet(quintptr id_font)
    {
        PValue v = m_model->getValueById(id_font);
        if (!v)
            return 0;

        SharedValueFont font = v->toFont();
        if (!font)
            return 0;

        return font->charset;
    }

    //!~~~~~~~~~~~~~~~~элемент | пользовательские данные ~~~~~~~~~~~~~
    //!ru Судя по CodeGen.dpr, используется для хранения указателя (ID элемента) на самого себя.

    //ru Возвращает пользовательские данные элемента.
    //ru Коммент из hiasm5 - user data used in FTCG codegen.
    //ru Судя по всему, данные могут быть любого типа, ибо хранит указатель..
    //ru Так же, скорее всего используется для хранения ID элемента
    EXPORT quintptr elGetData(quintptr id_element)
    {
        PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        return e->getUserData();
    }

    //ru Устанавливает пользовательские данные элементу.
    //ru Коммент из hiasm5 - user data used in FTCG codegen.
    //ru Судя по всему, данные могут быть любого типа, ибо хранит указатель.
    //ru Так же, скорее всего используется для хранения ID элемента
    EXPORT void elSetData(quintptr id_element, quintptr data)
    {
        PElement e = m_model->getElementById(id_element);
        if (e)
            e->setUserData(data);
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ точки элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает тип данных точки.
    EXPORT DataTypes ptGetDataType(quintptr id_point)
    {
        const PPoint p = m_model->getPointById(id_point);
        if (!p)
            return data_null;

        return p->getDataType();
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ элемент ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает ID родительского контейнера элемента.
    EXPORT quintptr elGetParent(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        const PContainer c = e->getParent();
        if (!c)
            return 0;

        return c->getId();
    }

    //ru Возвращает количество свойств в списке свойств(из панели свойств).
    //[deprecated]
    EXPORT int elGetPropertyListCount(quintptr id_element)
    {
        Q_UNUSED(id_element)

        return 0;
    }

    //ru Возвращает свойство из списка свойств (PropertyList).
    //[deprecated]
    EXPORT quintptr elGetPropertyListItem(quintptr id_element, int index)
    {
        Q_UNUSED(id_element)
        Q_UNUSED(index)

        return 0;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ список свойств элемента ~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает имя свойства.
    //[deprecated]
    EXPORT const char *plGetName(quintptr id_prop)
    {
        Q_UNUSED(id_prop)
        return nullptr;
    }

    //ru Возвращает описание свойства.
    //[deprecated]
    EXPORT const char *plGetInfo(quintptr id_prop)
    {
        Q_UNUSED(id_prop)
        return nullptr;
    }

    //ru Возвращает группу свойсва.
    //[deprecated]
    EXPORT const char *plGetGroup(quintptr id_prop)
    {
        Q_UNUSED(id_prop)
        return nullptr;
    }

    //ru Возвращает указатель на данные свойства.
    //[deprecated]
    EXPORT quintptr plGetProperty(quintptr id_prop)
    {
        Q_UNUSED(id_prop)
        return 0;
    }

    //ru Возвращает ID родительского элемента указанного свойства.
    //[deprecated]
    EXPORT quintptr plGetOwner(quintptr id_prop)
    {
        Q_UNUSED(id_prop)
        return 0;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ точки элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает описание точки.
    //[deprecated]
    EXPORT const char *ptGetInfo(quintptr id_point)
    {
        const PPoint p = m_model->getPointById(id_point);
        if (!p)
            return nullptr;

        return fcgt::strToCString(p->getInfo());
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ свойства элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает ID элемента, прилинкованного к указанному свойству.
    EXPORT quintptr propGetLinkedElement(quintptr id_element, const char *propName)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        PProperty p = e->getPropertyByName(QString::fromLocal8Bit(propName));
        if (!p)
            return 0;

        const SharedLinkedElementInfo info = p->toLinkedElementInfo();
        if (!info)
            return 0;

        return info->id;
    }

    //ru Возвращает 1, если свойство помечено на перевод.
    //[deprecated]
    EXPORT int propIsTranslate(quintptr id_element, quintptr id_prop)
    {
        Q_UNUSED(id_element)
        Q_UNUSED(id_prop)
        return 0;
    }

    /*
     * Возвращает ID элемента, прилинкованного к указанному свойству.
     * В буфер buf пишется имя интерфейса элемента.
     * Например в строке из INI: FormFastening=Форма для привязки позиции|20|(empty)|ControlManager
     * ControlManager - является той самой информацией передаваемой нам в buf.
     */
    EXPORT quintptr propGetLinkedElementInfo(quintptr id_element, quintptr id_prop, char *buf)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        PProperty p = e->getPropertyById(id_prop);
        if (!p)
            return 0;

        const SharedLinkedElementInfo info = p->toLinkedElementInfo();
        if (!info)
            return 0;

        strcpy(buf, info->interface.toStdString().c_str());
        return info->id;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ элемент - CI_PolyMulti ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает id_sdk контейнера по его индексу из элемента.
    EXPORT quintptr elGetSDKByIndex(quintptr id_element, int index)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        return e->getIdContainerByIndex(index);
    }

    //ru Возвращает количаство контейнеров полиморфного элемента(CI_PolyMulti).
    EXPORT int elGetSDKCount(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return 0;

        return e->getCountContainers();
    }

    //ru Возвращает имя контейнера по индексу.
    EXPORT const char *elGetSDKName(quintptr id_element, int index)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return nullptr;

        const PContainer c = e->getContainerByIndex(index);
        if (!c)
            return nullptr;

        return fcgt::strToCString(c->getName());
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ схема ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает элемент родитель для данного SDK.
    //ru Возвращает 0, если контейнер не имеет родителя.
    EXPORT quintptr sdkGetParent(quintptr id_sdk)
    {
        const PContainer c = m_model->getContainerById(id_sdk);
        if (!c)
            return 0;

        PElement e = c->getParent();
        if (!e)
            return 0;

        return e->getId();
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ элемент ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает интерфейсы, предоставляемые элементом.
    //ru Содержимое поля Interfaces= из конфигурации элемента.
    EXPORT const char *elGetInterface(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return nullptr;

        return fcgt::strToCString(e->getInterface());
    }

    //ru Возвращает список классов, от которых наследуется элемент
    //ru Содержимое поля Inherit= из конфигурации элемента.
    EXPORT const char *elGetInherit(quintptr id_element)
    {
        const PElement e = m_model->getElementById(id_element);
        if (!e)
            return nullptr;

        return fcgt::strToCString(e->getInherit());
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ ресурсы ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает 1, если список ресурсов пуст, и 0 в противном случае.
    EXPORT int resEmpty()
    {
        return m_model->resIsEmpty();
    }

    //ru Устанавливает префикс для имен ресурсов.
    //[deprecated]
    EXPORT int resSetPref(const char *pref)
    {
        Q_UNUSED(pref)
        return 0;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ информационные сообщения ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Добавляет информацию в информационную панель
    EXPORT int _Error(int line, quintptr id_element, const char *text)
    {
        Q_UNUSED(line)
        Q_UNUSED(id_element)
        Q_UNUSED(text)
        return 0;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ элемент ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Возвращает ID группы, к которой принадлежит элемент и 0, если группа отсутствует
    //[deprecated]
    EXPORT int elGetGroup(quintptr id_element)
    {
        Q_UNUSED(id_element)
        return 0;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ свойства элемента ~~~~~~~~~~~~~~~~~~~~~~~~~~
    //ru Сохраняет данные свойства в файл.
    //[deprecated]
    EXPORT int propSaveToFile(quintptr id_prop, const char *fileName)
    {
        Q_UNUSED(id_prop)
        Q_UNUSED(fileName)

        return 0;
    }

    //Заполняем массив указателей
    static uintptr_t arrayPointers[] {
        reinterpret_cast<quintptr>(sdkGetCount),
        reinterpret_cast<quintptr>(sdkGetElement),
        reinterpret_cast<quintptr>(sdkGetElementName),
        reinterpret_cast<quintptr>(elGetFlag),
        reinterpret_cast<quintptr>(elGetPropCount),
        reinterpret_cast<quintptr>(elGetProperty),
        reinterpret_cast<quintptr>(elIsDefProp),
        reinterpret_cast<quintptr>(elSetCodeName),
        reinterpret_cast<quintptr>(elGetCodeName),
        reinterpret_cast<quintptr>(elGetClassName),
        reinterpret_cast<quintptr>(elGetInfSub),
        reinterpret_cast<quintptr>(elGetPtCount),
        reinterpret_cast<quintptr>(elGetPt),
        reinterpret_cast<quintptr>(elGetPtName),
        reinterpret_cast<quintptr>(elGetClassIndex),
        reinterpret_cast<quintptr>(elGetSDK),
        reinterpret_cast<quintptr>(elLinkIs),
        reinterpret_cast<quintptr>(elLinkMain),
        reinterpret_cast<quintptr>(elGetPos),
        reinterpret_cast<quintptr>(elGetSize),
        reinterpret_cast<quintptr>(elGetEID),
        reinterpret_cast<quintptr>(ptGetLinkPoint),
        reinterpret_cast<quintptr>(ptGetRLinkPoint),
        reinterpret_cast<quintptr>(ptGetType),
        reinterpret_cast<quintptr>(ptGetName),
        reinterpret_cast<quintptr>(ptGetParent),
        reinterpret_cast<quintptr>(ptGetIndex),
        reinterpret_cast<quintptr>(pt_dpeGetName),
        reinterpret_cast<quintptr>(propGetType),
        reinterpret_cast<quintptr>(propGetName),
        reinterpret_cast<quintptr>(propGetValue),
        reinterpret_cast<quintptr>(propToByte),
        reinterpret_cast<quintptr>(propToInteger),
        reinterpret_cast<quintptr>(propToReal),
        reinterpret_cast<quintptr>(propToString),
        reinterpret_cast<quintptr>(resAddFile),
        reinterpret_cast<quintptr>(resAddIcon),
        reinterpret_cast<quintptr>(resAddStr),
        reinterpret_cast<quintptr>(resAddStream),
        reinterpret_cast<quintptr>(resAddWave),
        reinterpret_cast<quintptr>(resAddBitmap),
        reinterpret_cast<quintptr>(resAddMenu),
        reinterpret_cast<quintptr>(_Debug),
        reinterpret_cast<quintptr>(GetParam),
        reinterpret_cast<quintptr>(arrCount),
        reinterpret_cast<quintptr>(arrType),
        reinterpret_cast<quintptr>(arrItemName),
        reinterpret_cast<quintptr>(arrItemData),
        reinterpret_cast<quintptr>(arrGetItem),
        reinterpret_cast<quintptr>(isDebug),
        reinterpret_cast<quintptr>(dtType),
        reinterpret_cast<quintptr>(dtStr),
        reinterpret_cast<quintptr>(dtInt),
        reinterpret_cast<quintptr>(dtReal),
        reinterpret_cast<quintptr>(fntName),
        reinterpret_cast<quintptr>(fntSize),
        reinterpret_cast<quintptr>(fntStyle),
        reinterpret_cast<quintptr>(fntColor),
        reinterpret_cast<quintptr>(fntCharSet),
        reinterpret_cast<quintptr>(elGetData),
        reinterpret_cast<quintptr>(elSetData),
        reinterpret_cast<quintptr>(ptGetDataType),
        reinterpret_cast<quintptr>(elGetParent),
        reinterpret_cast<quintptr>(elGetPropertyListCount),
        reinterpret_cast<quintptr>(elGetPropertyListItem),
        reinterpret_cast<quintptr>(plGetName),
        reinterpret_cast<quintptr>(plGetInfo),
        reinterpret_cast<quintptr>(plGetGroup),
        reinterpret_cast<quintptr>(plGetProperty),
        reinterpret_cast<quintptr>(plGetOwner),
        reinterpret_cast<quintptr>(ptGetInfo),
        reinterpret_cast<quintptr>(propGetLinkedElement),
        reinterpret_cast<quintptr>(propIsTranslate),
        reinterpret_cast<quintptr>(propGetLinkedElementInfo),
        reinterpret_cast<quintptr>(elGetSDKByIndex),
        reinterpret_cast<quintptr>(elGetSDKCount),
        reinterpret_cast<quintptr>(elGetSDKName),
        reinterpret_cast<quintptr>(sdkGetParent),
        reinterpret_cast<quintptr>(elGetInterface),
        reinterpret_cast<quintptr>(elGetInherit),
        reinterpret_cast<quintptr>(resEmpty),
        reinterpret_cast<quintptr>(resSetPref),
        reinterpret_cast<quintptr>(_Error),
        reinterpret_cast<quintptr>(elGetGroup),
        reinterpret_cast<quintptr>(propSaveToFile),
    };

    /*!  Служебные функции   */
    //Сохранение указателя для дальнейшей работы с оным
    void setSceneModel(SceneModel *collector)
    {
        m_model = collector;
    }

    //Получаем массив указателей на функции
    PCodeGenTools getCgt()
    {
        return reinterpret_cast<PCodeGenTools>(arrayPointers);
    }
}
