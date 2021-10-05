project( pdfwidget )
cmake_minimum_required( VERSION 3.16 )

set( PROJECT_VERSION 0.0.3 )
set( PROJECT_VERSION_MAJOR 0 )
set( PROJECT_VERSION_MINOR 0 )
set( PROJECT_VERSION_PATCH 3 )

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

find_package ( Qt5Widgets REQUIRED )
find_package ( Qt5Core REQUIRED )
find_package ( Qt5Gui REQUIRED )
find_package ( Qt5PrintSupport REQUIRED )

include_directories(
    Document
    MuPdf
    Poppler
    View
)

set ( SOURCES
    Document/Document.cpp
    Document/Navigation.cpp
    Document/Renderer.cpp
    MuPdf/MuPdfDocument.cpp
    MuPdf/MuPdfDocument.hpp
    Poppler/PopplerDocument.cpp
    Poppler/PopplerDocument.hpp
    View/View.cpp
    View/ViewImpl.hpp
    View/ViewWidgets.cpp
    Document/RendererImpl.hpp
)

set ( HEADERS
    Document/Document.hpp
    Document/Navigation.hpp
    Document/RenderOptions.hpp
    Document/Renderer.hpp
    View/View.hpp
    View/ViewWidgets.hpp
)

add_library( pdfwidget SHARED ${SOURCES} ${HEADERS}  )
set_target_properties( pdfwidget PROPERTIES VERSION ${PROJECT_VERSION} )
set_target_properties( pdfwidget PROPERTIES SOVERSION ${PROJECT_VERSION} )
set_target_properties( pdfwidget PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR_MINOR} )
set_target_properties( pdfwidget PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR} )
target_link_libraries(
    pdfwidget
    Qt5::Core Qt5::Gui Qt5::Widgets Qt5::GuiPrivate Qt5::WidgetsPrivate
    poppler-qt5
    mupdf freetype z harfbuzz jpeg openjp2 jbig2dec mujs desq-core
)

install( TARGETS pdfwidget EXPORT LIBRARY )
install( FILES ${HEADERS} DESTINATION include/pdfwidget/ )
