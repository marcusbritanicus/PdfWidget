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
		enum ViewMode {
			SinglePageView = 0x54FC79,
			DoublePageView,
			FitPageToWidth,
			FitPageToHeight,
			BookView
		};

		enum LayoutMode {
			SinglePage = 0x1D83E6,
			Continuous
		};

		PdfView( QWidget *parent );

		void setPdfDocument( PdfDocument *Pdf );

		int viewMode() const {

			return mViewMode;
		};

		void setViewMode( int viewMode ) {

			mViewMode = viewMode;

			reshapeView();
			getCurrentPage();
		};

		int layoutMode() const {

			return mLytMode;
		};

		void setLayoutMode( int lytMode ) {

			mLytMode = lytMode;

			reshapeView();
			getCurrentPage();
		};

		qreal zoom();
		void setZoom( qreal );

	private:
		PdfDocument *PdfDoc;
		QHash<int, QImage> renderedImages;
		QHash<int, QRectF> pageRects;

		int currentPage;
		qreal mZoom;

		int mViewMode;
		int mLytMode;

		void getCurrentPage();

		void reshapeView();

		bool isPageVisible( int pgNo );

	private Q_SLOTS:
		void slotZoomIn() {

			/* Maximum default zoom 400% */
			if ( mZoom >= 4.0 )
				return;

			/* Book view has fixed zoom */
			if ( mViewMode == BookView )
				return;

			if ( mViewMode != DoublePageView )
				mViewMode = SinglePageView;

			setZoom( mZoom + 0.1 );
		};

		void slotZoomOut() {

			/* Minimum default zoom 10% */
			if ( mZoom <= 0.1 )
				return;

			/* Book view has fixed zoom */
			if ( mViewMode == BookView )
				return;

			if ( mViewMode != DoublePageView )
				mViewMode = SinglePageView;

			setZoom( mZoom - 0.1 );
		};

	protected:
		void paintEvent( QPaintEvent *pEvent );

		void resizeEvent( QResizeEvent *rEvent );
		void wheelEvent( QWheelEvent *wEvent );

		void keyPressEvent( QKeyEvent *kEvent );
};
