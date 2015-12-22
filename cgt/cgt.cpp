//Project
#include "cgt.h"

//STL

//Qt
#include <QtCore>
#include <QFlags>

namespace fcgt
{
    bool isSelect(const ElementFlgs &flags)
    {
        return flags & ELEMENT_FLG_IS_SELECT;
    }

    bool isParent(const ElementFlgs &flags)
    {
        return flags & ELEMENT_FLG_IS_PARENT;
    }

    bool isCore(const ElementFlgs &flags)
    {
        return flags & ELEMENT_FLG_IS_CORE;
    }

    bool isNoDelete(const ElementFlgs &flags)
    {
        return flags & ELEMENT_FLG_IS_NODELETE;
    }

    bool isMulti(const ElementFlgs &flags)
    {
        return flags & ELEMENT_FLG_IS_MULTI;
    }

    bool isEdit(const ElementFlgs &flags)
    {
        return flags & ELEMENT_FLG_IS_EDIT;
    }

    bool isLink(const ElementFlgs &flags)
    {
        return flags & ELEMENT_FLG_IS_LINK;
    }


    bool isEditMultiEx(const ElementClass &eClass)
    {
        return eClass == CI_EditMultiEx;
    }

    bool isPolyMulti(const ElementClass &eClass)
    {
        return eClass == CI_PolyMulti;
    }

    const char *strToCString(const QString &str)
    {
        char *buf = new char[uint(str.size() + 1)];
        strcpy(buf, str.toStdString().c_str());
        return buf;
    }
}


namespace cgt
{
    //!~~~~~~~~~~~~~~~~~~~~~ служебные функции ~~~~~~~~~~~~~~~~~~~~~~~
    static TBuildProcessRec *m_buildProcess;
    static PCodeGenTools m_origCgt{};
    static PCodeGenTools m_cgt{};

    static quintptr m_sdk{};

    //ru Инициализация параметров
    void init(TBuildProcessRec &params)
    {
        m_buildProcess = &params;
        m_origCgt = params.cgt;
        m_cgt = params.cgt;
        m_sdk = params.sdk;
    }

    void setProxyCgt(PCodeGenTools cgt)
    {
        m_cgt = cgt;
        m_buildProcess->cgt = cgt;
    }

    PCodeGenTools getOriginalCgt()
    {
        return m_origCgt;
    }

    quintptr getMainSDK()
    {
        return m_sdk;
    }

    //!~~~~~~~~~~~~~~~~~~~~~~~~ CGT ~~~~~~~~~~~~~~~~~~~~~~~~~~
    int sdkGetCount(quintptr SDK)
    {
        return m_cgt->sdkGetCount(SDK);
    }

    quintptr sdkGetElement(quintptr SDK, int Index)
    {
        return m_cgt->sdkGetElement(SDK, Index);
    }

    quintptr sdkGetElementName(quintptr SDK, char *Name)
    {
        return m_cgt->sdkGetElementName(SDK, Name);
    }

    ElementFlags elGetFlag(quintptr e)
    {
        return m_cgt->elGetFlag(e);
    }

    int elGetPropCount(quintptr e)
    {
        return m_cgt->elGetPropCount(e);
    }

    quintptr elGetProperty(quintptr e, int Index)
    {
        return m_cgt->elGetProperty(e, Index);
    }

    bool elIsDefProp(quintptr id_element, int index)
    {
        return m_cgt->elIsDefProp(id_element, index);
    }

    quintptr elSetCodeName(quintptr e, const char *Name)
    {
        return m_cgt->elSetCodeName(e, Name);
    }

    const char *elGetCodeName(quintptr e)
    {
        return m_cgt->elGetCodeName(e);
    }

    const char *elGetClassName(quintptr e)
    {
        return m_cgt->elGetClassName(e);
    }

    const char *elGetInfSub(quintptr e)
    {
        return m_cgt->elGetInfSub(e);
    }

    int elGetPtCount(quintptr e)
    {
        return m_cgt->elGetPtCount(e);
    }

