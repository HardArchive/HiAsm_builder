//Project
#include "proxycgt.h"

//STL

//Qt
#include <QDebug>
#include <QFlags>

namespace ProxyCgt
{
    //Дефайны
#define EXPORT __stdcall
#define PRINT_FUNC_INFO qInfo("  Call: %s", Q_FUNC_INFO);
#define PRINT_RESULT(X) qInfo() << qUtf8Printable(QString("  Return: %1").arg(X));

#define id(X) QString("id(%1)").arg(X)
#define str(X) QString("\"%1\"").arg(X)
#define p(X) QString("p(%1)").arg(X)

    //Для хранения указателя на массив указателей на callback функции
    static PCodeGenTools m_cgt = nullptr;

    //Служебные функции
    void printArgs(const std::initializer_list<QVariant> &args)
    {
        uint numArg = 1;
        for (const QVariant &v : args) {
            qInfo("  Arg%d: %s", numArg, qUtf8Printable(v.toString()));
            ++numArg;
        }
    }
    void printParamArgs(CgtParams index, const QVariant &value)
    {
        qInfo("  Arg1: %s", qUtf8Printable(CgtParamsMap[index]));
        if (value.type() == QVariant::String) {
            qInfo("  Arg2: \"%s\"", qUtf8Printable(value.toString()));
        } else {
            qInfo("  Arg2: %s", qUtf8Printable(value.toString()));
        }
    }


    //~~~~~~~~~~~~~~~~~ Проксированные функции ~~~~~~~~~~~~~~~~~~~

