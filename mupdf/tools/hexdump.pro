TEMPLATE = app
TARGET = hexdump.exe
INCLUDEPATH += .

QT -= gui core gl pthread

QMAKE_CFLAGS += -ffunction-sections -fdata-sections -O2 -DNDEBUG -fomit-frame-pointer -DJBIG_NO_MEMENTO -fPIC -DTOFU -DNOCJK -fPIC -DNO_ICC -DHAVE_LIBCRYPTO -DSHARE_JPEG
QMAKE_LFLAGS += -Wl,--gc-sections -Wl,-s

LIBS += -lm -lfreetype -lharfbuzz -ljbig2dec -lcrypto -ljpeg -lopenjp2 -lz

# Input
SOURCES += hexdump.c

OBJECTS_DIR = ../build/tools/
