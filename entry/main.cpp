//Project
#include "cgt/cgt.h"
#include "cgt/emulatecgt.h"
#include "cgt/proxycgt.h"
#include "logger.h"
#include "package/packagemanager.h"
#include "scenemodel/scenemodel.h"

//STL

//Qt
#include <QCoreApplication>

bool execApp(const QString &appPath, const QString &args = "")
{
    bool res = false;
    QFileInfo fi(appPath);
    QString path = fi.absolutePath();
    QString name = fi.fileName();
    QString tmpCurrentPath = QDir::currentPath();

    QDir::setCurrent(path);
    if (QProcess::execute(name + args) >= 0) {
        res = true;
    }
    QDir::setCurrent(tmpCurrentPath);

    return res;
}

int main(int argc, char *argv[])
{
    logger::initialize();

    QCoreApplication a(argc, argv);

    //Задаём информацию о приложении
    QCoreApplication::setOrganizationName(APP_COMPANY);
    QCoreApplication::setOrganizationDomain(APP_COPYRIGHT);
    QCoreApplication::setApplicationName(APP_PRODUCT);
    QCoreApplication::setApplicationVersion(APP_VERSION);

    const QString codeGenFile = "CodeGen.dll";
    const QString makeExe = "make_exe.dll";
    const QString packagePath = QDir::toNativeSeparators(QDir::currentPath() + QDir::separator() + "packages/delphi/");
    const QString codePath = QDir::toNativeSeparators(packagePath + "code/");
    const QString makePath = QDir::toNativeSeparators(packagePath + "make/");
    const QString fullPathCodeGen = packagePath + codeGenFile;
    const QString fullPathMakeExe = makePath + makeExe;

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


    PPackageManager manager = new PackageManager();
    SceneModel model(manager);
    model.loadFromSha("test.sha");

    const QString modelFilePath = "test.json";
    qInfo("Loading model from file: %s", qUtf8Printable(modelFilePath));
    if (!model.loadModel(modelFilePath)) {
        qWarning("Model is not loaded from file: %s", qUtf8Printable(modelFilePath));
        exit(0);
    }
    QString fullPathProjectFile = codePath + model.getProjectName() + ".dpr";

    qInfo("Set params for model.");
    model.setProjectPath(QDir::currentPath());
    model.setCodePath(codePath);

    qInfo("Initialize EmulateCgt and TBuildProcessRec.");
    EmulateCgt::setSceneModel(&model);

    ProxyCgt::setProxiedCgt(EmulateCgt::getCgt());
    qInfo("Call func buildProcessProc from CodeGen.dll...");
    TBuildProcessRec rec(ProxyCgt::getCgt(), model.getIdRootContainer());
    CgResult resultBuild = buildProcessProcLib(rec);
    if (resultBuild != CG_SUCCESS) {
        qCritical() << "Error build project: buildProcessProc return" << CgResultMap[resultBuild];
        exit(0);
    }

    qInfo("Compile resources.");
    model.compileResources();

    qInfo("Use make_*.dll library.");
    //ru Загружаем make_*.dll в память
    if (!QFile::exists(fullPathMakeExe)) {
        qCritical("%s library not found!", qPrintable(makeExe));
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

    TBuildMakePrjRec buildMakePrjRec;
    buildMakePrjRec.compiler = fcgt::strToCString(model.getCompiler());
    buildMakePrjRec.prjFilename = fcgt::strToCString(fullPathProjectFile);
    buildMakePrjRec.result = rec.result;

    TBuildCompliteRec buildCompliteRec;
    buildCompliteRec.appFilename = fcgt::strToCString(QDir::currentPath() + QDir::separator() + model.getProjectName() + ".exe");
    buildCompliteRec.prjFilename = fcgt::strToCString(fullPathProjectFile);

    buildMakePrj(buildMakePrjRec);
    model.addResList(fullPathProjectFile);

    qInfo("Compile project.");
    QString args = QString(" \"%1\" \"-U%2.\" -Q").arg(fullPathProjectFile).arg(QDir::currentPath());

    if (execApp("compiler/delphi/dcc32.exe", args)) {
        qInfo() << "Compilation successful.";
    } else {
        qWarning() << "Compile error.";
    }

    buildCompliteProc(buildCompliteRec);

    qInfo("Unload make_* library.");
    libMakeExe.unload();

    qInfo("Unload CodeGen library.");
    libCodeGen.unload();

    qInfo("Delete resources.");
    model.deleteResources();

    return a.exec();
}
