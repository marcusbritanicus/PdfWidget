/*
    *
    * * RecentFiles.cpp - MiniPDF's RecentFiles
    *
*/

#include "RecentFilesWidget.hpp"
#include "MessageDialog.hpp"

RecentFile::RecentFile( QString path ) : QPushButton() {

	setToolTip( path );

	QFileInfo info( path );

	mName = baseName( path );
	mIcon = getPixmap( path );
	mDate = info.lastModified().toString( "ddd, MMM dd, yyyy hh:mm:ss AP" );
	mSize = formatSize( info.size() );

	setFixedSize( 270, 64 );
	setCheckable( true );
	setChecked( false );

	setFocusPolicy( Qt::NoFocus );
};

QString RecentFile::file() {

	return toolTip();
};

void RecentFile::paintEvent( QPaintEvent *pEvent ) {

	QPushButton::paintEvent( pEvent );
	QPainter painter( this );

	if ( isChecked() ) {
		/* HighLight color */
		QColor highlight = palette().color( QPalette::Highlight );
		highlight.setAlpha( 150 );

		painter.save();
		painter.setPen( Qt::NoPen );
		painter.setBrush( highlight );
		painter.drawRoundedRect( QRectF( rect() ).adjusted( 1.5, 1.5, 0, 0 ), 2.0, 2.0 );
		painter.restore();

		painter.setPen( palette().color( QPalette::HighlightedText ) );
	}

	painter.drawPixmap( QRect( 8, 8, 48, 48 ), mIcon.pixmap( 48 ) );

	painter.drawText( QRect( 64,  9, 206, 16 ), Qt::AlignVCenter | Qt::AlignLeft, mName );
	painter.drawText( QRect( 64, 25, 206, 16 ), Qt::AlignVCenter | Qt::AlignLeft, mSize );
	painter.drawText( QRect( 64, 41, 206, 16 ), Qt::AlignVCenter | Qt::AlignLeft, mDate );
	painter.end();

	pEvent->accept();
};

RecentFiles::RecentFiles( QWidget *parent ) : QWidget( parent ) {

	stackLyt = new QStackedLayout();

	QStringList recentList;
	Q_FOREACH( QString recent, settings.value( "Recent" ).toStringList() ) {
		if ( QFile::exists( recent ) )
			recentList << recent;
	}

	/* recentList Files Layout */
	QGridLayout *recentLyt = new QGridLayout();

	for( int i = 0; i < recentList.count(); i++ ) {
		RecentFile *btn = new RecentFile( recentList[ i ] );

		if ( i % 2 == 0 )
			recentLyt->addWidget( btn, i / 2, 0 );

		else
			recentLyt->addWidget( btn, i / 2, 1 );
	}

	QPushButton *openSelBtn = new QPushButton( QIcon::fromTheme( "document-open" ), "Open &Selected" );
	openSelBtn->setFixedWidth( 140 );
	connect( openSelBtn, SIGNAL( clicked() ), this, SLOT( openFiles() ) );

	QPushButton *clearRecentBtn = new QPushButton( QIcon( ":/icons/clear.png" ), "&Clear Recent" );
	clearRecentBtn->setFixedWidth( 140 );
	connect( clearRecentBtn, SIGNAL( clicked() ), this, SLOT( clearRecents() ) );

	QPushButton *openBtn = new QPushButton( QIcon::fromTheme( "document-open" ), "&Open" );
	openBtn->setFixedWidth( 140 );
	connect( openBtn, SIGNAL( clicked() ), this, SIGNAL( open() ) );

	QVBoxLayout *btnLyt = new QVBoxLayout();

	btnLyt->addStretch();
	btnLyt->addWidget( openSelBtn );
	btnLyt->addWidget( clearRecentBtn );
	btnLyt->addStretch();
	btnLyt->addWidget( openBtn );
	btnLyt->addStretch();

	QHBoxLayout *baseLyt1 = new QHBoxLayout();
	baseLyt1->addLayout( recentLyt );
	baseLyt1->addWidget( Separator::vertical() );
	baseLyt1->addLayout( btnLyt );

	QWidget *baseWidget1 = new QWidget();
	baseWidget1->setLayout( baseLyt1 );

	QPushButton *openBtn2 = new QPushButton( QIcon::fromTheme( "document-open" ), "&Open" );
	openBtn2->setFixedWidth( 140 );
	connect( openBtn2, SIGNAL( clicked() ), this, SIGNAL( open() ) );

	QVBoxLayout *baseLyt2 = new QVBoxLayout();
	baseLyt2->addStretch();
	baseLyt2->addWidget( openBtn2, 0, Qt::AlignCenter );
	baseLyt2->addStretch();

	QWidget *baseWidget2 = new QWidget();
	baseWidget2->setLayout( baseLyt2 );

	stackLyt->addWidget( baseWidget1 );
	stackLyt->addWidget( baseWidget2 );

	setLayout( stackLyt );

	if ( recentList.count() )
		stackLyt->setCurrentIndex( 0 );

	else
		stackLyt->setCurrentIndex( 1 );
};

void RecentFiles::openFiles() {

	QStringList files;
	QList<RecentFile*> rfBtns = findChildren<RecentFile *>();
	Q_FOREACH( RecentFile *rBtn, rfBtns )
		if ( rBtn->isChecked() ) {
			files << rBtn->file();
			rBtn->setChecked( false );
		}

	if ( files.count() )
		emit open( files.join( ";;" ) );
};

void RecentFiles::clearRecents() {

	int ret = MessageDialog::question(
		this,
		"MiniPDF - Clear Recents List?",
		"You are about to clear the list of recent files. This action can not be undone. Do you want to proceed?",
		QList<MessageDialog::StandardButton>() << MessageDialog::Yes << MessageDialog::No
	);

	if ( ret == MessageDialog::No )
		return;

	settings.setValue( "Recent", QStringList() );
	settings.sync();

	stackLyt->setCurrentIndex( 1 );
};
