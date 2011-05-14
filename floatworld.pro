TEMPLATE = app
CONFIG += qt \
    debug
HEADERS += src/*.hpp \
    gui/newworld.hpp
HEADERS += gui/*.hpp
SOURCES += src/*.cpp
SOURCES += gui/*.cpp
OBJECTS_DIR = build
LIBS += -lpion-net -lpion-common -ldl -lboost_thread-mt -lboost_system-mt -lboost_filesystem-mt -lboost_regex-mt -lboost_date_time-mt -lboost_signals-mt -lboost_iostreams-mt -lssl -lcrypto -lz -lbz2
DESTDIR = build
FORMS += gui/mainwindow.ui \
    gui/newworld.ui
RESOURCES += gui/floatworld.qrc

OTHER_FILES += \
    README.markdown \
    PLANS
