TEMPLATE = lib
VERSION = 0.0.1

INCLUDEPATH += .

# Input
HEADERS += PdfDocument.hpp PdfView.hpp PdfPrinter.hpp
SOURCES += PdfDocument.cpp PdfView.cpp PdfPrinter.cpp

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
	QT += widgets printsupport
}

# Silent Compilation
CONFIG += silent

MOC_DIR         = build/moc
OBJECTS_DIR     = build/obj
RCC_DIR         = build/qrc
UI_DIR          = build/uic
