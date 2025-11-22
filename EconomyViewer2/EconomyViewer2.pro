TEMPLATE = app
TARGET = EconomyViewer

QT = core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++20

INCLUDEPATH += include

SOURCES += \
  src/core/account.cpp \
  src/core/importedrow.cpp \
  src/core/constants.cpp \
  src/core/economyviewer.cpp \
  src/core/jsonNode.cpp \
  src/core/matchCondition.cpp \
  src/core/transaction.cpp \
  src/core/transactionGroup.cpp \
  src/views/accounts.cpp \
  src/views/matchConditions.cpp \
  src/views/newtransactions.cpp \
  src/views/transactionGroups.cpp \
  src/views/transactions.cpp \
  src/main.cpp

HEADERS += \
  include/core/account.h \
  include/core/constants.h \
  include/core/economyviewer.h \
  include/core/importedrow.h \
  include/core/jsonNode.h \
  include/core/matchCondition.h \
  include/core/transaction.h \
  include/core/transactionGroup.h \
  include/views/accounts.h \
  include/views/matchConditions.h \
  include/views/newtransactions.h \
  include/views/transactionGroups.h \
  include/views/transactions.h \
