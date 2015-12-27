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

    const char *strToCString(const QString &str);
}
