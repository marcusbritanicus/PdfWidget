/*
	*
	* MessageDialog.cpp - MessageDialog class
	*
*/

#include "MessageDialog.hpp"

MessageDialog::MessageDialog( QWidget *parent, QString icon, QString title, QString text, QList<StandardButton> btns, QWidget *info ) : Dialog( parent ) {

	setDialogIcon( QIcon( ":/icons/minipdf.png" ) );

	QVBoxLayout *dlgLyt = new QVBoxLayout();
	dlgLyt->setContentsMargins( QMargins() );

	QHBoxLayout *msgLyt = new QHBoxLayout();
	QHBoxLayout *btnLyt = new QHBoxLayout();

	iconLbl = new QLabel();
	iconLbl->setMaximumSize( QSize( 64, 64 ) );
	iconLbl->setPixmap( QPixmap( icon ).scaled( 64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );

	textLbl = new QLabel( text );
	textLbl->setWordWrap( true );

	setTitle( title );

	msgLyt->addWidget( iconLbl );
	msgLyt->addWidget( textLbl );

	if ( btns.count() == 0 )
		btns << MessageDialog::Ok;

	if ( info ) {
		infoWidget = qobject_cast<QWidget *>( info );
		infoWidget->hide();

		detailsBtn = new QPushButton( QIcon( ":/icons/info.png" ), "&More" );
		connect( detailsBtn, SIGNAL( clicked() ), this, SLOT( toggleInfoWidget() ) );
		btnLyt->addWidget( detailsBtn );
	}

	btnLyt->addStretch( 0 );

	ButtonSet *bSet = new ButtonSet();

	QStringList labels;
	QList<QIcon> icons;

	foreach( StandardButton btn, btns ) {
		QPushButton *button;
		switch( btn ) {
			case MessageDialog::Ok :
				button = new QPushButton( QIcon( ":/icons/ok.png" ), "&Ok" );
				button->setObjectName( tr( "okBtn" ) );
				connect( button, SIGNAL( clicked() ), this, SLOT( buttonClickHandler() ) );
				bSet->addSegment( button );
				break;

			case MessageDialog::Yes :
				button = new QPushButton( QIcon( ":/icons/ok.png" ), "&Yes" );
				button->setObjectName( tr( "okBtn" ) );
				connect( button, SIGNAL( clicked() ), this, SLOT( buttonClickHandler() ) );
				bSet->addSegment( button );
				break;

			case MessageDialog::YesToAll :
				button = new QPushButton( QIcon( ":/icons/ok.png" ), "Yes to &All" );
				button->setObjectName( tr( "okBtn" ) );
				connect( button, SIGNAL( clicked() ), this, SLOT( buttonClickHandler() ) );
				bSet->addSegment( button );
				break;

			case MessageDialog::No :
				button = new QPushButton( QIcon( ":/icons/delete2.png" ), "&No" );
				button->setObjectName( tr( "cancelBtn" ) );
				connect( button, SIGNAL( clicked() ), this, SLOT( buttonClickHandler() ) );
				bSet->addSegment( button );
				break;

			case MessageDialog::NoToAll :
				button = new QPushButton( QIcon( ":/icons/delete2.png" ), "N&o to All" );
				button->setObjectName( tr( "cancelBtn" ) );
				connect( button, SIGNAL( clicked() ), this, SLOT( buttonClickHandler() ) );
				bSet->addSegment( button );
				break;

			case MessageDialog::Abort :
				button = new QPushButton( QIcon( ":/icons/abort.png" ), "A&bort" );
				button->setObjectName( tr( "abortBtn" ) );
				connect( button, SIGNAL( clicked() ), this, SLOT( buttonClickHandler() ) );
				bSet->addSegment( button );
				break;

			case MessageDialog::Retry :
				button = new QPushButton( QIcon( ":/icons/refresh.png" ), "&Retry" );
				connect( button, SIGNAL( clicked() ), this, SLOT( buttonClickHandler() ) );
				bSet->addSegment( button );
				break;

			case MessageDialog::Ignore :
				button = new QPushButton( QIcon( ":/icons/ok.png" ), "&Ignore" );
				connect( button, SIGNAL( clicked() ), this, SLOT( buttonClickHandler() ) );
				bSet->addSegment( button );
				break;

			case MessageDialog::Cancel :
				button = new QPushButton( QIcon::fromTheme( "dialog-cancel" ), "&Cancel" );
				button->setObjectName( tr( "cancelBtn" ) );
				connect( button, SIGNAL( clicked() ), this, SLOT( buttonClickHandler() ) );
				bSet->addSegment( button );
				break;

			case MessageDialog::Save :
				button = new QPushButton( QIcon::fromTheme( "document-save" ), "&Save" );
				button->setObjectName( tr( "okBtn" ) );
				connect( button, SIGNAL( clicked() ), this, SLOT( buttonClickHandler() ) );
				bSet->addSegment( button );
				break;

			case MessageDialog::Discard :
				button = new QPushButton( QIcon::fromTheme( "document-close" ), "&Discard" );
				button->setObjectName( tr( "abortBtn" ) );
				connect( button, SIGNAL( clicked() ), this, SLOT( buttonClickHandler() ) );
				bSet->addSegment( button );
				break;

			default:
				qDebug() << "No Button added!!";
				break;
		}
	}

	bSet->setSegmentWidth( 100 );
	bSet->setSegmentHeight( 24 );
	btnLyt->addWidget( bSet );

	dlgLyt->addLayout( msgLyt );
	if ( info )
		dlgLyt->addWidget( infoWidget );

	dlgLyt->addWidget( Separator::horizontal() );
	dlgLyt->addLayout( btnLyt );

	Dialog::setLayout( dlgLyt );
};

void MessageDialog::setIcon( QString icon ) {

	iconLbl->setPixmap( QPixmap( icon ).scaled( 64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation ) );
};

void MessageDialog::setTitle( QString title ) {

	Dialog::setDialogTitle( title );
};

void MessageDialog::setText( QString text ) {

	textLbl->setText( text );
};

void MessageDialog::keyPressEvent( QKeyEvent *keyEvent ) {

	if ( keyEvent->key() == Qt::Key_Escape ) {
		retValue = MessageDialog::Cancel;
		close();
	}

	Dialog::keyPressEvent( keyEvent );
};

void MessageDialog::close() {

	Dialog::close();
	emit closed();
};

void MessageDialog::toggleInfoWidget() {

	if ( infoWidget->isVisible() ) {
		detailsBtn->setText( "&More..." );
		infoWidget->hide();

		restoreGeometry( origGeometry );
	}

	else {
		origGeometry = saveGeometry();

		detailsBtn->setText( "&Less..." );
		infoWidget->show();

		adjustSize();
	}
};

void MessageDialog::buttonClickHandler() {

	QString text = qobject_cast<QPushButton*>( sender() )->text();

	if ( text == QString( "&Ok" ) ) {
		retValue = MessageDialog::Ok;
		close();
	}

	else if ( text == QString( "&Yes" ) ) {
		retValue = MessageDialog::Yes;
		close();
	}

	else if ( text == QString( "Yes to &All" ) ) {
		retValue = MessageDialog::YesToAll;
		close();
	}

	else if ( text == QString( "&No" ) ) {
		retValue = MessageDialog::No;
		close();
	}

	else if ( text == QString( "N&o to All" ) ) {
		retValue = MessageDialog::NoToAll;
		close();
	}

	else if ( text == QString( "A&bort" ) ) {
		retValue = MessageDialog::Abort;
		close();
	}

	else if ( text == QString( "&Retry" ) ) {
		retValue = MessageDialog::Retry;
		close();
	}

	else if ( text == QString( "&Ignore" ) ) {
		retValue = MessageDialog::Ignore;
		close();
	}

	else if ( text == QString( "&Cancel" ) ) {
		retValue = MessageDialog::Cancel;
		close();
	}

	else if ( text == QString( "&Save" ) ) {
		retValue = MessageDialog::Save;
		close();
	}

	else if ( text == QString( "&Discard" ) ) {
		retValue = MessageDialog::Discard;
		close();
	}
};

void MessageDialog::aboutNewBreeze() {

	QString title = QString( "About NewBreeze v2.5.0" );
	QString text = QString(
		"<p><b><center>Fast and light-weight File Manager</center></b></p>"		\
		"<p>Built on top of the Powerful Qt4 GUI Toolkit, this file manager "	\
		"is sleek, stylish and fast. Some of its features are</p>"				\
		"<ul>"																	\
		"	<li>Fast startup and browsing</li>"									\
		"	<li>Folder, text, html, image and odt quick preview</li>"			\
		"	<li>File Association support</li>"									\
		"	<li>Multiple Views - Tiles, Icons and Details</li>"					\
		"	<li>Sleek side panel showing drives and boomarks</li>"				\
		"	<li>Full drag and drop support</li>"								\
		"	<li>Applications and Catalog View</li>"								\
		"   <li>Custom Folder Icons and Thumbnail Preview</li>"					\
		"   <li>Mime-Icon Support</li>"											\
		"   <li>BreadCrumbs Navigation</li>"									\
		"   <li>Custom Actions</li>"											\
		"   <li>Inline terminal</li>"											\
		"</ul>"																	\
		"<p><small>The credit for the inline terminal goes to "					\
		"Christian Surlykke, the author of QTermWidget. "						\
		"I have used the QTermWidget to dispaly the "							\
		"inline terminal.</small></p>"											\
	);
	QList<StandardButton> btns = QList<StandardButton>() << MessageDialog::Ok;

	MessageDialog *dlg = new MessageDialog( 0, ":/icons/newbreeze2.png", title, text, btns );
	dlg->setFixedSize( 540, 390 );
	dlg->exec();
};

void MessageDialog::aboutQt() {

	QString title = QObject::tr(
		"About Qt %1"
	).arg( QLatin1String( QT_VERSION_STR ) );

	QString text  = QObject::tr(
		"<p>Qt is a C++ toolkit for cross-platform application development.</p>"

		"<p>Qt provides single-source portability across MS&nbsp;Windows, Mac&nbsp;OS&nbsp;X, Linux, and all major commercial Unix"
		"variants. Qt is also available for embedded devices as Qt for Embedded Linux and Qt for Windows CE.</p>"

		"<p>Qt is available under three different licensing options designed to accommodate the needs of our various users.</p>"

		"<p>Qt licensed under our commercial license agreement is appropriate for development of proprietary/commercial software where you do "
		"not want to share any source code with third parties or otherwise cannot comply with the terms of the GNU LGPL version 2.1 or GNU "
		"GPL version 3.0.</p>"

		"<p>Qt licensed under the GNU LGPL version 2.1 is appropriate for the development of Qt applications (proprietary or open source) "
		"provided you can comply with the terms and conditions of the GNU LGPL version 2.1.</p>"

		"<p>Qt licensed under the GNU General Public License version 3.0 is appropriate for the development of Qt applications where you wish "
		"to use such applications in combination with software subject to the terms of the GNU GPL version 3.0 or where you are otherwise "
		"willing to comply with the terms of the GNU GPL version 3.0.</p>"

		"<p>Please see <a href='http://qt.digia.com/product/licensing'>qt.digia.com/product/licensing</a> for an overview of Qt licensing.</p>"

		"<p>Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).</p>"

		"<p>Qt is a Digia product. See <a href='http://qt.digia.com/'>qt.digia.com</a> for more information.</p>"
	);

	QList<StandardButton> btns = QList<StandardButton>() << MessageDialog::Ok;

	MessageDialog *dlg = new MessageDialog( NULL, ":/icons/QtLogo.png", title, text, btns );
	dlg->setMinimumSize( 630, 450 );
	dlg->exec();
};

int MessageDialog::information( QWidget* parent, QString title, QString text, QList<StandardButton> btns, QWidget *infoWidget ) {

	MessageDialog *dlg = new MessageDialog( parent, ":/icons/info.png", title, text, btns, infoWidget );
	dlg->exec();

	return dlg->retValue;
};

int MessageDialog::question( QWidget* parent, QString title, QString text, QList<StandardButton> btns, QWidget *infoWidget ) {

	MessageDialog *dlg = new MessageDialog( parent, ":/icons/question.png", title, text, btns, infoWidget );
	dlg->exec();

	return dlg->retValue;
};

int MessageDialog::warning( QWidget* parent, QString title, QString text, QList<StandardButton> btns, QWidget *infoWidget ) {

	MessageDialog *dlg = new MessageDialog( parent, ":/icons/warning.png", title, text, btns, infoWidget );
	dlg->exec();

	return dlg->retValue;
};

int MessageDialog::error( QWidget* parent, QString title, QString text, QList<StandardButton> btns, QWidget *infoWidget ) {

	MessageDialog *dlg = new MessageDialog( parent, ":/icons/error.png", title, text, btns, infoWidget );
	dlg->exec();

	return dlg->retValue;
};

int MessageDialog::critical( QWidget* parent, QString title, QString text, QList<StandardButton> btns, QWidget *infoWidget ) {

	MessageDialog *dlg = new MessageDialog( parent, ":/icons/stop.png", title, text, btns, infoWidget );
	dlg->exec();

	return dlg->retValue;
};
