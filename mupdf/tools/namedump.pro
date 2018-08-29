TEMPLATE = app
TARGET = namedump
INCLUDEPATH += .

QT -= gui core gl

QMAKE_CFLAGS += -Wall -Wdeclaration-after-statement -ffunction-sections -fdata-sections -pipe -O2 -DNDEBUG -fomit-frame-pointer -fPIC -I../include

# Input
SOURCES += namedump.c

OBJECTS_DIR = ../build/tools/
