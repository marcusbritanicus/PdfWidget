/*
    *
    * TabWidget.hpp - MiniPDF's TabWidget Header
    *
*/

#pragma once

#include "Global.hpp"
#include "GuiWidgets.hpp"

class TabWidget : public QTabWidget {
	Q_OBJECT

	public :
		TabWidget( QWidget *parent = 0 );

		void removeTab( int );
		void setTabTextColor( int, QColor );

	private slots:
		void nextTab();
		void prevTab();

		void moveTabRight();
		void moveTabLeft();

		void moveTabFirst();
		void moveTabLast();

	Q_SIGNALS:
		void open();
};
