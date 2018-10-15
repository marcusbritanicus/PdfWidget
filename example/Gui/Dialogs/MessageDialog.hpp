/*
	*
	* MessageDialog.hpp - MessageDialog.cpp header
	*
*/

#pragma once

#include "Global.hpp"
#include "Dialog.hpp"
#include "GuiWidgets.hpp"
#include "ButtonSet.hpp"

class MessageDialog : public Dialog {
	Q_OBJECT

	public:
		enum StandardButton {
			Ok                 = 0x00000400,
			Yes                = 0x00004000,
			YesToAll           = 0x00008000,
			No                 = 0x00010000,
			NoToAll            = 0x00020000,
			Abort              = 0x00040000,
			Retry              = 0x00080000,
			Ignore             = 0x00100000,
			Cancel             = 0x00400000,
			Save               = 0x00000800,
			Discard            = 0x00800000,
		};

		MessageDialog( QWidget*, QString icon = "", QString title = "", QString text = "",
				QList<StandardButton> btns = QList<StandardButton>() << MessageDialog::Ok, QWidget *info = 0 );
		void setIcon( QString );
		void setTitle( QString );
		void setText( QString );
		void setButtons( QStringList );

		static void aboutNewBreeze();
		static void aboutQt();
		static int information( QWidget*, QString, QString, QList<StandardButton> btns = QList<StandardButton>() << MessageDialog::Ok, QWidget *infoWidget = 0 );
		static int question( QWidget*, QString, QString, QList<StandardButton> btns = QList<StandardButton>() << MessageDialog::Ok, QWidget *infoWidget = 0 );
		static int warning( QWidget*, QString, QString, QList<StandardButton> btns = QList<StandardButton>() << MessageDialog::Ok, QWidget *infoWidget = 0 );
		static int error( QWidget*, QString, QString, QList<StandardButton> btns = QList<StandardButton>() << MessageDialog::Ok, QWidget *infoWidget = 0 );
		static int critical( QWidget*, QString, QString, QList<StandardButton> btns = QList<StandardButton>() << MessageDialog::Ok, QWidget *infoWidget = 0 );

		int retValue;

	private:
		QLabel *iconLbl, *textLbl;
		QPushButton *detailsBtn;
		QWidget *infoWidget;

		QByteArray origGeometry;

	protected:
		void keyPressEvent( QKeyEvent *keyEvent );

	public slots:
		void close();

	private slots:
		void toggleInfoWidget();
		void buttonClickHandler();

	signals:
		void closed();
};
