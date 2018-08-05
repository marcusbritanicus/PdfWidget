/*
	*
	* PopplerDocument - PopplerDocument class header
	*
*/

#pragma once

#include <QtCore>
#include <QtGui>

#if QT_VERSION >= 0x050000
	#include <poppler-qt5.h>
	#include <QtWidgets>
#else
	#include <poppler-qt4.h>
#endif

typedef Poppler::Page PdfPage;
typedef QList<Poppler::Page*> PdfPages;

class PopplerDocument : public QObject {
	Q_OBJECT

	public:
		PopplerDocument( QString pdfPath );

		/* Poppler Document */
		Poppler::Document* document();

		/* Poppler Page */
		PdfPage *page( int pageNum ) const;

		/* Poppler Pages */
		PdfPages allPages();

		/* Check if a password is needed */
		bool passwordNeeded();

		/* Set a password */
		void setPassword( QString password ) {
			mPass = QString( password );
			loadPdf();
		};

		/* Pdf File Name and File Path */
		QString pdfName() const;
		QString pdfPath() const;

		/* Number of pages */
		int pageCount() const;

		/* Size of the page */
		QSizeF pageSize( int pageNo ) const;

		/* Render and return a page */
		QImage renderPage( int );

		/* Page Text */
		QString pageText( int pageNo ) const;

		/* Text of a Selection rectangle */
		QString text( int pageNo, QRectF ) const;

		qreal zoomForWidth( int pageNo, qreal width );
		qreal zoomForHeight( int pageNo, qreal width );

		void setZoom( qreal zoom ) {

			mZoom = zoom;
		};

		bool isReady() const {

			return mLoaded;
		}

	private:
		Poppler::Document *mPdfDoc;

		QString mPdfPath;
		PdfPages mPages;

		QString mPass;

		qreal mZoom;

		bool mLoaded;
		bool mPassNeeded;

		void loadPdf();

	Q_SIGNALS:
		void pdfLoaded();
		void loadFailed();
};
