#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T17:23:14
#
#-------------------------------------------------

QT       += core \
      network

QT       -= gui

TARGET = monitor
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    configfile.cpp \
    uart/uart.c \
    common/log.cpp \
    monitor.cpp \
    uart/uartthread.cpp \
    tcp/tcpclient.cpp \
    email.cpp \
    smtp.cpp \
    uart/writethread.cpp

HEADERS += \
    uart/uart.h \
    configfile.h \
    uart/uart.h \
    common/log.h \
    monitor.h \
    uart/uartthread.h \
    tcp/tcpclient.h \
    common/define.h \
    common/sensor.h \
    email.h \
    smtp.h \
    uart/writethread.h

FORMS +=
