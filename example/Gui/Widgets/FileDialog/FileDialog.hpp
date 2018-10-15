/*
	*
	* FileDialog.hpp - MiniPDF's FileDialog class header
	*
*/

#pragma once

#include "Global.hpp"

class FileDialog : public QFileDialog {
	Q_OBJECT

	public:
		static QStringList getOpenFileNames( QWidget *parent, QString location );
		static QString getSaveFileName( QWidget *parent, QString location );
};
