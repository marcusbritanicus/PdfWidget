/*
	*
	* Copyright 2017 Britanicus <marcusbritanicus@gmail.com>
	*

	*
	* This program is free software; you can redistribute it and/or modify
	* it under the terms of the GNU General Public License as published by
	* the Free Software Foundation; either version 2 of the License, or
	* (at your option) any later version.
	*

	*
	* This program is distributed in the hope that it will be useful,
	* but WITHOUT ANY WARRANTY; without even the implied warranty of
	* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	* GNU General Public License for more details.
	*

	*
	* You should have received a copy of the GNU General Public License
	* along with this program; if not, write to the Free Software
	* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
	* MA 02110-1301, USA.
	*
*/

// Local Headers
#include "PdfDocument.hpp"
#include "PdfView.hpp"

int main( int argc, char **argv ) {

	QApplication app( argc, argv );

	PdfView *view = new PdfView( NULL );
	view->setObjectName( "PdfView" );

	view->setViewMode( PdfView::FitPageToHeight );
	view->setLayoutMode( PdfView::Continuous );

	view->loadPdfDocument( argv[ 1 ], PdfView::MuPdfRenderBackend );

	QMainWindow *UI = new QMainWindow();
	UI->setObjectName( "MainWindow" );
	UI->setCentralWidget( view );
	UI->setWindowTitle( "MiniPDf - " + QFileInfo( argv[ 1 ] ).fileName() );

	QAction *zoomInAct = new QAction( UI );
	zoomInAct->setShortcut( QKeySequence( "Ctrl++" ) );
	UI->connect( zoomInAct, SIGNAL( triggered() ), view, SLOT( slotZoomIn() ) );
	UI->addAction( zoomInAct );

	QAction *zoomOutAct = new QAction( UI );
	zoomOutAct->setShortcut( QKeySequence( "Ctrl+-" ) );
	UI->connect( zoomOutAct, SIGNAL( triggered() ), view, SLOT( slotZoomOut() ) );
	UI->addAction( zoomOutAct );

	UI->showMaximized();

	return app.exec();
}
