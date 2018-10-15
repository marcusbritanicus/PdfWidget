/*
	*
	* Copyright 2014 Britanicus <marcusbritanicus@gmail.com>
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

#include "MiniPdf.hpp"
#include "MPApplication.hpp"

int main( int argc, char **argv ) {

	/* Initialize the single application instance */
	MPApplication app( argc, argv );

	app.setOrganizationName( "MiniPdf" );
	app.setApplicationName( "MiniPdf" );

	/* We have no command line args */
	if ( argc == 1 ) {
		if ( app.sendMessage( QString() ) )
			return 0;
	}

	/* If we have command line args, we re-route them to the original instance */
	else if ( argc >= 1 ) {

		QStringList files = app.arguments();
		files.removeFirst();
		if ( app.sendMessage( files.join( ";;" ) ) )
			return 0;
	}

	MiniPdf *Gui = new MiniPdf();
	Gui->showMaximized();

	QObject::connect( &app, SIGNAL( messageReceived( const QString ) ), Gui, SLOT( messageReciever( const QString ) ) );
	app.setActivationWindow( Gui, true );

	return app.exec();
};
