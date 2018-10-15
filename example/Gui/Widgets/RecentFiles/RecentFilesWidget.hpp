/*
    *
    * RecentFiles.hpp - MiniPDF's RecentFiles Header
    *
*/

#pragma once

#include "Global.hpp"
#include "Tools.hpp"
#include "GuiWidgets.hpp"

class RecentFile : public QPushButton{
	Q_OBJECT

	public:
		RecentFile( QString );
		QString file();

	private:
		QIcon mIcon;

		QString mName;

		QString mDate;
		QString mSize;

	protected:
		void paintEvent( QPaintEvent * );
};

class RecentFiles : public QWidget {
	Q_OBJECT

	public :
		RecentFiles( QWidget *parent = 0 );

	private:
		QStackedLayout *stackLyt;

	private slots:
		void openFiles();
		void clearRecents();

	Q_SIGNALS:
		void open();
		void open( QString );
};
