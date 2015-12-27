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
        char *buf = new char[size_t(str.size() + 1)];
        strcpy(buf, str.toLocal8Bit().data());
        return buf;
    }
}
