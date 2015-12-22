QT += core
QT -= gui

CONFIG += c++14

TARGET = HiAsm_builder
CONFIG += console
CONFIG -= app_bundle

#ru Тестовый каталог
DESTDIR += "D:/dev/Qt/MainProjects/HiAsm_builder/test"

TEMPLATE = app

#Параметры логгера
DEFINES += ELPP_QT_LOGGING \
           ELPP_STL_LOGGING \
           ELPP_STRICT_SIZE_CHECK \
           ELPP_THREAD_SAFE \
           ELPP_MULTI_LOGGER_SUPPORT \
           ELPP_NO_DEFAULT_LOG_FILE

SOURCES += $$files("*.cpp", true)

HEADERS += $$files("*.h", true)
