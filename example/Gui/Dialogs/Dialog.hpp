/*
	*
	* Dialog.hpp - Dialog.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "GuiWidgets.hpp"

class Dialog : public QDialog {
	Q_OBJECT

	public:
		Dialog( QWidget *parent = 0 );

		void setLayout( QLayout* );
		void setDialogTitle( QString );
		void setDialogIcon( QIcon );

	private:
		void setupGUI();
		void setupActions();
		void setupDialogProperties();

	protected:
		void closeEvent( QCloseEvent* );

	Q_SIGNALS:
		void closed();
};