    quintptr elGetPt(quintptr e, int i)
    {
        return m_cgt->elGetPt(e, i);
    }

    quintptr elGetPtName(quintptr e, const char *Name)
    {
        return m_cgt->elGetPtName(e, Name);
    }

    ElementClass elGetClassIndex(quintptr e)
    {
        return m_cgt->elGetClassIndex(e);
    }

    quintptr elGetSDK(quintptr e)
    {
        return m_cgt->elGetSDK(e);
    }

    bool elLinkIs(quintptr e)
    {
        return m_cgt->elLinkIs(e);
    }

    quintptr elLinkMain(quintptr e)
    {
        return m_cgt->elLinkMain(e);
    }

    void elGetPos(quintptr e, int &X, int &Y)
    {
        m_cgt->elGetPos(e, X, Y);
    }

    void elGetSize(quintptr e, int &W, int &H)
    {
        m_cgt->elGetSize(e, W, H);
    }

    int elGetEID(quintptr e)
    {
        return m_cgt->elGetEID(e);
    }

    quintptr ptGetLinkPoint(quintptr p)
    {
        return m_cgt->ptGetLinkPoint(p);
    }

    quintptr ptGetRLinkPoint(quintptr p)
    {
        return m_cgt->ptGetRLinkPoint(p);
    }

    PointTypes ptGetType(quintptr p)
    {
        return m_cgt->ptGetType(p);
    }

    const char *ptGetName(quintptr p)
    {
        return m_cgt->ptGetName(p);
    }

    quintptr ptGetParent(quintptr p)
    {
        return m_cgt->ptGetParent(p);
    }

    int ptGetIndex(quintptr p)
    {
        return m_cgt->ptGetIndex(p);
    }

    const char *pt_dpeGetName(quintptr p)
    {
        return m_cgt->pt_dpeGetName(p);
    }

    DataTypes propGetType(quintptr prop)
    {
        return m_cgt->propGetType(prop);
    }

    const char *propGetName(quintptr prop)
    {
        return m_cgt->propGetName(prop);
    }

    quintptr propGetValue(quintptr prop)
    {
        return m_cgt->propGetValue(prop);
    }

    uchar propToByte(quintptr prop)
    {
        return m_cgt->propToByte(prop);
    }

    int propToInteger(quintptr prop)
    {
        return m_cgt->propToInteger(prop);
    }

    qreal propToReal(quintptr prop)
    {
        return m_cgt->propToReal(prop);
    }

    const char *propToString(quintptr prop)
    {
        return m_cgt->propToString(prop);
    }

    int resAddFile(const char *Name)
    {
        return m_cgt->resAddFile(Name);
    }

    const char *resAddIcon(quintptr p)
    {
        return m_cgt->resAddIcon(p);
    }

    const char *resAddStr(const char *p)
    {
        return m_cgt->resAddStr(p);
    }

    const char *resAddStream(quintptr p)
    {
        return m_cgt->resAddStream(p);
    }

    const char *resAddWave(quintptr p)
    {
        return m_cgt->resAddWave(p);
    }

    const char *resAddBitmap(quintptr p)
    {
        return m_cgt->resAddBitmap(p);
    }

    const char *resAddMenu(quintptr p)
    {
        return m_cgt->resAddMenu(p);
    }

    int _Debug(const char *Text, int Color)
    {
        return m_cgt->_Debug(Text, Color);
    }

    int GetParam(CgtParams index, void *value)
    {
        return m_cgt->GetParam(index, value);
    }

    int arrCount(quintptr a)
    {
        return m_cgt->arrCount(a);
    }

    DataTypes arrType(quintptr a)
    {
        return m_cgt->arrType(a);
    }

    const char *arrItemName(quintptr a, int Index)
    {
        return m_cgt->arrItemName(a, Index);
    }

    quintptr arrItemData(quintptr a, int Index)
    {
        return m_cgt->arrItemData(a, Index);
    }

    quintptr arrGetItem(quintptr a, int Index)
    {
        return m_cgt->arrGetItem(a, Index);
    }

