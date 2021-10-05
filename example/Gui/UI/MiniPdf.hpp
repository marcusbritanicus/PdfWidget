/*
	*
	* MiniPdf.hpp - MiniPdf Header
	*
*/

#pragma once

#include "Global.hpp"

#include <pdfwidget/View.hpp>
#include <pdfwidget/Document.hpp>

#include "TabWidget.hpp"
#include "GuiWidgets.hpp"
#include "Settings.hpp"
#include "MessageDialog.hpp"

class MiniPdf : public QMainWindow {
	Q_OBJECT

	public :
		MiniPdf();

	private :
		void setWindowProperties();
		void setupGui();

		void setupConnections();
		void addToRecentList( QString );

		TabWidget *Tabs;
		QStringList filesQueue;
		QStringList openedFiles;

		QPoint dragPosition;
		int recentIdx;

	private slots :
		void openFiles();
		void openRecentFiles();
		void loadFiles();
		void reload();

		void printFile();

		void settings();

		void closeTab();
		void closeTab( int index );
		void closeAllTabs();

		void dfreeMode();

		void messageReciever( const QString );

		void toggleMaximizeRestore();

	protected :
		void closeEvent( QCloseEvent * );
};
