//Project
#include "cgt/cgt.h"
#include "cgt/emulatecgt.h"
#include "scenemodel/scenemodel.h"
#include "logger.h"

//STL

//Qt
#include <QCoreApplication>

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
    //SetConsoleCP(1251);
    //SetConsoleOutputCP(1251);

    QCoreApplication a(argc, argv);

    //Задаём информацию о приложении
    QCoreApplication::setOrganizationName(APP_COMPANY);
    QCoreApplication::setOrganizationDomain(APP_COPYRIGHT);
    QCoreApplication::setApplicationName(APP_PRODUCT);
    QCoreApplication::setApplicationVersion(APP_VERSION);

    initLogger();

    const QString packagePath = QDir::toNativeSeparators(QDir::currentPath() + QDir::separator() + "Elements/delphi/");
    const QString codePath = QDir::toNativeSeparators(packagePath + "code/");
    const QString makePath = QDir::toNativeSeparators(packagePath + "make/");
    const QString codeGenFile = "CodeGen.dll";
    const QString makeExe = "make_exe.dll";
    const QString fullPathCodeGen = packagePath + codeGenFile;
    const QString fullPathMakeExe = makePath + makeExe;

    //ru Загружаем make_CNET в память
    if (!QFile::exists(fullPathMakeExe)) {
        qCritical(qUtf8Printable(QString("%1 library not found!").arg(qPrintable(makeExe))));
    }
    QLibrary libMakeExe(fullPathMakeExe);
    if (!libMakeExe.load()) {
        qCritical("%s library not found!", qPrintable(makeExe));
        exit(0);
    }
    qInfo("%s library successfully loaded.", qPrintable(makeExe));

    //ru Определение функций
    buildGetParamsProc = reinterpret_cast<t_buildGetParamsProc>(libMakeExe.resolve("buildGetParamsProc"));
    buildMakePrj = reinterpret_cast<t_buildMakePrj>(libMakeExe.resolve("buildMakePrj"));
    buildCompliteProc = reinterpret_cast<t_buildCompliteProc>(libMakeExe.resolve("buildCompliteProc"));

    //ru Загружаем CodeGen в память
    if (!QFile::exists(fullPathCodeGen)) {
        qCritical(qUtf8Printable(QString("%1 library not found!").arg(qPrintable(codeGenFile))));
    }
    QLibrary libCodeGen(fullPathCodeGen);
    if (!libCodeGen.load()) {
        qCritical("%s library not found!", qPrintable(codeGenFile));
        exit(0);
    }
    qInfo("%s library successfully loaded.", qPrintable(codeGenFile));

    //ru Определение функций кодогенератора
    buildPrepareProcLib = reinterpret_cast<t_buildPrepareProc>(libCodeGen.resolve("buildPrepareProc"));
    buildProcessProcLib = reinterpret_cast<t_buildProcessProc>(libCodeGen.resolve("buildProcessProc"));
    checkVersionProcLib = reinterpret_cast<t_checkVersionProc>(libCodeGen.resolve("CheckVersionProc"));

    const QString modelFilePath = "test.json";
    qInfo("Loading model from file: %s", qUtf8Printable(modelFilePath));
    SceneModel model;
    if (!model.loadModel(modelFilePath)) {
        qWarning("Model is not loaded from file: %s", qUtf8Printable(modelFilePath));
        exit(0);
    }
    QString fullPathProjectFile = codePath + QDir::separator() +  model.getProjectName() + ".dpr";

    qInfo("Set params for Model.");
    model.setProjectPath(QDir::currentPath());
    model.setCodePath(codePath);

    qInfo("Initialize EmulateCgt and TBuildProcessRec.");
    EmulateCgt::setSceneModel(&model);

    qInfo("Call func buildProcessProc from CodeGen.dll...");
    TBuildProcessRec rec(EmulateCgt::getCgt(), model.getIdRootContainer());
    buildProcessProcLib(rec);

    qInfo("Compile resources.");
    model.compileResources();

    qInfo("Use make_*.dll library.");
    TBuildParams buildParams;
    buildParams.flags = 0;

    TBuildMakePrjRec buildMakePrjRec;
    buildMakePrjRec.compiler = fcgt::strToCString(model.getCompiler());
    buildMakePrjRec.prjFilename = fcgt::strToCString(fullPathProjectFile);
    buildMakePrjRec.result = rec.result;

    TBuildCompliteRec buildCompliteRec;
    buildCompliteRec.appFilename = fcgt::strToCString(QDir::currentPath() + QDir::separator() + model.getProjectName() + ".exe");
    buildCompliteRec.prjFilename = fcgt::strToCString(fullPathProjectFile);

    buildGetParamsProc(buildParams);
    buildMakePrj(buildMakePrjRec);
    buildCompliteProc(buildCompliteRec);

    qInfo("Unload CodeGen library.");
    libCodeGen.unload();

    qInfo("Build project.");
    QString tmpCurrentPath = QDir::currentPath();
    QDir::setCurrent("compiler/delphi/");
    QString pathCompiler = "compiler/delphi/dcc32.exe";
    QString args = QString(" \"%1\" \"-U%2.\" -Q").arg(fullPathProjectFile).arg(QDir::currentPath());
    QProcess::execute(pathCompiler + args);
    QDir::setCurrent(tmpCurrentPath);

    return a.exec();
}
