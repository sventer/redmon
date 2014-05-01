
QT += core gui network xml

TARGET = src
TEMPLATE = app

SOURCES += \
  main.cpp \
  data/issue.cpp \
  models/tasks_model.cpp \
  windows/main_window.cpp

HEADERS += \
  data/issue.h \
  models/tasks_model.h \
  windows/main_window.h

FORMS += \
  windows/main_window.ui
