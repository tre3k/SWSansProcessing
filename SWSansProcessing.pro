# (c) 2021 Kirill Pshenichnyi 
# GNU GPLv3 License
  
QT += core gui widgets printsupport

TARGET = swsansprocessing
TEMPLATE = app

INCLUDEPATH = includes/


SOURCES += src/main.cpp \
           src/centralwidget.cpp \
           src/generalplot.cpp \
           src/generalsetup.cpp \
           src/mainwindow.cpp \
           src/qcustomplot.cpp


HEADERS += includes/main.h \
           includes/centralwidget.h \
           includes/generalplot.h \
           includes/generalsetup.h \
           includes/mainwindow.h \
           includes/qcustomplot.h




