/*
	*
	* PdfPrinter.hpp - PDF printing class header
	*
*/

#pragma once

#include <QtCore>
#include <QtGui>

#if QT_VERSION >= 0x050000
	#include <QtWidgets>
#endif

class PdfPrinter : public QDialog {
	Q_OBJECT

	public:
		PdfPrinter( QString path, int curPg = 1, QWidget *parent = NULL );

	private:
		void setupUI();

		QComboBox *printersCB;
		QRadioButton *pageRangeAllRB, *pageRangeCustomRB, *pageRangeCurrentRB;
		QLineEdit *pageCustomLE;
		QRadioButton *singleRB, *doubleRB;
		QRadioButton *potraitRB, *landscapeRB;
		QSpinBox *copiesSB;
		QPushButton *cancelBtn, *printBtn;

		QString mPdfDoc;
		int mCurrentPage;

	private Q_SLOTS:
		void print();
};
