/*
	*
	* StyleManager.cpp - StyleManager Class
	*
*/

#include <StyleManager.hpp>

QString StyleManager::getStyleSheet( QString widget ) {

	QFile file( qApp->tr( ":/StyleSheets/%1.qss" ).arg( widget ) );
	if ( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
		return QString();

	QString QSS = QString::fromLocal8Bit( file.readAll() );
	file.close();

	return QSS;
};

QPalette StyleManager::transparentPalette() {

	QPalette pltt = qApp->palette();

	pltt.setColor( QPalette::Active, QPalette::Window, Qt::transparent );
	pltt.setColor( QPalette::Active, QPalette::WindowText, Qt::white );
	pltt.setColor( QPalette::Active, QPalette::Base, Qt::transparent );
	pltt.setColor( QPalette::Active, QPalette::AlternateBase, Qt::transparent );
	pltt.setColor( QPalette::Active, QPalette::ToolTipBase, QColor( 0, 0, 0, 206 ) );
	pltt.setColor( QPalette::Active, QPalette::ToolTipText, QColor( 0x61, 0x93, 0xCF ) );
	pltt.setColor( QPalette::Active, QPalette::Text, Qt::white );
	pltt.setColor( QPalette::Active, QPalette::Button, Qt::transparent );
	pltt.setColor( QPalette::Active, QPalette::ButtonText, Qt::white );
	pltt.setColor( QPalette::Active, QPalette::BrightText, Qt::white );
	pltt.setColor( QPalette::Active, QPalette::Link, Qt::darkGreen );

	pltt.setColor( QPalette::Disabled, QPalette::Window, Qt::transparent );
	pltt.setColor( QPalette::Disabled, QPalette::WindowText, Qt::white );
	pltt.setColor( QPalette::Disabled, QPalette::Base, Qt::transparent );
	pltt.setColor( QPalette::Disabled, QPalette::AlternateBase, Qt::transparent );
	pltt.setColor( QPalette::Disabled, QPalette::ToolTipBase, QColor( 0, 0, 0, 206 ) );
	pltt.setColor( QPalette::Disabled, QPalette::ToolTipText, QColor( 0x61, 0x93, 0xCF ) );
	pltt.setColor( QPalette::Disabled, QPalette::Text, Qt::white );
	pltt.setColor( QPalette::Disabled, QPalette::Button, Qt::transparent );
	pltt.setColor( QPalette::Disabled, QPalette::ButtonText, Qt::white );
	pltt.setColor( QPalette::Disabled, QPalette::BrightText, Qt::white );
	pltt.setColor( QPalette::Disabled, QPalette::Link, Qt::darkGreen );

	pltt.setColor( QPalette::Inactive, QPalette::Window, Qt::transparent );
	pltt.setColor( QPalette::Inactive, QPalette::WindowText, Qt::white );
	pltt.setColor( QPalette::Inactive, QPalette::Base, Qt::transparent );
	pltt.setColor( QPalette::Inactive, QPalette::AlternateBase, Qt::transparent );
	pltt.setColor( QPalette::Inactive, QPalette::ToolTipBase, QColor( 0, 0, 0, 206 ) );
	pltt.setColor( QPalette::Inactive, QPalette::ToolTipText, QColor( 0x61, 0x93, 0xCF ) );
	pltt.setColor( QPalette::Inactive, QPalette::Text, Qt::white );
	pltt.setColor( QPalette::Inactive, QPalette::Button, Qt::transparent );
	pltt.setColor( QPalette::Inactive, QPalette::ButtonText, Qt::white );
	pltt.setColor( QPalette::Inactive, QPalette::BrightText, Qt::white );
	pltt.setColor( QPalette::Inactive, QPalette::Link, Qt::darkGreen );

	return pltt;
};

QPalette StyleManager::getPalette() {

	QPalette pltt = qApp->palette();

	pltt.setColor( QPalette::Active, QPalette::Window, QColor( 30, 30, 30, 180 ) );
	pltt.setColor( QPalette::Active, QPalette::WindowText, Qt::white );
	pltt.setColor( QPalette::Active, QPalette::Base, Qt::transparent );
	pltt.setColor( QPalette::Active, QPalette::AlternateBase, QColor( 30, 30, 30, 180 ) );
	pltt.setColor( QPalette::Active, QPalette::ToolTipBase, QColor( 0, 0, 0, 206 ) );
	pltt.setColor( QPalette::Active, QPalette::ToolTipText, QColor( 0x61, 0x93, 0xCF ) );
	pltt.setColor( QPalette::Active, QPalette::Text, Qt::white );
	pltt.setColor( QPalette::Active, QPalette::Button, Qt::transparent );
	pltt.setColor( QPalette::Active, QPalette::ButtonText, Qt::white );
	pltt.setColor( QPalette::Active, QPalette::BrightText, Qt::white );
	pltt.setColor( QPalette::Active, QPalette::Link, Qt::darkGreen );

	return pltt;
};
