TEMPLATE = lib
TARGET = pdfwidget

INCLUDEPATH += .

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
	LIBS         += -L/usr/lib -lpoppler-qt4 -lmupdf -lmupdfthird -lharfbuzz -lfreetype -lz -ljpeg -lopenjp2 -ljbig2dec
	TARGET        = pdfwidget4
}

# Qt5
greaterThan( QT_MAJOR_VERSION, 4 ) {
	INCLUDEPATH  += /usr/include/poppler/qt5
	LIBS         += -L/usr/lib -lpoppler-qt5 -lmupdf -lmupdfthird -lharfbuzz -lfreetype -lz -ljpeg -lopenjp2 -ljbig2dec
	TARGET        = pdfwidget
	QT += widgets printsupport
}

# Silent Compilation
CONFIG += silent shared_and_static

MOC_DIR 	= $$(BUILD_DIR)/PdfWidget/moc
OBJECTS_DIR = $$(BUILD_DIR)/PdfWidget/obj
RCC_DIR		= $$(BUILD_DIR)/PdfWidget/qrc
UI_DIR      = $$(BUILD_DIR)/PdfWidget/uic