    int isDebug(quintptr e)
    {
        return m_cgt->isDebug(e);
    }

    DataTypes dtType(quintptr d)
    {
        return m_cgt->dtType(d);
    }

    const char *dtStr(quintptr d)
    {
        return m_cgt->dtStr(d);
    }

    int dtInt(quintptr d)
    {
        return m_cgt->dtInt(d);
    }

    qreal dtReal(quintptr d)
    {
        return m_cgt->dtReal(d);
    }

    const char *fntName(quintptr f)
    {
        return m_cgt->fntName(f);
    }

    int fntSize(quintptr f)
    {
        return m_cgt->fntSize(f);
    }

    uchar fntStyle(quintptr f)
    {
        return m_cgt->fntStyle(f);
    }

    uint fntColor(quintptr f)
    {
        return m_cgt->fntColor(f);
    }

    uchar fntCharSet(quintptr f)
    {
        return m_cgt->fntCharSet(f);
    }

    quintptr elGetData(quintptr e)
    {
        return m_cgt->elGetData(e);
    }

    void elSetData(quintptr e, quintptr data)
    {
        m_cgt->elSetData(e, data);
    }

    DataTypes ptGetDataType(quintptr p)
    {
        return m_cgt->ptGetDataType(p);
    }

    quintptr elGetParent(quintptr e)
    {
        return m_cgt->elGetParent(e);
    }

    int elGetPropertyListCount(quintptr e)
    {
        return m_cgt->elGetPropertyListCount(e);
    }

    quintptr elGetPropertyListItem(quintptr e, int i)
    {
        return m_cgt->elGetPropertyListItem(e, i);
    }

    const char *plGetName(quintptr p)
    {
        return m_cgt->plGetName(p);
    }

    const char *plGetInfo(quintptr p)
    {
        return m_cgt->plGetInfo(p);
    }

    const char *plGetGroup(quintptr p)
    {
        return m_cgt->plGetGroup(p);
    }

    quintptr plGetProperty(quintptr p)
    {
        return m_cgt->plGetProperty(p);
    }

    quintptr plGetOwner(quintptr p)
    {
        return m_cgt->plGetOwner(p);
    }

    const char *ptGetInfo(quintptr p)
    {
        return m_cgt->ptGetInfo(p);
    }

    quintptr propGetLinkedElement(quintptr e, const char *propName)
    {
        return m_cgt->propGetLinkedElement(e, propName);
    }

    int propIsTranslate(quintptr e, quintptr p)
    {
        return m_cgt->propIsTranslate(e, p);
    }

    quintptr propGetLinkedElementInfo(quintptr e, quintptr prop, char *buf)
    {
        return m_cgt->propGetLinkedElementInfo(e, prop, buf);
    }

    quintptr elGetSDKByIndex(quintptr e, int index)
    {
        return m_cgt->elGetSDKByIndex(e, index);
    }

    int elGetSDKCount(quintptr e)
    {
        return m_cgt->elGetSDKCount(e);
    }

    const char *elGetSDKName(quintptr e, int index)
    {
        return m_cgt->elGetSDKName(e, index);
    }

    quintptr sdkGetParent(quintptr SDK)
    {
        return m_cgt->sdkGetParent(SDK);
    }

    const char *elGetInterface(quintptr e)
    {
        return m_cgt->elGetInterface(e);
    }

    const char *elGetInherit(quintptr e)
    {
        return m_cgt->elGetInherit(e);
    }

    int resEmpty()
    {
        return m_cgt->resEmpty();
    }

    int resSetPref(const char *pref)
    {
        return m_cgt->resSetPref(pref);
    }

    int _Error(int line, quintptr e, const char *text)
    {
        return m_cgt->_Error(line, e, text);
    }

    int elGetGroup(quintptr e)
    {
        return m_cgt->elGetGroup(e);
    }

    int propSaveToFile(quintptr p, const char *fileName)
    {
        return m_cgt->propSaveToFile(p, fileName);
    }
}
