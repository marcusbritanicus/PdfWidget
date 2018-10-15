/*
    *
    * * TabWidget.cpp - MiniPDF's TabWidget
    *
*/

#include "TabWidget.hpp"

TabWidget::TabWidget( QWidget *parent ) : QTabWidget( parent ) {

	// TabBar settings
	setMovable( true );
	setDocumentMode( true );
	setTabsClosable( true );

	setElideMode( Qt::ElideMiddle );

	QToolButton *openBtn = new QToolButton( this );
	openBtn->setIcon( QIcon( ":/icons/open.png" ) );
	openBtn->setShortcut( tr( "Ctrl+O" ) );
	connect( openBtn, SIGNAL( clicked() ), this, SIGNAL( open() ) );

	setCornerWidget( openBtn, Qt::TopLeftCorner );

	QAction *nextTabAct = new QAction( "Next Tab", this );
	nextTabAct->setShortcuts( QList<QKeySequence>() << QKeySequence( "Ctrl+Tab" ) << QKeySequence( "Ctrl+PgDown" ) );
	connect( nextTabAct, SIGNAL( triggered() ), this, SLOT( nextTab() ) );
	addAction( nextTabAct );

	QAction *prevTabAct = new QAction( "Previous Tab", this );
	prevTabAct->setShortcuts( QList<QKeySequence>() << QKeySequence( "Ctrl+Shift+Tab" ) << QKeySequence( "Ctrl+PgUp" ) );
	connect( prevTabAct, SIGNAL( triggered() ), this, SLOT( prevTab() ) );
	addAction( prevTabAct );

	QAction *moveTabRightAct = new QAction( "Move Tab to Right", this );
	moveTabRightAct->setShortcuts( QList<QKeySequence>() << QKeySequence( "Alt+PgDown" ) );
	connect( moveTabRightAct, SIGNAL( triggered() ), this, SLOT( moveTabRight() ) );
	addAction( moveTabRightAct );

	QAction *moveTabLeftAct = new QAction( "Previous Tab", this );
	moveTabLeftAct->setShortcuts( QList<QKeySequence>() << QKeySequence( "Alt+PgUp" ) );
	connect( moveTabLeftAct, SIGNAL( triggered() ), this, SLOT( moveTabLeft() ) );
	addAction( moveTabLeftAct );

	QAction *moveTabLastAct = new QAction( "Move Tab to Last", this );
	moveTabLastAct->setShortcuts( QList<QKeySequence>() << QKeySequence( "Alt+Shift+PgDown" ) );
	connect( moveTabLastAct, SIGNAL( triggered() ), this, SLOT( moveTabLast() ) );
	addAction( moveTabLastAct );

	QAction *moveTabFirstAct = new QAction( "Move Tab to First", this );
	moveTabFirstAct->setShortcuts( QList<QKeySequence>() << QKeySequence( "Alt+Shift+PgUp" ) );
	connect( moveTabFirstAct, SIGNAL( triggered() ), this, SLOT( moveTabFirst() ) );
	addAction( moveTabFirstAct );
};

void TabWidget::removeTab( int index ) {
	/* Does the tab get automatically get removed if we delete the widget? */

	widget( index )->deleteLater();
	QTabWidget::removeTab( index );
};

void TabWidget::setTabTextColor( int index, QColor color ) {

	tabBar()->setTabTextColor( index, color );
	tabBar()->update();
};

void TabWidget::nextTab() {

	if ( currentIndex() + 1 == count() )
		setCurrentIndex( 0 );

	else
		setCurrentIndex( currentIndex() + 1 );
};

void TabWidget::prevTab() {

	if ( currentIndex() - 1 < 0 )
		setCurrentIndex( count() - 1 );

	else
		setCurrentIndex( currentIndex() - 1 );
};

void TabWidget::moveTabRight() {

	if ( currentIndex() + 1 == count() )
		return;

	else
		tabBar()->moveTab( currentIndex(), currentIndex() + 1 );
};

void TabWidget::moveTabLeft() {

	if ( currentIndex() == 0 )
		return;

	else
		tabBar()->moveTab( currentIndex(), currentIndex() - 1 );
};

void TabWidget::moveTabFirst() {

	tabBar()->moveTab( currentIndex(), 0 );
};

void TabWidget::moveTabLast() {

	tabBar()->moveTab( currentIndex(), count() - 1 );
};
