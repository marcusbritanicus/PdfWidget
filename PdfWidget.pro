TEMPLATE = lib
VERSION = 0.0.1

INCLUDEPATH += .

# Input
HEADERS += PdfDocument.hpp PdfView.hpp
SOURCES += PdfDocument.cpp PdfView.cpp

# Qt4
# ===
lessThan( QT_MAJOR_VERSION, 5 ) {
	INCLUDEPATH  += /usr/include/poppler/qt4
	LIBS         += -L/usr/lib -lpoppler-qt4
	TARGET        = pdfwidget
}

# Qt5
# ===
greaterThan( QT_MAJOR_VERSION, 4 ) {
	INCLUDEPATH  += /usr/include/poppler/qt5
	LIBS         += -L/usr/lib -lpoppler-qt5
	TARGET        = pdfwidget5
	QT += widgets
}

# Silent Compilation
CONFIG += silent

isEmpty(BUILD_DIR) {
	BUILD_DIR = "build"
}

MOC_DIR 	= $$(BUILD_DIR)/PdfWidget/moc
OBJECTS_DIR = $$(BUILD_DIR)/PdfWidget/obj
RCC_DIR		= $$(BUILD_DIR)/PdfWidget/qrc
UI_DIR      = $$(BUILD_DIR)/PdfWidget/uic
