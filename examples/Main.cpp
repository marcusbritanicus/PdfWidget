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
#include "PdfView.hh"

int main( int argc, char **argv ) {

	QApplication app( argc, argv );

	PdfDocument *Pdf = new PdfDocument( argv[ 1 ] );

	PdfView *view = new PdfView( NULL );
	view->setPdfDocument( Pdf );

	QAction *zoomInAct = new QAction( view );
	zoomInAct->setShortcut( QKeySequence( "Ctrl++" ) );
	QObject::connect( zoomInAct, SIGNAL( triggered() ), view, SLOT( slotZoomIn() ) );
	// addAction( zoomInAct );

	QAction *zoomOutAct = new QAction( view );
	zoomOutAct->setShortcut( QKeySequence( "Ctrl+-" ) );
	QObject::connect( zoomOutAct, SIGNAL( triggered() ), view, SLOT( slotZoomOut() ) );
	// addAction( zoomOutAct );

	view->showMaximized();

	return app.exec();
}
