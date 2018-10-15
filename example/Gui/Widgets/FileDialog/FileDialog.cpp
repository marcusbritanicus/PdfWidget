/*
	*
	* FileDialog.hpp - MiniPDF's FileDialog class
	*
*/

#include "FileDialog.hpp"

QStringList FileDialog::getOpenFileNames( QWidget *parent, QString location ) {

	QFileDialog dlg( parent, QString( "MiniPDF - Open Files" ), location, QString( "All Files (*);;PDF Files (*.pdf)" ) );
	dlg.setWindowFlags( Qt::Dialog );

	dlg.setAcceptMode( QFileDialog::AcceptOpen );
	dlg.setFileMode( QFileDialog::ExistingFiles );
	dlg.setFixedSize( 800, 600 );
	dlg.setModal( true );

	if ( dlg.exec() == QDialog::Accepted )
		return dlg.selectedFiles();

	else
		return QStringList();
};

QString FileDialog::getSaveFileName( QWidget *parent, QString location ) {

	QFileDialog dlg( parent, QString( "MiniPDF - Save File" ), location, QString( "All Files (*);;PDF Files (*.pdf)" ) );
	dlg.setWindowFlags( Qt::Dialog );

	dlg.setAcceptMode( QFileDialog::AcceptSave );
	dlg.setFileMode( QFileDialog::AnyFile );
	dlg.setFixedSize( 800, 600 );
	dlg.setModal( true );

	if ( dlg.exec() == QDialog::Accepted )
		return dlg.selectedFiles().at( 0 );

	else
		return QString();
};
