TEMPLATE = lib
TARGET = pdfwidget

INCLUDEPATH += . ../mupdf/include
DEPENDPATH += . ../mupdf/include

VERSION = 0.0.3

if ( !exists( "libmupdf.a" ) ) {

	message( "Could not locate libmupdf.a and libmupdfthird.a" )
	message( "Building MuPDF...")

	# Build the mupdf library
	system( "cd ../mupdf/; sh build.sh" )

	# Copy the libmupdf* static objects to this folder
	system( "mv ../mupdf/build/release/libmupdf*.a ." )
}

QMAKE_DISTCLEAN += ../mupdf/include/mupdf/pdf/name-table.h ../mupdf/source/pdf/pdf-name-table.h libmupdf.a libmupdfthird.a
QMAKE_CLEAN += ../mupdf/include/mupdf/pdf/name-table.h ../mupdf/source/pdf/pdf-name-table.h

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
	LIBS         += -L/usr/lib -lpoppler-qt4 libmupdf.a libmupdfthird.a
	TARGET        = pdfwidget4
}

# Qt5
greaterThan( QT_MAJOR_VERSION, 4 ) {
	INCLUDEPATH  += /usr/include/poppler/qt5
	LIBS         += -L/usr/lib -lpoppler-qt5 libmupdf.a libmupdfthird.a
	TARGET        = pdfwidget
	QT += widgets printsupport
}

# Silent Compilation
CONFIG += silent shared_and_static

isEmpty( BUILD_PREFIX ) {
	BUILD_PREFIX = ../build
}

MOC_DIR 	= $$BUILD_PREFIX/moc
OBJECTS_DIR = $$BUILD_PREFIX/obj
RCC_DIR		= $$BUILD_PREFIX/qrc
UI_DIR      = $$BUILD_PREFIX/uic