    EXPORT int sdkGetCount(quintptr id_sdk)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->sdkGetCount(id_sdk);
        printArgs({id(id_sdk)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT quintptr sdkGetElement(quintptr id_sdk, int index)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->sdkGetElement(id_sdk, index);
        printArgs({id(id_sdk), index});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT quintptr sdkGetElementName(quintptr id_sdk, char *name)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->sdkGetElementName(id_sdk, name);
        printArgs({id(id_sdk), str(name)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT ElementFlags elGetFlag(quintptr id_element)
    {
        PRINT_FUNC_INFO
        ElementFlags res = m_cgt->elGetFlag(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(ElementFlgs(res))

        return res;
    }
    EXPORT int elGetPropCount(quintptr id_element)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->elGetPropCount(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT quintptr elGetProperty(quintptr id_element, int index)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->elGetProperty(id_element, index);
        printArgs({id(id_element), index});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT bool elIsDefProp(quintptr id_element, int index)
    {
        PRINT_FUNC_INFO
        bool res = m_cgt->elIsDefProp(id_element, index);
        printArgs({id(id_element), index});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT quintptr elSetCodeName(quintptr id_element, const char *name)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->elSetCodeName(id_element, name);
        printArgs({id(id_element), str(name)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT const char *elGetCodeName(quintptr id_element)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->elGetCodeName(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT const char *elGetClassName(quintptr id_element)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->elGetClassName(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT const char *elGetInfSub(quintptr id_element)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->elGetInfSub(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT int elGetPtCount(quintptr id_element)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->elGetPtCount(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT quintptr elGetPt(quintptr id_element, int index)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->elGetPt(id_element, index);
        printArgs({id(id_element), index});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT quintptr elGetPtName(quintptr id_element, const char *name)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->elGetPtName(id_element, name);
        printArgs({id(id_element), str(name)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT ElementClass elGetClassIndex(quintptr id_element)
    {
        PRINT_FUNC_INFO
        ElementClass res = m_cgt->elGetClassIndex(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(ElementClassMap[res])

        return res;
    }
    EXPORT quintptr elGetSDK(quintptr id_element)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->elGetSDK(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT bool elLinkIs(quintptr id_element)
    {
        PRINT_FUNC_INFO
        bool res = m_cgt->elLinkIs(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT quintptr elLinkMain(quintptr id_element)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->elLinkMain(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT void elGetPos(quintptr id_element, int &X, int &Y)
    {
        PRINT_FUNC_INFO
        m_cgt->elGetPos(id_element, X, Y);
        printArgs({id(id_element), X, Y});
    }
    EXPORT void elGetSize(quintptr id_element, int &W, int &H)
    {
        PRINT_FUNC_INFO
        m_cgt->elGetSize(id_element, W, H);
        printArgs({id(id_element), W, H});
    }
    EXPORT int elGetEID(quintptr id_point)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->elGetEID(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT quintptr ptGetLinkPoint(quintptr id_point)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->ptGetLinkPoint(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT quintptr ptGetRLinkPoint(quintptr id_point)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->ptGetRLinkPoint(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT PointTypes ptGetType(quintptr id_point)
    {
        PRINT_FUNC_INFO
        PointTypes res = m_cgt->ptGetType(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(PointTypesMap[res])

        return res;
    }
    EXPORT const char *ptGetName(quintptr id_point)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->ptGetName(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT quintptr ptGetParent(quintptr id_point)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->ptGetParent(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT int ptGetIndex(quintptr id_point)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->ptGetIndex(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT const char *pt_dpeGetName(quintptr id_point)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->pt_dpeGetName(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT DataType propGetType(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        DataType res = m_cgt->propGetType(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(DataTypesMap[res])

        return res;
    }
    EXPORT const char *propGetName(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->propGetName(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT quintptr propGetValue(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->propGetValue(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT uchar propToByte(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        uchar res = m_cgt->propToByte(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT int propToInteger(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->propToInteger(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT qreal propToReal(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        float res = m_cgt->propToReal(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT const char *propToString(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->propToString(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT int resAddFile(const char *name)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->resAddFile(name);
        printArgs({str(name)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT const char *resAddIcon(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->resAddIcon(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT const char *resAddStr(const char *str)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->resAddStr(str);
        printArgs({str});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT const char *resAddStream(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->resAddStream(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT const char *resAddWave(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->resAddWave(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT const char *resAddBitmap(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->resAddBitmap(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT const char *resAddMenu(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->resAddMenu(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT int _Debug(const char *text, int color)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->_Debug(text, color);
        printArgs({str(text), color});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT int GetParam(CgtParams index, void *value)
    {
        PRINT_FUNC_INFO
        if (index > 12)
            index = PARAM_DEBUG_MODE;

        int res = m_cgt->GetParam(index, value);

        switch (index) {
        case PARAM_CODE_PATH :
            printParamArgs(index, reinterpret_cast<const char *>(value));
            break;
        case PARAM_DEBUG_MODE:
            printParamArgs(index, *reinterpret_cast<const int *>(value));
            break;
        case PARAM_DEBUG_SERVER_PORT:
            printParamArgs(index, *reinterpret_cast<const int *>(value));
            break;
        case PARAM_DEBUG_CLIENT_PORT:
            printParamArgs(index, *reinterpret_cast<const int *>(value));
            break;
        case PARAM_PROJECT_PATH:
            printParamArgs(index, reinterpret_cast<const char *>(value));
            break;
        case PARAM_HIASM_VERSION:
            printParamArgs(index, reinterpret_cast<const char *>(value));
            break;
        case PARAM_USER_NAME:
            printParamArgs(index, reinterpret_cast<const char *>(value));
            break;
        case PARAM_USER_MAIL:
            printParamArgs(index, reinterpret_cast<const char *>(value));
            break;
        case PARAM_PROJECT_NAME:
            printParamArgs(index, reinterpret_cast<const char *>(value));
            break;
        case PARAM_SDE_WIDTH:
            printParamArgs(index, *reinterpret_cast<const int *>(value));
            break;
        case PARAM_SDE_HEIGHT:
            printParamArgs(index, *reinterpret_cast<const int *>(value));
            break;
        case PARAM_COMPILER:
            printParamArgs(index, reinterpret_cast<const char *>(value));
            break;
        }

        PRINT_RESULT(res)

        return res;
    }
    EXPORT int arrCount(quintptr id_value)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->arrCount(id_value);
        printArgs({id(id_value)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT DataType arrType(quintptr id_value)
    {
        PRINT_FUNC_INFO
        DataType res = m_cgt->arrType(id_value);
        printArgs({id(id_value)});
        PRINT_RESULT(DataTypesMap[res])

        return res;
    }
    EXPORT const char *arrItemName(quintptr id_value, int index)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->arrItemName(id_value, index);
        printArgs({id(id_value), index});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT quintptr arrItemData(quintptr id_value, int index)
    {
        PRINT_FUNC_INFO
        const quintptr res = m_cgt->arrItemData(id_value, index);
        printArgs({id(id_value), index});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT quintptr arrGetItem(quintptr id_value, int index)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->arrGetItem(id_value, index);
        printArgs({id(id_value), index});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT int isDebug(quintptr id_element)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->isDebug(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT DataType dtType(quintptr id_value)
    {
        PRINT_FUNC_INFO
        DataType res = m_cgt->dtType(id_value);
        printArgs({id(id_value)});
        PRINT_RESULT(DataTypesMap[res])

        return res;
    }
    EXPORT const char *dtStr(quintptr id_value)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->dtStr(id_value);
        printArgs({id(id_value)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT int dtInt(quintptr id_value)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->dtInt(id_value);
        printArgs({id(id_value)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT qreal dtReal(quintptr id_value)
    {
        PRINT_FUNC_INFO
        qreal res = m_cgt->dtReal(id_value);
        printArgs({id(id_value)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT const char *fntName(quintptr id_value)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->fntName(id_value);
        printArgs({id(id_value)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT int fntSize(quintptr id_value)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->fntSize(id_value);
        printArgs({id(id_value)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT uchar fntStyle(quintptr id_value)
    {
        printArgs({id(id_value)});
        PRINT_FUNC_INFO
        uchar res = m_cgt->fntStyle(id_value);
        PRINT_RESULT(res)

        return res;
    }
    EXPORT uint fntColor(quintptr id_value)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->fntColor(id_value);
        printArgs({id(id_value)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT uchar fntCharSet(quintptr id_value)
    {
        PRINT_FUNC_INFO
        uchar res = m_cgt->fntCharSet(id_value);
        printArgs({id(id_value)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT quintptr elGetData(quintptr id_element)
    {
        PRINT_FUNC_INFO
        const quintptr res = m_cgt->elGetData(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(p(res))

        return res;
    }
    EXPORT void elSetData(quintptr id_element, quintptr data)
    {
        PRINT_FUNC_INFO
        m_cgt->elSetData(id_element, data);
        printArgs({id(id_element), p(data)});
    }
    EXPORT DataType ptGetDataType(quintptr id_point)
    {
        PRINT_FUNC_INFO
        DataType res = m_cgt->ptGetDataType(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(DataTypesMap[res])

        return res;
    }
    EXPORT quintptr elGetParent(quintptr id_element)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->elGetParent(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT int elGetPropertyListCount(quintptr id_element)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->elGetPropertyListCount(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT quintptr elGetPropertyListItem(quintptr id_element, int index)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->elGetPropertyListItem(id_element, index);
        printArgs({id(id_element), index});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT const char *plGetName(quintptr id_point)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->plGetName(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT const char *plGetInfo(quintptr id_point)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->plGetInfo(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT const char *plGetGroup(quintptr id_point)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->plGetGroup(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT quintptr plGetProperty(quintptr id_point)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->plGetProperty(id_point);
        printArgs({id(id_point)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT quintptr plGetOwner(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->plGetOwner(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT const char *ptGetInfo(quintptr id_prop)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->ptGetInfo(id_prop);
        printArgs({id(id_prop)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT quintptr propGetLinkedElement(quintptr id_prop, const char *propName)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->propGetLinkedElement(id_prop, propName);
        printArgs({id(id_prop), str(propName)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT int propIsTranslate(quintptr id_element, quintptr id_prop)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->propIsTranslate(id_element, id_prop);
        printArgs({id(id_element), id(id_prop)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT quintptr propGetLinkedElementInfo(quintptr id_element, quintptr id_prop, char *info)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->propGetLinkedElementInfo(id_element, id_prop, info);
        printArgs({id(id_element), id(id_prop), str(info)});
        PRINT_RESULT(id(res))

        return 0;
    }
    EXPORT quintptr elGetSDKByIndex(quintptr id_element, int index)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->elGetSDKByIndex(id_element, index);
        printArgs({id(id_element), index});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT int elGetSDKCount(quintptr id_element)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->elGetSDKCount(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT const char *elGetSDKName(quintptr id_element, int index)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->elGetSDKName(id_element, index);
        printArgs({id(id_element), index});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT quintptr sdkGetParent(quintptr id_sdk)
    {
        PRINT_FUNC_INFO
        quintptr res = m_cgt->sdkGetParent(id_sdk);
        printArgs({id(id_sdk)});
        PRINT_RESULT(id(res))

        return res;
    }
    EXPORT const char *elGetInterface(quintptr id_element)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->elGetInterface(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT const char *elGetInherit(quintptr id_element)
    {
        PRINT_FUNC_INFO
        const char *res = m_cgt->elGetInherit(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(str(res))

        return res;
    }
    EXPORT int resEmpty()
    {
        PRINT_FUNC_INFO
        int res = m_cgt->resEmpty();
        PRINT_RESULT(res)

        return res;
    }
    EXPORT int resSetPref(const char *pref)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->resSetPref(pref);
        printArgs({str(pref)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT int _Error(int line, quintptr id_element, const char *text)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->_Error(line, id_element, text);
        printArgs({line, id(id_element), str(text)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT int elGetGroup(quintptr id_element)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->elGetGroup(id_element);
        printArgs({id(id_element)});
        PRINT_RESULT(res)

        return res;
    }
    EXPORT int propSaveToFile(quintptr id_prop, const char *fileName)
    {
        PRINT_FUNC_INFO
        int res = m_cgt->propSaveToFile(id_prop, fileName);
        printArgs({id(id_prop), str(fileName)});
        PRINT_RESULT(res)

        return res;
    }

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

    void setProxiedCgt(PCodeGenTools cgt)
    {
        m_cgt = cgt;
    }

    //Получаем массив указателей на функции
    PCodeGenTools getCgt()
    {
        return reinterpret_cast<PCodeGenTools>(arrayPointers);
    }
}
