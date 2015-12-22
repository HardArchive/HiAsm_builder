QT += core
QT -= gui

CONFIG += c++14

TARGET = HiAsm_builder
CONFIG += console
CONFIG -= app_bundle

#ru Тестовый каталог
DESTDIR += "D:/dev/Qt/MainProjects/HiAsm_builder/test"
TEMPLATE = app

#Информация о приложении
CONFIG += embed_manifest_exe
QMAKE_TARGET_COMPANY = "HiAsm community(c)"
QMAKE_TARGET_DESCRIPTION = "HiAsm builder"
QMAKE_TARGET_COPYRIGHT = "CriDos"
QMAKE_TARGET_PRODUCT = "HiAsm_builder"
#RC_ICONS = "res/icon/icon.ico"
RC_LANG = "0x419"
VERSION = "1.0.0.0" #Версия приложения, ваш Кэп:)

DEFINES += APP_COMPANY=\"\\\"$$QMAKE_TARGET_COMPANY\\\"\" \
           APP_DESCRIPTION=\"\\\"$$QMAKE_TARGET_DESCRIPTION\\\"\" \
           APP_COPYRIGHT=\"\\\"$$QMAKE_TARGET_COPYRIGHT\\\"\" \
           APP_PRODUCT=\"\\\"$$QMAKE_TARGET_PRODUCT\\\"\" \
           APP_VERSION=\"\\\"$$VERSION\\\"\"

#Параметры логгера
DEFINES += ELPP_QT_LOGGING \
           ELPP_STL_LOGGING \
           ELPP_STRICT_SIZE_CHECK \
           ELPP_THREAD_SAFE \
           ELPP_MULTI_LOGGER_SUPPORT \
           ELPP_NO_DEFAULT_LOG_FILE

SOURCES += $$files("*.cpp", true)

HEADERS += $$files("*.h", true)
