TEMPLATE = app
VERSION = 0.0.3

INCLUDEPATH += . ../lib/ ../mupdf/include
DEPENDPATH += . ../lib/ ../mupdf/include

# Input
SOURCES += Main.cpp

# Qt4
lessThan( QT_MAJOR_VERSION, 5 ) {
	INCLUDEPATH  += /usr/include/poppler/qt4
	LIBS         += -L/usr/lib -lpoppler-qt4 -L../lib/ -lpdfwidget4 -L../mupdf/source/ -lmupdf -L../mupdf/mujs/ -lmupdfthird -lharfbuzz -lfreetype -lz -ljpeg -lopenjp2 -ljbig2dec
	TARGET        = minipdf4
}

# Qt5
greaterThan( QT_MAJOR_VERSION, 4 ) {
	INCLUDEPATH  += /usr/include/poppler/qt5
	LIBS         += -L/usr/lib -lpoppler-qt5 -L../lib/ -lpdfwidget -L../mupdf/source/ -lmupdf -L../mupdf/mujs/ -lmupdfthird -lharfbuzz -lfreetype -lz -ljpeg -lopenjp2 -ljbig2dec
	TARGET        = minipdf
	QT += widgets printsupport
}

# Silent Compilation
CONFIG += silent

isEmpty( BUILD_PREFIX ) {
	BUILD_PREFIX = ../build
}

MOC_DIR 	= $$BUILD_PREFIX/moc
OBJECTS_DIR = $$BUILD_PREFIX/obj
RCC_DIR		= $$BUILD_PREFIX/qrc
UI_DIR      = $$BUILD_PREFIX/uic
