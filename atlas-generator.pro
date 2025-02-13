QT += core
QT += gui

CONFIG += c++11

TARGET = atlas-generator
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    main.cc \
    RectangleBinPack/GuillotineBinPack.cpp \
    RectangleBinPack/MaxRectsBinPack.cpp \
    RectangleBinPack/Rect.cpp \
    RectangleBinPack/ShelfBinPack.cpp \
    RectangleBinPack/ShelfNextFitBinPack.cpp \
    RectangleBinPack/SkylineBinPack.cpp \
    AtlasPacker.cc \
    Configuration.cc \
    JsonExport.cpp \
    Utils.cc \
    CRC32/CRC32.cpp

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000
# disables all the APIs deprecated before Qt 6.0.0

HEADERS += \
    RectangleBinPack/GuillotineBinPack.h \
    RectangleBinPack/MaxRectsBinPack.h \
    RectangleBinPack/Rect.h \
    RectangleBinPack/ShelfBinPack.h \
    RectangleBinPack/ShelfNextFitBinPack.h \
    RectangleBinPack/SkylineBinPack.h \
    AtlasPacker.h \
    Configuration.h \
    DataExport.h \
    JsonExport.h \
    Utils.h \
    CRC32/CRC32.h
