/*
	*
	* MiniPdf.cpp - MiniPdf UI Class
	*
*/

// Local Headers
#include "Global.hpp"
#include "MiniPdf.hpp"
#include "Tools.hpp"
#include "FileDialog.hpp"
#include "RecentFilesWidget.hpp"

MiniPdf::MiniPdf() : QMainWindow() {

	// Setup base gui
	setWindowProperties();
	setupGui();
	setupConnections();

	recentIdx = -1;

	if ( qApp->arguments().count() > 1 ) {

		filesQueue = qApp->arguments();
		filesQueue.removeFirst();
		loadFiles();
	}

	else {

		openRecentFiles();
	}
};

void MiniPdf::setupGui()  {

	// Setup the tab widget
	Tabs = new TabWidget( this );

	QVBoxLayout *bLyt = new QVBoxLayout();
	bLyt->setContentsMargins( 1, 1, 1, 1 );
	bLyt->setSpacing( 0 );

	bLyt->addWidget( Tabs );

	QWidget *Base = new QWidget( this );
	Base->setObjectName( "guiBase" );
	Base->setLayout( bLyt );

	setCentralWidget( Base );
};

void MiniPdf::setWindowProperties() {

	setWindowTitle( "MiniPdf" );
	setWindowIcon( QIcon( ":/icons/minipdf.png" ) );

	setWindowFlags( Qt::Window );

	setMinimumSize( QSize( 800, 600 ) );
};

void MiniPdf::setupConnections() {

	// Setup various connections to slots
	connect( Tabs, SIGNAL( tabCloseRequested( int ) ), this, SLOT( closeTab( int ) ) );
	connect( Tabs, SIGNAL( open() ), this, SLOT( openFiles() ) );

	// Distraction-free mode
	//QAction *dfreeAct = new QAction( QIcon( ":/icons/info.png" ), "D-Free Mode", this );
	//dfreeAct->setShortcut( QKeySequence( "Ctrl+Shift+D" ) );
	//connect( dfreeAct, SIGNAL( triggered() ), this, SLOT( dfreeMode() ) );
	//addAction( dfreeAct );
};

void MiniPdf::addToRecentList( QString file ) {

	QStringList recent = ::settings.value( "Recent" ).toStringList();

	/* Add the just opened file to the list */
	if ( recent.contains( file ) )
		recent.move( recent.indexOf( file ), 0 );

	else
		recent.prepend( QFileInfo( file ).absoluteFilePath() );

	/* If we have more than 10 recent, remove the older ones */
	while ( recent.count() > 10 )
		recent.removeLast();

	::settings.setValue( "Recent", recent );
	::settings.sync();
};

void MiniPdf::openFiles() {

	if ( recentIdx >= 0 ) {
		Tabs->removeTab( recentIdx );
		recentIdx = -1;
	}

	if ( Tabs->currentIndex() != recentIdx )
		filesQueue << FileDialog::getOpenFileNames( this, dirName( openedFiles.at( Tabs->currentIndex() ) ) );

	else
		filesQueue << FileDialog::getOpenFileNames( this, QDir::homePath() );

	if ( !filesQueue.isEmpty() )
		loadFiles();

	if ( not Tabs->count() )
		openRecentFiles();
};

void MiniPdf::openRecentFiles() {

	if ( recentIdx >= 0 ) {
		Tabs->setCurrentIndex( recentIdx );
		return;
	}

	RecentFiles *rFiles = new RecentFiles( this );

	connect( rFiles, SIGNAL( open() ), this, SLOT( openFiles() ) );
	connect( rFiles, SIGNAL( open( QString ) ), this, SLOT( messageReciever( const QString ) ) );

	recentIdx = Tabs->addTab( rFiles, QIcon( ":/icons/open-recent.png" ), "Recent Files" );
	Tabs->setCurrentIndex( recentIdx );
	Tabs->currentWidget()->setFocus();
};

void MiniPdf::loadFiles() {

	for( int i = 0; i < filesQueue.count(); i++ ) {
		if ( openedFiles.contains( filesQueue[ i ] ) )
			continue;

		PdfView *view = new PdfView( this );
		view->loadPdfDocument( filesQueue[ i ], PdfView::MuPdfRenderBackend );

		/* Zoom In */
		QAction *zoomInAct = new QAction( view );
		zoomInAct->setShortcut( QKeySequence( "Ctrl++" ) );
		connect( zoomInAct, SIGNAL( triggered() ), view, SLOT( slotZoomIn() ) );
		view->addAction( zoomInAct );

		/* Zoom Out */
		QAction *zoomOutAct = new QAction( view );
		zoomOutAct->setShortcut( QKeySequence( "Ctrl+-" ) );
		connect( zoomOutAct, SIGNAL( triggered() ), view, SLOT( slotZoomOut() ) );
		view->addAction( zoomOutAct );

		Tabs->addTab( view, QIcon::fromTheme( "application-pdf", QIcon( ":/icons/pdf.png" ) ), baseName( filesQueue[ i ] ) );
		openedFiles << filesQueue[ i ];

		/* Add the just opened file to the list */
		addToRecentList( filesQueue[ i ] );
	}

	filesQueue.clear();

	Tabs->setCurrentIndex( Tabs->count() - 1 );
	qobject_cast<PdfView*>( Tabs->currentWidget() )->setFocus();
};

