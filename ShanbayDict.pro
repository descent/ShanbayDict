#-------------------------------------------------
#
# Project created by QtCreator 2012-01-06T14:00:30
#
#-------------------------------------------------

QT       += core gui\
            network\
            phonon


TARGET = ShanbayDict
TEMPLATE = app

INCLUDEPATH += src
SOURCES += \
    ./src/main.cpp \
    ./src/shanbaynet.cpp \
    ./src/login.cpp \
    ./src/englishword.cpp \
    ./src/mainwindow.cpp \
    ./src/clicklabel.cpp \
    ./src/ballonwindow.cpp \
    ./src/dict.cpp \
    ./src/config.cpp \
    ./src/picturebutton.cpp \
    ./src/history.cpp
HEADERS  += \
    ./src/shanbaynet.h \
    ./src/login.h \
    ./src/englishword.h \
    ./src/mainwindow.h \
    ./src/clicklabel.h \
    ./src/ballonwindow.h \
    ./src/dict.h \
    ./src/config.h \
    ./src/picturebutton.h \
    ./src/history.h
FORMS    += \
    ./src/login.ui \
    ./src/mainwindow.ui \
    ./src/ballonwindow.ui
INCLUDEPATH += ./qjson/include
LIBS += -L../ShanbayDict/qjson/lib \
    -lqjson
INCLUDEPATH +=./lib
LIBS += -L../ShanbayDict/lib \
    -lWinHook
#-lgdi32
RESOURCES += \
    icons.qrc
RC_FILE = app.rc






































