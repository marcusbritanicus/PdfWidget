/*
	*
	* wSettings.hpp - MiniPDF's Settings Class Header
	*
*/

#pragma once

#include "Global.hpp"

class wSettings : public QDialog {
	Q_OBJECT

	public :
		wSettings( QWidget *parent = 0 );
		void setFont( const QFont & );

		QLabel *fontLbl;
		QPushButton *fontBtn;

	private :
		void setupGUI();
		void setWindowProperties();

	private slots :
		void chooseFont();

	signals :
		void fontSet();
};
