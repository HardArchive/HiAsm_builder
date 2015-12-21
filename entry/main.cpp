//Project
#include "cgt/cgt.h"
#include "cgt/proxycgt.h"
#include "cgt/emulatecgt.h"
#include "scenemodel/scenemodel.h"
#include "logger.h"

//STL

//Qt
#include <QCoreApplication>
#include <QDir>
#include <QLibrary>

//Типы функций
typedef int(*t_buildPrepareProc)(void *params);
typedef int(*t_buildProcessProc)(TBuildProcessRec &params);
typedef int(*t_CheckVersionProc)(const THiAsmVersion &params);

//Объявление прототипов функций оригинального кодогенератора
static t_buildPrepareProc buildPrepareProc;
static t_buildProcessProc buildProcessProc;
static t_CheckVersionProc CheckVersionProc;

//Переопределение вывода отладочных сообщений
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context)

    QByteArray message = msg.toLocal8Bit();
    switch (type) {
    case QtDebugMsg:
        LOG(DEBUG) << message.constData();
        break;
    case QtInfoMsg:
        LOG(INFO) << message.constData();
        break;
    case QtWarningMsg:
        LOG(WARNING) << message.constData();
        break;
    case QtCriticalMsg:
        LOG(ERROR) << message.constData();
        break;
    case QtFatalMsg:
        LOG(FATAL) << message.constData();
        abort();
    }
}

INITIALIZE_EASYLOGGINGPP
void initLogger()
{
    QDir makeLogDir;
    makeLogDir.mkdir("logs");
    el::Configurations conf;
    conf.setGlobally(el::ConfigurationType::Filename, "logs/%datetime.log");
    //conf.setGlobally(el::ConfigurationType::Format, "%datetime{%h:%m:%s.%z}:%levshort: %msg");
    conf.setGlobally(el::ConfigurationType::Format, "%msg");
    el::Logger *defaultLogger = el::Loggers::getLogger("default");
    defaultLogger->configure(conf);
    el::Loggers::removeFlag(el::LoggingFlag::NewLineForContainer);
    el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
    el::Loggers::addFlag(el::LoggingFlag::LogDetailedCrashReason);
    el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
    el::Loggers::addFlag(el::LoggingFlag::DisablePerformanceTrackingCheckpointComparison);
    el::Loggers::addFlag(el::LoggingFlag::DisableVModules);
    el::Loggers::addFlag(el::LoggingFlag::DisableVModulesExtensions);
    qInstallMessageHandler(myMessageOutput);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    initLogger();

    QString codegenFileName = "CodeGen.dll";
    QString filePathCodeGen = "Elements/CNET/CodeGen.dll";

    //ru Загружаем оригинальную DLL в память
    if (!QFile::exists(filePathCodeGen)) {
        qCritical("«%s» library not found!", qPrintable(codegenFileName));
    }
    QLibrary codegen;
    codegen.setFileName(filePathCodeGen);
    if (codegen.load())
        qInfo("%s library successfully loaded.", qUtf8Printable(codegenFileName));
    else
        qCritical("%s library is not loaded.", qUtf8Printable(codegenFileName));

    //ru Определение прототипов функций кодогенератора
    buildPrepareProc = reinterpret_cast<t_buildPrepareProc>(codegen.resolve("buildPrepareProc"));
    buildProcessProc = reinterpret_cast<t_buildProcessProc>(codegen.resolve("buildProcessProc"));
    CheckVersionProc = reinterpret_cast<t_CheckVersionProc>(codegen.resolve("CheckVersionProc"));

    QFile file("test.json");
    file.open(QIODevice::ReadOnly);

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    SceneModel model(doc);

    EmulateCgt::setSceneModel(&model);
    TBuildProcessRec rec;
    rec.sdk = model.getIdRootContainer();
    rec.cgt = EmulateCgt::getCgt();
    buildProcessProc(rec);

    return a.exec();
}
