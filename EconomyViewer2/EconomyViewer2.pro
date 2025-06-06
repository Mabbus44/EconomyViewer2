TEMPLATE = app
TARGET = EconomyViewer

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

INCLUDEPATH += include

SOURCES += \
    src/core/constants.cpp \
    src/core/economyviewer.cpp \
    src/core/transaction.cpp \
    src/views/newtransactions.cpp \
    src/views/transactions.cpp \
    src/main.cpp

HEADERS += \
    include/core/constants.h \
    include/core/economyviewer.h \
    include/core/transaction.h \
    include/views/newtransactions.h \
    include/views/transactions.h
