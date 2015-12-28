TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_LFLAGS += -pthread
SOURCES += main.c

HEADERS += \
    view.h \
    get_dir.h \
    view_dir.h \
    ch_dir.h \
    download.h \
    mk_dir.h \
    rm_dir.h \
    connection_handler.h \
    upload.h

