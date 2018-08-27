TEMPLATE = app
VERSION = 0.0.3

INCLUDEPATH += . ../lib/ ../mupdf/include
DEPENDPATH += . ../lib/ ../mupdf/include

# Input
SOURCES += Main.cpp

if ( !exists( "../lib/libpdfwidget.a" ) ) {
	message( "Building libpdfwidget.a" )
	system( "cd ../lib/; make static" )
}

# Qt4
lessThan( QT_MAJOR_VERSION, 5 ) {
	INCLUDEPATH  += /usr/include/poppler/qt4
	LIBS         += -L/usr/lib -lpoppler-qt4 ../lib/libpdfwidget4.a ../lib/libmupdf.a ../lib/libmupdfthird.a -lharfbuzz -lfreetype -lz -ljpeg -lopenjp2 -ljbig2dec
	TARGET        = minipdf4
}

# Qt5
greaterThan( QT_MAJOR_VERSION, 4 ) {
	INCLUDEPATH  += /usr/include/poppler/qt5
	LIBS         += -L/usr/lib -lpoppler-qt5 ../lib/libpdfwidget.a ../lib/libmupdf.a ../lib/libmupdfthird.a -lharfbuzz -lfreetype -lz -ljpeg -lopenjp2 -ljbig2dec
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