void MiniPdf::reload() {

	qobject_cast<PdfView*>( Tabs->currentWidget() )->reload();
};

void MiniPdf::printFile() {

};

void MiniPdf::settings() {

	wSettings *settingsDlg = new wSettings();
	connect( settingsDlg, SIGNAL( fontSet() ), this, SLOT( setFont() ) );
	settingsDlg->exec();
};

void MiniPdf::closeTab() {

	qWarning() << "Closing tab";

	int idx = Tabs->currentIndex();
	if ( idx == recentIdx ) {
		qWarning() << "Removing tab:" << idx;
		Tabs->removeTab( recentIdx );
		recentIdx = -1;

		if ( not Tabs->count() ) {
			qWarning() << "Opening recent files";
			openRecentFiles();
		}

		return;
	}

	openedFiles.takeAt( idx );
	Tabs->removeTab( Tabs->currentIndex() );

	if ( Tabs->count() )
		qobject_cast<PdfView*>( Tabs->currentWidget() )->setFocus();

	else
		openRecentFiles();
};

void MiniPdf::closeTab( int index ) {

	if ( index == recentIdx ) {
		Tabs->removeTab( recentIdx );
		recentIdx = -1;

		if ( not Tabs->count() )
			openRecentFiles();

		return;
	}

	openedFiles.takeAt( index );
	Tabs->removeTab( index );

	if ( Tabs->count() )
		qobject_cast<PdfView*>( Tabs->currentWidget() )->setFocus();

	else
		openRecentFiles();
};

void MiniPdf::closeAllTabs() {

	while ( Tabs->count() )
		Tabs->removeTab( Tabs->currentIndex() );

	openRecentFiles();
	openedFiles.clear();
};

void MiniPdf::dfreeMode() {

	//if ( Tabs->currentIndex() == recentIdx )
		//return;

	/* Hide the main window */
	//hide();

	/* Open the distraction-free widget */
	//PdfView *view = qobject_cast<PdfView *>( Tabs->currentWidget() );
	//ed->setParent( 0 );
	//ed->showFullScreen();

	//int cIdx = Tabs->currentIndex();
	//Tabs->removeTab( cIdx );

	//ed->exec();

	//Tabs->insertTab( cIdx, ed, getPixmap( ed->fileName() ), baseName( ed->fileName() ) );

	/* Show the main window on exit */
	//show();
};

void MiniPdf::messageReciever( QString msg ) {

	if ( recentIdx >= 0 ) {
		Tabs->removeTab( recentIdx );
		recentIdx = -1;
	}

	if ( msg.count() ) {
		Q_FOREACH( QString file, msg.split( ";;" ) ) {
			if ( openedFiles.contains( file ) )
				continue;

			PdfView *view = new PdfView( this );
			view->loadPdfDocument( file, PdfView::MuPdfRenderBackend );

			/* Zoom In */
			QAction *zoomInAct = new QAction( view );
			zoomInAct->setShortcut( QKeySequence( "Ctrl++" ) );
			connect( zoomInAct, SIGNAL( triggered() ), view, SLOT( slotZoomIn() ) );
			view->addAction( zoomInAct );

			/* Zoom Out */
			QAction *zoomOutAct = new QAction( view );
			zoomOutAct->setShortcut( QKeySequence( "Ctrl+-" ) );
			connect( zoomOutAct, SIGNAL( triggered() ), view, SLOT( slotZoomOut() ) );
			view->addAction( zoomOutAct );

			int idx = Tabs->addTab( view, QIcon( getPixmap( file ) ) , baseName( file ) );
			Tabs->setCurrentIndex( idx );

			qobject_cast<PdfView*>( Tabs->currentWidget() )->setFocus();

			/* Add the just opened file to the list */
			addToRecentList( file );

			if ( not openedFiles.contains( file ) )
				openedFiles << file;
		}
	}

	if ( not Tabs->count() )
		openRecentFiles();
};

void MiniPdf::toggleMaximizeRestore() {

	if ( isMaximized() )
		showNormal();

	else
		showMaximized();
};

void MiniPdf::closeEvent( QCloseEvent *cEvent ) {

	for( int i = 0; i < Tabs->count(); i++ )
		Tabs->removeTab( i );

	cEvent->accept();
};
