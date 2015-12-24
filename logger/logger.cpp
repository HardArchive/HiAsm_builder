#include "logger.h"
//Project

//STL

//Qt
#include <QtCore>

namespace logger
{
    void handler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
    {
        Q_UNUSED(context)
        QTextStream cout(stdout);
        QTextStream cerr(stderr);

#ifndef QT_NO_TEXTCODEC
#ifdef _WIN32
        cout.setCodec("CP866");
        cerr.setCodec("CP866");
#else
        cout.setCodec("System");
        cerr.setCodec("System");
#endif
#endif

        switch (type) {
        case QtDebugMsg:
            cout << msg << endl;

            break;
        case QtInfoMsg:
            cout << msg << endl;

            break;
        case QtWarningMsg:
            cout << msg << endl;

            break;
        case QtCriticalMsg:
            cerr << msg << endl;

            break;
        case QtFatalMsg:
            cerr << msg << endl;

            abort();
        }
    }

    void initialize()
    {
        qInstallMessageHandler(handler);
    }

}
