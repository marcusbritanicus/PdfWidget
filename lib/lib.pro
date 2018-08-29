TEMPLATE = lib
TARGET = pdfwidget

INCLUDEPATH += . ../mupdf/include
DEPENDPATH += . ../mupdf/include

VERSION = 0.0.3

# Input
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
	LIBS         += -L/usr/lib -lpoppler-qt4 -L../mupdf/source/ -lmupdf -L../mupdf/mujs/ -lmupdfthird
	TARGET        = pdfwidget4
}

# Qt5
greaterThan( QT_MAJOR_VERSION, 4 ) {
	INCLUDEPATH  += /usr/include/poppler/qt5
	LIBS         += -L/usr/lib -lpoppler-qt5 -L../mupdf/source/ -lmupdf -L../mupdf/mujs/ -lmupdfthird
	TARGET        = pdfwidget
	QT += widgets printsupport
}

# Silent Compilation
CONFIG += silent static
CONFIG -= shared

isEmpty( BUILD_PREFIX ) {
	BUILD_PREFIX = ../build
}

MOC_DIR 	= $$BUILD_PREFIX/moc
OBJECTS_DIR = $$BUILD_PREFIX/obj
RCC_DIR		= $$BUILD_PREFIX/qrc
UI_DIR      = $$BUILD_PREFIX/uic
