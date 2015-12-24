#pragma once

//Project

//STL

//Qt
#include <QTextStream>

namespace logger
{
    void handler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void initialize();
}


