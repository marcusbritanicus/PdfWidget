project( minipdf )
cmake_minimum_required( VERSION 3.16 )

set( PROJECT_VERSION 1.0.0 )
set( PROJECT_VERSION_MAJOR 1 )
set( PROJECT_VERSION_MINOR 0 )
set( PROJECT_VERSION_PATCH 0 )

set( PROJECT_VERSION_MAJOR_MINOR ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR} )
add_compile_definitions(VERSION_TEXT="${PROJECT_VERSION}")

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_INCLUDE_CURRENT_DIR ON )
set( CMAKE_BUILD_TYPE Release )

add_definitions ( -Wall )
if ( CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT )
    set( CMAKE_INSTALL_PREFIX "/usr" CACHE PATH "Location for installing the project" FORCE )
endif()

set( CMAKE_AUTOMOC ON )
set( CMAKE_AUTORCC ON )
set( CMAKE_AUTOUIC ON )

find_package( Qt5Core REQUIRED )
find_package( Qt5Gui REQUIRED )
find_package( Qt5Widgets REQUIRED )
find_package( Qt5Network REQUIRED )
find_package( Qt5PrintSupport REQUIRED )

include_directories(
    Core
    Core/Tools
    Gui
    Gui/Application
    Gui/Dialogs
    Gui/UI
    Gui/Widgets
    Gui/Widgets/Buttons
    Gui/Widgets/FileDialog
    Gui/Widgets/Misc
    Gui/Widgets/RecentFiles
    Gui/Widgets/Settings
    Gui/Widgets/TabWidget
)

set( HEADERS
	Core/Tools/Tools.hpp
	Global.hpp
	Gui/Dialogs/Dialog.hpp
	Gui/Dialogs/MessageDialog.hpp
	Gui/Application/qtlocalpeer.hpp
	Gui/Application/qtlockedfile.hpp
	Gui/Application/MPApplication.hpp
	Gui/Widgets/Buttons/ActionButtons.hpp
	Gui/Widgets/Buttons/ButtonSet.hpp
	Gui/Widgets/FileDialog/FileDialog.hpp
	Gui/Widgets/Misc/GuiWidgets.hpp
	Gui/Widgets/RecentFiles/RecentFilesWidget.hpp
	Gui/Widgets/Settings/Settings.hpp
	Gui/Widgets/TabWidget/TabWidget.hpp
	Gui/UI/MiniPdf.hpp
)

set( SOURCES
	Core/Tools/Tools.cpp
	Gui/Dialogs/Dialog.cpp
	Gui/Dialogs/MessageDialog.cpp
	Gui/Application/qtlocalpeer.cpp
	Gui/Application/MPApplication.cpp
	Gui/Widgets/Buttons/ButtonSet.cpp
	Gui/Widgets/Buttons/ActionButtons.cpp
	Gui/Widgets/FileDialog/FileDialog.cpp
	Gui/Widgets/Misc/GuiWidgets.cpp
	Gui/Widgets/RecentFiles/RecentFilesWidget.cpp
	Gui/Widgets/Settings/Settings.cpp
	Gui/Widgets/TabWidget/TabWidget.cpp
	Gui/UI/MiniPdf.cpp
	Main.cpp
)

set ( RESOURCES
	Resources/MiniPdf.qrc
)

add_executable( minipdf ${SOURCES} ${HEADERS} ${RESOURCES}  )
target_link_libraries(
    minipdf
    Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network Qt5::PrintSupport
    pdfwidget
)

install( TARGETS minipdf DESTINATION bin )
install( PROGRAMS minipdf.desktop DESTINATION share/applications )
install( FILES minipdf.svg DESTINATION share/icons/hicolor/scalable/apps/ )
