#pragma once

//Project
#include "CGTShare.h"

//STL

//Qt
#include <QString>

namespace fcgt
{
    bool isSelect(const ElementFlgs &flags);
    bool isParent(const ElementFlgs &flags);
    bool isCore(const ElementFlgs &flags);
    bool isNoDelete(const ElementFlgs &flags);
    bool isMulti(const ElementFlgs &flags);
    bool isEdit(const ElementFlgs &flags);
    bool isLink(const ElementFlgs &flags);

    bool isEditMultiEx(const ElementClass &eClass);
    bool isPolyMulti(const ElementClass &eClass);

    const char *strToPChar(const QString &str);
}

namespace cgt
{
    //!~~~~~~~~~~~~~~~~~~~~~ служебные функции ~~~~~~~~~~~~~~~~~~~~~~~
    //ru Устанавливаем параметры
    void init(TBuildProcessRec &params);
    void setProxyCgt(PCodeGenTools cgt);
    PCodeGenTools getOriginalCgt();
    quintptr getMainSDK();

    //!~~~~~~~~~~~~~~~~~~~~~~~~ CGT ~~~~~~~~~~~~~~~~~~~~~~~~~~
    int sdkGetCount(quintptr SDK);
    quintptr sdkGetElement(quintptr SDK, int Index);
    quintptr sdkGetElementName(quintptr SDK, char *Name);
    ElementFlags elGetFlag(quintptr e);
    int elGetPropCount(quintptr e);
    quintptr elGetProperty(quintptr e, int Index);
    bool elIsDefProp(quintptr id_element, int index);
    quintptr elSetCodeName(quintptr e, const char *Name);
    const char *elGetCodeName(quintptr e);
    const char *elGetClassName(quintptr e);
    const char *elGetInfSub(quintptr e);
    int elGetPtCount(quintptr e);
    quintptr elGetPt(quintptr e, int i);
    quintptr elGetPtName(quintptr e, const char *Name);
    ElementClass elGetClassIndex(quintptr e);
    quintptr elGetSDK(quintptr e);
    bool elLinkIs(quintptr e);
    quintptr elLinkMain(quintptr e);
    void elGetPos(quintptr e, int &X, int &Y);
    void elGetSize(quintptr e, int &W, int &H);
    int elGetEID(quintptr e);
    quintptr ptGetLinkPoint(quintptr p);
    quintptr ptGetRLinkPoint(quintptr p);
    PointTypes ptGetType(quintptr p);
    const char *ptGetName(quintptr p);
    quintptr ptGetParent(quintptr p);
    int ptGetIndex(quintptr p);
    const char *pt_dpeGetName(quintptr p);
    DataTypes propGetType(quintptr prop);
    const char *propGetName(quintptr prop);
    quintptr propGetValue(quintptr prop);
    uchar propToByte(quintptr prop);
    int propToInteger(quintptr prop);
    qreal propToReal(quintptr prop);
    const char *propToString(quintptr prop);
    int resAddFile(const char *Name);
    const char *resAddIcon(quintptr p);
    const char *resAddStr(const char *p);
    const char *resAddStream(quintptr p);
    const char *resAddWave(quintptr p);
    const char *resAddBitmap(quintptr p);
    const char *resAddMenu(quintptr p);
    int _Debug(const char *Text, int Color);
    int GetParam(CgtParams index, void *value);
    int arrCount(quintptr a);
    DataTypes arrType(quintptr a);
    const char *arrItemName(quintptr a, int Index);
    quintptr arrItemData(quintptr a, int Index);
    quintptr arrGetItem(quintptr a, int Index);
    int isDebug(quintptr e);
    DataTypes dtType(quintptr d);
    const char *dtStr(quintptr d);
    int dtInt(quintptr d);
    qreal dtReal(quintptr d);
    const char *fntName(quintptr f);
    int fntSize(quintptr f);
    uchar fntStyle(quintptr f);
    uint fntColor(quintptr f);
    uchar fntCharSet(quintptr f);
    quintptr elGetData(quintptr e);
    void elSetData(quintptr e, quintptr data);
    DataTypes ptGetDataType(quintptr p);
    quintptr elGetParent(quintptr e);
    int elGetPropertyListCount(quintptr e);
    quintptr elGetPropertyListItem(quintptr e, int i);
    const char *plGetName(quintptr p);
    const char *plGetInfo(quintptr p);
    const char *plGetGroup(quintptr p);
    quintptr plGetProperty(quintptr p);
    quintptr plGetOwner(quintptr p);
    const char *ptGetInfo(quintptr p);
    quintptr propGetLinkedElement(quintptr e, const char *propName);
    int propIsTranslate(quintptr e, quintptr p);
    quintptr propGetLinkedElementInfo(quintptr e, quintptr prop, char *buf);
    quintptr elGetSDKByIndex(quintptr e, int index);
    int elGetSDKCount(quintptr e);
    const char *elGetSDKName(quintptr e, int index);
    quintptr sdkGetParent(quintptr SDK);
    const char *elGetInterface(quintptr e);
    const char *elGetInherit(quintptr e);
    int resEmpty();
    int resSetPref(const char *pref);
    int _Error(int line, quintptr e, const char *text);
    int elGetGroup(quintptr e);
    int propSaveToFile(quintptr p, const char *fileName);
}
