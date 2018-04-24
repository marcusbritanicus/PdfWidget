/*
	*
	* PdfView - PdfView class header
	*
*/

#pragma once

#include <QtCore>
#include <QtGui>

#if QT_VERSION >= 0x050000
	#include <QtWidgets>
#endif

#include "PdfDocument.hpp"

class PdfView : public QScrollArea {
	Q_OBJECT

	public:
		PdfView( QWidget *parent );
		PdfView( QString, QWidget *parent );

		void setPdfDocument( PdfDocument *Pdf );
		void load( QString pdfPath );
		QString pageText( int );

		qreal zoom();
		void setZoom( qreal );

	private:
		PdfDocument *PdfDoc;
		QHash<int, QImage> renderedImages;
		QHash<int, QRect> pageRects;

		int currentPage;
		qreal mZoom;

		void basicInit();

		void getCurrentPage();
		void lookAround();

		void reshapeView();
		float getResolution( int );

		void paintPage( int );
		void paintRect( int );

	public Q_SLOTS:
		void slotZoomIn() {

			if ( mZoom >= 4.0 )
				return;

			setZoom( mZoom + 0.25 );
		};

		void slotZoomOut() {

			if ( mZoom <= 0.25 )
				return;

			setZoom( mZoom - 0.25 );
		};

	protected:
		void paintEvent( QPaintEvent *pEvent );

		void resizeEvent( QResizeEvent *rEvent );
		void wheelEvent( QWheelEvent *wEvent );
};

/*
class PageRenderer : public QThread {
	Q_OBJECT

	public:
		PageRenderer( QWidget *parent );

		void render( PdfPages pages );

	protected:
		void run();

	private:
		PdfPages pageList;
};
*/
