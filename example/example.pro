TEMPLATE = app
TARGET = minipdf

VERSION = 0.0.3

INCLUDEPATH		+= /usr/include/poppler/qt5
LIBS       		+= -L/usr/lib -lpoppler-qt5 -L../lib/ -lpdfwidget -L../lib/mupdf/build/release
TARGET			 = minipdf
QT				+= network widgets printsupport

INCLUDEPATH += ../lib/ ../lib/mupdf/include
DEPENDPATH += ../lib/ ../lib/mupdf/include

INCLUDEPATH += . Core Core/MimeHandler Core/StandardPaths Core/Tools Gui Gui/Dialogs Gui/Editor Gui/Application Gui/Widgets
INCLUDEPATH += Gui/Widgets/Buttons Gui/Widgets/FileDialog Gui/Widgets/FileProps Gui/Widgets/Misc Gui/Widgets/RecentFiles
INCLUDEPATH += Gui/Widgets/Settings Gui/Widgets/TabWidget Gui/UI Resources Resources/icons Resources/StyleSheets

DEPENDPATH += . Core Core/MimeHandler Core/StandardPaths Core/Tools Gui Gui/Dialogs Gui/Editor Gui/Application Gui/Widgets
DEPENDPATH += Gui/Widgets/Buttons Gui/Widgets/FileDialog Gui/Widgets/FileProps Gui/Widgets/Misc Gui/Widgets/RecentFiles
DEPENDPATH += Gui/Widgets/Settings Gui/Widgets/TabWidget Gui/UI Resources Resources/icons Resources/StyleSheets

# Headers
HEADERS += Core/Tools/Tools.hpp
HEADERS += Global.hpp
HEADERS += Gui/Dialogs/Dialog.hpp
HEADERS += Gui/Dialogs/MessageDialog.hpp
HEADERS += Gui/Application/qtlocalpeer.hpp
HEADERS += Gui/Application/qtlockedfile.hpp
HEADERS += Gui/Application/MPApplication.hpp
HEADERS += Gui/Widgets/Buttons/ActionButtons.hpp
HEADERS += Gui/Widgets/Buttons/ButtonSet.hpp
HEADERS += Gui/Widgets/FileDialog/FileDialog.hpp
HEADERS += Gui/Widgets/Misc/GuiWidgets.hpp
HEADERS += Gui/Widgets/RecentFiles/RecentFilesWidget.hpp
HEADERS += Gui/Widgets/Settings/Settings.hpp
HEADERS += Gui/Widgets/TabWidget/TabWidget.hpp
HEADERS += Gui/UI/MiniPdf.hpp

# Sources
SOURCES += Core/Tools/Tools.cpp
SOURCES += Gui/Dialogs/Dialog.cpp
SOURCES += Gui/Dialogs/MessageDialog.cpp
SOURCES += Gui/Application/qtlocalpeer.cpp
SOURCES += Gui/Application/MPApplication.cpp
SOURCES += Gui/Widgets/Buttons/ButtonSet.cpp
SOURCES += Gui/Widgets/Buttons/ActionButtons.cpp
SOURCES += Gui/Widgets/FileDialog/FileDialog.cpp
SOURCES += Gui/Widgets/Misc/GuiWidgets.cpp
SOURCES += Gui/Widgets/RecentFiles/RecentFilesWidget.cpp
SOURCES += Gui/Widgets/Settings/Settings.cpp
SOURCES += Gui/Widgets/TabWidget/TabWidget.cpp
SOURCES += Gui/UI/MiniPdf.cpp
SOURCES += Main.cpp

RESOURCES += Resources/MiniPdf.qrc

# Builds
BUILD_PREFIX = $$(BUILD_DIR)

isEmpty( BUILD_PREFIX ) {
	BUILD_PREFIX = ../build
}

MOC_DIR 	= $$BUILD_PREFIX/moc
OBJECTS_DIR = $$BUILD_PREFIX/obj
RCC_DIR		= $$BUILD_PREFIX/qrc
UI_DIR      = $$BUILD_PREFIX/uic

CONFIG += warn_on silent

# C++11 Support
QMAKE_CFLAGS += -std=c++11
QMAKE_CXXFLAGS += -std=c++11

unix {
	isEmpty(PREFIX) {
		PREFIX = /usr/local
	}
	BINDIR = $$PREFIX/bin

	INSTALLS += target
	target.path = $$BINDIR
}
