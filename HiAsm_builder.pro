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

SOURCES += $$files("cgt/*.cpp", true)
SOURCES += $$files("scenemodel/*.cpp", true)
SOURCES += $$files("entry/*.cpp", true)

HEADERS += $$files("cgt/*.h", true)
HEADERS += $$files("scenemodel/*.h", true)
HEADERS += $$files("entry/*.h", true)

#Подключаем логгер
include(logger/logger.pri)
