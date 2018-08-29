TEMPLATE = app
TARGET = cmapdump
INCLUDEPATH += .

QT -= gui core gl

QMAKE_CFLAGS += -ffunction-sections -fdata-sections -pipe -O2 -DNDEBUG -fomit-frame-pointer -fPIC -I../include
CONFIG += warn_off silent

# Input
SOURCES += cmapdump.c

OBJECTS_DIR = ../build/tools/
