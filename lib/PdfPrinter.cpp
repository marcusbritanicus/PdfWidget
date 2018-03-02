/*
	*
	* PdfPrinter.cpp - PDF printing class
	*
*/

#include <QPrinterInfo>
#include "PdfPrinter.hpp"

PdfPrinter::PdfPrinter( PdfDocument *pdfDoc, int curPg, QWidget *parent ) : QDialog( parent ) {

	mPdfDoc = pdfDoc;
	mCurrentPage = curPg;
	setWindowTitle( "Print PDF Document" );

	setupUI();
};

void PdfPrinter::setupUI() {

	/* Section 1 */
	QLabel *printersLbl = new QLabel( "P&rinter:" );
	printersCB = new QComboBox();
	Q_FOREACH( QPrinterInfo pInfo, QPrinterInfo::availablePrinters() )
		printersCB->addItem( pInfo.printerName() );
	printersLbl->setBuddy( printersCB );

	/* Section 2: Left Column */
	pageRangeAllRB = new QRadioButton( "&All Pages" );
	pageRangeAllRB->setChecked( true );
	pageRangeCustomRB = new QRadioButton( "Pa&ges:" );
	pageRangeCurrentRB = new QRadioButton( "C&urrent page" );
	pageCustomLE = new QLineEdit();
	pageCustomLE->setPlaceholderText( "For ex: 2,5-9,18" );

	QGroupBox *pageRangeGB = new QGroupBox( "Print Range" );
	QGridLayout *rangeLyt = new QGridLayout();
	rangeLyt->addWidget( pageRangeAllRB, 0, 0, 1, 2 );
	rangeLyt->addWidget( pageRangeCustomRB, 1, 0 );
	rangeLyt->addWidget( pageCustomLE, 1, 1 );
	rangeLyt->addWidget( pageRangeCurrentRB, 2, 0, 1, 2 );
	pageRangeGB->setLayout( rangeLyt );

	/* Section 2: Right Column */
	singleRB = new QRadioButton( "&Single Side" );
	singleRB->setChecked( true );
	doubleRB = new QRadioButton( "&Back to back" );

	QGroupBox *duplexGB = new QGroupBox( "Duplex Printing" );
	QVBoxLayout *duplexLyt = new QVBoxLayout();
	duplexLyt->addWidget( singleRB );
	duplexLyt->addWidget( doubleRB );
	duplexLyt->addWidget( new QLabel( "  " ) );
	duplexGB->setLayout( duplexLyt );

	/* Section 3: Orientation */
	potraitRB = new QRadioButton( "P&otrait" );
	potraitRB->setChecked( true );
	landscapeRB = new QRadioButton( "&Landscape" );

	QGroupBox *orientGB = new QGroupBox( "Page Orientation" );
	QHBoxLayout *orientLyt = new QHBoxLayout();
	orientLyt->addWidget( potraitRB );
	orientLyt->addWidget( landscapeRB );
	orientGB->setLayout( orientLyt );

	/* Section 4: Copies */
	QLabel *copiesLbl = new QLabel( "&Number of copies:" );
	copiesSB = new QSpinBox();
	copiesSB->setRange( 1, 100 );
	copiesLbl->setBuddy( copiesSB );

	QGroupBox *outputGB = new QGroupBox( "Output Settings" );
	QHBoxLayout *outputLyt = new QHBoxLayout();
	outputLyt->addWidget( copiesLbl );
	outputLyt->addWidget( copiesSB );
	outputGB->setLayout( outputLyt );

	/* Section 5: Buttons */
	cancelBtn = new QPushButton( QIcon::fromTheme( "dialog-cancel" ), "&Cancel" );
	connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( reject() ) );
	printBtn = new QPushButton( QIcon::fromTheme( "document-print" ), "&Print" );
	connect( printBtn, SIGNAL( clicked() ), this, SLOT( print() ) );

	QHBoxLayout *btnLyt = new QHBoxLayout();
	btnLyt->addWidget( cancelBtn );
	btnLyt->addStretch();
	btnLyt->addWidget( printBtn );

	QHBoxLayout *sec1Lyt = new QHBoxLayout();
	sec1Lyt->addWidget( printersLbl );
	sec1Lyt->addWidget( printersCB );
	sec1Lyt->addStretch();

	QHBoxLayout *sec2Lyt = new QHBoxLayout();
	sec2Lyt->addWidget( pageRangeGB );
	sec2Lyt->addWidget( duplexGB );

	QHBoxLayout *sec3Lyt = new QHBoxLayout();
	sec3Lyt->addWidget( orientGB );

	QHBoxLayout *sec4Lyt = new QHBoxLayout();
	sec4Lyt->addWidget( outputGB );

	QHBoxLayout *sec5Lyt = new QHBoxLayout();
	sec5Lyt->addWidget( cancelBtn );
	sec5Lyt->addStretch();
	sec5Lyt->addWidget( printBtn );

	QVBoxLayout *baseLyt = new QVBoxLayout();
	baseLyt->addLayout( sec1Lyt );
	baseLyt->addLayout( sec2Lyt );
	baseLyt->addLayout( sec3Lyt );
	baseLyt->addLayout( sec4Lyt );
	baseLyt->addLayout( sec5Lyt );

	setLayout( baseLyt );
};

void PdfPrinter::print() {

	/* Printer and A4 paper */
	QStringList lprOpts;
	lprOpts << "-P" << QString( "\"%1\"" ).arg( printersCB->currentText() );
	lprOpts << "-h"; 																		// No banners
	lprOpts << "-o" << "media=a4";
	lprOpts << "-o" << "fit-to-page";

	/* Page Range */
	if ( pageRangeCustomRB->isChecked() )
		lprOpts << "-o" << QString( "page-ranges=\"%1\"" ).arg( pageCustomLE->text() );

	else if ( pageRangeCurrentRB->isChecked() )
		lprOpts << "-o" << QString( "page-ranges=\"%1\"" ).arg( mCurrentPage );

	// else /* By default all pages are printed */
		// lprOpts << /* Nothing to be added here */

	/* Orientation and duplexing: Potrait */
	if ( potraitRB->isChecked() ) {
		lprOpts << "-o" << "potrait";
		if ( doubleRB->isChecked() )
			lprOpts << "-o" << "sides=two-sided-long-edge";
	}

	/* Orientation and duplexing: Landscape */
	else {
		lprOpts << "-o" << "landscape";
		if ( doubleRB->isChecked() )
			lprOpts << "-o" << "sides=two-sided-short-edge";
	}

	/* Number of copies */
	if ( copiesSB->value() > 1 ) {
		lprOpts << "-#" << QString::number( copiesSB->value() );
		lprOpts << "-o" << "collate=true";
	}

	lprOpts << mPdfDoc->name();

	printf( "lpr " );
	Q_FOREACH( QString opt, lprOpts )
		printf( "%s ", opt.toLocal8Bit().data() );

	printf( "\n" );

	QProcess::startDetached( "lpr", lprOpts );

	accept();
};
