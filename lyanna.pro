TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
QMAKE_CXXFLAGS += -Ofast
QMAKE_CXXFLAGS += -march=native
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_DEBUG -= -O
QMAKE_CXXFLAGS_DEBUG -= -O1
QMAKE_CXXFLAGS_DEBUG -= -O2
SOURCES += main.cpp \
    boardstate.cpp \
    evalinfo.cpp \
    search.cpp

HEADERS += \
    boardstate.h \
    evalinfo.h \
    search.h

