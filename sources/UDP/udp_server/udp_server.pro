TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_LFLAGS += -pthread
SOURCES += main.c

HEADERS += \
    ch_dir.h \
    download.h \
    get_dir.h \
    mk_dir.h \
    rm_dir.h \
    upload.h \
    view.h \
    view_dir.h

