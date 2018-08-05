TEMPLATE = app
VERSION = 0.0.1

INCLUDEPATH += . ../lib/
DEPENDPATH += . ../lib/

# Input
SOURCES += Main.cpp

# Qt4
lessThan( QT_MAJOR_VERSION, 5 ) {
	INCLUDEPATH  += /usr/include/poppler/qt4
	LIBS         += -L/usr/lib -lpoppler-qt4 ../lib/libpdfwidget4.a -lmupdf -lmupdfthird -lharfbuzz -lfreetype -lz -ljpeg -lopenjp2 -ljbig2dec
	TARGET        = pdfwidget
}

# Qt5
greaterThan( QT_MAJOR_VERSION, 4 ) {
	INCLUDEPATH  += /usr/include/poppler/qt5
	LIBS         += -L/usr/lib -lpoppler-qt5 ../lib/libpdfwidget.a -lmupdf -lmupdfthird -lharfbuzz -lfreetype -lz -ljpeg -lopenjp2 -ljbig2dec
	TARGET        = pdfwidget5
	QT += widgets printsupport
}

# Silent Compilation
CONFIG += silent

MOC_DIR 	= $$(BUILD_DIR)/PdfWidget/moc
OBJECTS_DIR = $$(BUILD_DIR)/PdfWidget/obj
RCC_DIR		= $$(BUILD_DIR)/PdfWidget/qrc
UI_DIR      = $$(BUILD_DIR)/PdfWidget/uic
