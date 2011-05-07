TEMPLATE = app
CONFIG += qt \
    debug
HEADERS += src/*.hpp \
    gui/newworld.hpp
HEADERS += gui/*.hpp
INCLUDEPATH += /home/dbuschho/Desktop/pion_network/pion-net-3.0.27/net/include /home/dbuschho/Desktop/pion_network/pion-net-3.0.27/common/include
SOURCES += src/*.cpp
SOURCES += gui/*.cpp
OBJECTS_DIR = build
LIBS += -L/home/dbuschho/Desktop/pion_network/pion-net-3.0.27/net/utils /home/dbuschho/Desktop/pion_network/pion-net-3.0.27/net/utils/libpion-net-3.0.so /home/dbuschho/Desktop/pion_network/pion-net-3.0.27/common/src/.libs/libpion-common.so -ldl -lboost_thread-mt -lboost_system-mt -lboost_filesystem-mt -lboost_regex-mt -lboost_date_time-mt -lboost_signals-mt -lboost_iostreams-mt -lssl -lcrypto -lz -lbz2
DESTDIR = build
FORMS += gui/mainwindow.ui \
    gui/newworld.ui
RESOURCES += gui/floatworld.qrc

OTHER_FILES += \
    README.markdown \
    PLANS
