TEMPLATE = lib
TARGET = pdfwidget

# MuPDF locations
# ===============
INCLUDEPATH += . mupdf/source/cbz mupdf/source/fitz mupdf/source/gprf mupdf/source/html mupdf/source/pdf mupdf/source/svg mupdf/source/xps mupdf/mujs mupdf/include
INCLUDEPATH += /usr/include/freetype2 /usr/include/harfbuzz
DEPENDPATH += . mupdf/source/cbz mupdf/source/fitz mupdf/source/gprf mupdf/source/html mupdf/source/pdf mupdf/source/svg mupdf/source/xps mupdf/mujs mupdf/include

VERSION = 0.0.3

# cflags and defines
# ==================
QMAKE_CFLAGS += -ffunction-sections -fdata-sections -pipe -O2 -fomit-frame-pointer -fPIC -fno-dce -fno-dse -fno-tree-dce -fno-tree-dse
QMAKE_CFLAGS += $$system( pkg-config --cflags libopenjp2 )					# openJP2
QMAKE_CPPFLAGS += -fPIC -fno-dce -fno-dse -fno-tree-dce -fno-tree-dse
DEFINES += NDEBUG JBIG_NO_MEMENTO TOFU NOCJK SHARE_JPEG NO_ICC HAVE_LIBCRYPTO

# LIBS
# ====
LIBS         += -L/usr/lib -lharfbuzz -lfreetype -lz -ljpeg -lopenjp2 -ljbig2dec

# MuJS
# ====
HEADERS += mupdf/mujs/astnames.h
HEADERS += mupdf/mujs/jsbuiltin.h
HEADERS += mupdf/mujs/jscompile.h
HEADERS += mupdf/mujs/jsi.h
HEADERS += mupdf/mujs/jslex.h
HEADERS += mupdf/mujs/jsparse.h
HEADERS += mupdf/mujs/jsrun.h
HEADERS += mupdf/mujs/jsvalue.h
HEADERS += mupdf/mujs/mujs.h
HEADERS += mupdf/mujs/opnames.h
HEADERS += mupdf/mujs/regexp.h
HEADERS += mupdf/mujs/utf.h

SOURCES += mupdf/mujs/one.c

# MuPDF
# =====
HEADERS += mupdf/include/mupdf/*.h mupdf/include/mupdf/fitz/*.h mupdf/include/mupdf/helpers/*.h mupdf/include/mupdf/pdf/*.h
HEADERS += mupdf/source/fitz/*.h mupdf/source/html/*.h mupdf/source/pdf/*.h mupdf/source/svg/*.h mupdf/source/xps/*.h

SOURCES += mupdf/source/cbz/*.c
SOURCES += mupdf/source/fitz/*.c
SOURCES += mupdf/source/gprf/*.c
SOURCES += mupdf/source/html/*.c
SOURCES += mupdf/source/pdf/*.c
SOURCES += mupdf/source/svg/*.c
SOURCES += mupdf/source/xps/*.c
SOURCES += mupdf/source/generated/*.c

# PdfWidget
# =========
HEADERS += MuPdfDocument.hpp
HEADERS += PdfDocument.hpp
HEADERS += PdfPrinter.hpp
HEADERS += PdfView.hpp
HEADERS += PopplerDocument.hpp

SOURCES += MuPdfDocument.cpp
SOURCES += PdfPrinter.cpp
SOURCES += PdfView.cpp
SOURCES += PopplerDocument.cpp

# Qt4
lessThan( QT_MAJOR_VERSION, 5 ) {
	INCLUDEPATH  += /usr/include/poppler/qt4
	LIBS         += -L/usr/lib -lpoppler-qt4
	TARGET        = pdfwidget4
}

# Qt5
greaterThan( QT_MAJOR_VERSION, 4 ) {
	INCLUDEPATH  += /usr/include/poppler/qt5
	LIBS         += -L/usr/lib -lpoppler-qt5
	QT += widgets printsupport
}

# Silent compilation
# ==================
CONFIG += silent shared warn_off

isEmpty( BUILD_PREFIX ) {
	BUILD_PREFIX = ../build
}

MOC_DIR 	= $$BUILD_PREFIX/moc
OBJECTS_DIR = $$BUILD_PREFIX/obj
RCC_DIR		= $$BUILD_PREFIX/qrc
UI_DIR      = $$BUILD_PREFIX/uic

unix {
	isEmpty(PREFIX) {
		PREFIX = /usr
	}

	INSTALLS += target
	target.path = $$PREFIX/lib/
}
