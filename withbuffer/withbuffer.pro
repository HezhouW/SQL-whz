#-------------------------------------------------
#
# Project created by QtCreator 2013-06-17T09:11:25
#
#-------------------------------------------------

QT       += core
QT       += sql

QT       -= gui

TARGET = withbuffer
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    SqlwriteThread_buffer.cpp \
    producer.cpp

HEADERS += \
    SqlwriteThread_buffer.h \
    producer.h
