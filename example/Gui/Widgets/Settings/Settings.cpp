/*
    *
    * wSettings.cpp - MiniPDF's Settings Class
    *
*/

#include "Global.hpp"
#include "Settings.hpp"

wSettings::wSettings( QWidget *parent ) : QDialog( parent ) {

	setupGUI();
	setWindowProperties();
}

void wSettings::setFont( const QFont &nFont ) {

	QDialog::setFont( nFont );
	fontBtn->setText( tr( "%1, %2" ).arg( nFont.family() ).arg( nFont.pointSize() ) );
}

void wSettings::setupGUI() {

	QFont dFont = settings.value( "Font" ).value<QFont>();

	fontLbl = new QLabel( "Font: " );
	fontBtn = new QPushButton( tr( "%1, %2" ).arg( dFont.family() ).arg( dFont.pointSize() ) );
	fontBtn->setMinimumWidth( 200 );

	QPushButton *closeBtn = new QPushButton();
	closeBtn->setText( "&Close" );
	closeBtn->setIcon( QIcon::fromTheme( "dialog-close" ) );

	QVBoxLayout *baseLyt = new QVBoxLayout();
	QHBoxLayout *fontLyt = new QHBoxLayout();
	QHBoxLayout *cbtnLyt = new QHBoxLayout();

	fontLyt->addWidget( fontLbl );
	fontLyt->addWidget( fontBtn );

	cbtnLyt->addStretch( 0 );
	cbtnLyt->addWidget( closeBtn );

	baseLyt->addLayout( fontLyt );
	baseLyt->addLayout( cbtnLyt );

	setLayout( baseLyt );

	connect( fontBtn, SIGNAL( clicked() ), this, SLOT( chooseFont() ) );
	connect( closeBtn, SIGNAL( clicked() ), this, SLOT( close() ) );
};

void wSettings::setWindowProperties() {

	setWindowTitle( "MiniPDF - Settings" );
	setMinimumWidth( 250 );
};

void wSettings::chooseFont() {

	bool ok = false;
	QFont newFont = QFontDialog::getFont( &ok, settings.value( "Font" ).value<QFont>(), this );

	if ( ok ) {
		fontBtn->setText( tr( "%1, %2" ).arg( newFont.family() ).arg( newFont.pointSize() ) );

		settings.setValue( "Font", QVariant( newFont ) );
		settings.sync();

		emit fontSet();
	}
};
