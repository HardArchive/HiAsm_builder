#include "logger.h"
//Project

//STL

//Qt
#include <QtCore>
#include <QDateTime>
#include <QDir>

namespace logger
{
    static QDir dir;
    static QString format = "yyyy-mm-dd_hh_mm_ss_zzz";
    static QFile file(QString("logs/%1.log").arg(QDateTime::currentDateTime().toString(format)));

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

        dir.mkdir("logs");
        file.write(QString("%1\r\n").arg(msg).toUtf8());

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
        dir.mkdir("logs");
        file.open(QIODevice::Append);
        qInstallMessageHandler(handler);
    }

}
