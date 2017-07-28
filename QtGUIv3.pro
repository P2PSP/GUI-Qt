TARGET      = QtGUIv3
TEMPLATE    = app
CONFIG 	   += c++11

QT         += widgets
QT         += network

SOURCES    += main.cpp \
    add_dialog.cpp \
    peer.cpp \
    peerthread.cpp \
    helper.cpp \
    addchannels.cpp \
    rundialog.cpp \
    rundialogstorage.cpp \
    SimplePlayer.cpp \
    sink.cpp

HEADERS    += \
    add_dialog.h \
    peer.h \
    peerthread.h \
    helper.h \
    addchannels.h \
    rundialog.h \
    globalheader.h \
    rundialogstorage.h \
    SimplePlayer.h \
    sink.h

FORMS      += \
    add_dialog.ui \
    helper.ui \
    rundialog.ui \
    SimplePlayer.ui

# Edit below for custom library location
LIBS        += -L/usr/include/boost -DBOOST_LOG_DYN_LINK -lboost_thread -lpthread -lboost_system -lboost_log_setup -lboost_log -lboost_date_time  -lboost_chrono -lboost_timer -lboost_filesystem -lboost_log_setup -lboost_program_options
LIBS        += -L/usr/lib -lvlc
LIBS       += -L"$$PWD/../core/src"
LIBS       += -L"$$PWD/../core/bin" -lp2psp

INCLUDEPATH += /usr/include/boost
INCLUDEPATH += /usr/include
INCLUDEPATH +=/"$$PWD/../core/src"
INCLUDEPATH +=/"$$PWD/../core/bin"

RESOURCES += \
    icons.qrc
