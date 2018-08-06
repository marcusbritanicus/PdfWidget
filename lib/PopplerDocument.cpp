/*
	*
	* PopplerDocument - Pdf Document Model for PdfWidget
	*
*/

#include "PopplerDocument.hpp"

PopplerDocument::PopplerDocument( QString pdfPath ) : QObject() {

	mZoom = 1.0;
	mLoaded = false;
	mPassNeeded = false;

	mPdfPath = pdfPath;
	loadPdf();
};

Poppler::Document* PopplerDocument::document() {

	return mPdfDoc;
};

PdfPage* PopplerDocument::page( int pageNum ) const {

	return mPages.at( pageNum );
};

PdfPages PopplerDocument::allPages() {

	return mPages;
};

QString PopplerDocument::pdfName() const {

	return QFileInfo( mPdfPath ).baseName();
};

QString PopplerDocument::pdfPath() const {

	return QFileInfo( mPdfPath ).absolutePath();
};

int PopplerDocument::pageCount() const {

	return mPages.count();
};

QSizeF PopplerDocument::pageSize( int pageNo ) const {

	return page( pageNo )->pageSizeF() * mZoom;
};

QImage PopplerDocument::renderPage( int pageNo ) {

	return page( pageNo )->renderToImage( 72 * mZoom, 72 * mZoom );
};

QString PopplerDocument::pageText( int pageNo ) const {

	return mPages.at( pageNo )->text( QRectF() );
};

QString PopplerDocument::text( int pageNo, QRectF rect ) const {

	return mPages.at( pageNo )->text( rect );
};

qreal PopplerDocument::zoomForWidth( int pageNo, qreal width ) {

	if ( pageNo >= mPages.count() )
		return 0.0;

	return 1.0 * width / page( pageNo )->pageSizeF().width();
};

qreal PopplerDocument::zoomForHeight( int pageNo, qreal height ) {

	if ( pageNo >= mPages.count() )
		return 0.0;

	return 1.0 * height / page( pageNo )->pageSizeF().height();
};

void PopplerDocument::loadPdf() {

	mPdfDoc = Poppler::Document::load( mPdfPath, mPass.toLatin1() );

	if ( not mPdfDoc ) {
		emit loadFailed();
		return;
	}

	if ( mPdfDoc->isLocked() ) {
		mPassNeeded = true;
		emit loadFailed();
		return;
	}

	mPdfDoc->setRenderHint( Poppler::Document::Antialiasing );
	mPdfDoc->setRenderHint( Poppler::Document::TextAntialiasing );
	mPdfDoc->setRenderHint( Poppler::Document::TextHinting );

	for( int i = 0; i < mPdfDoc->numPages(); i++ ) {
		PdfPage *p = mPdfDoc->page( i );
		mPages.append( p );
	}

	emit pdfLoaded();
	mLoaded = true;
};
