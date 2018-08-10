/*
	*
	* PdfView - Pdf Document Viewer for PdfWidget
	*
*/

#include "PdfView.hpp"

PdfView::PdfView( QWidget *parent ) : QScrollArea( parent ) {

	setWidgetResizable( true );
	setAlignment( Qt::AlignCenter );
	setWidget( new QWidget() );

	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );

	setMinimumSize( QSize( 640, 480 ) );

	PdfDoc = 0;
	currentPage = -1;
	mZoom = 1.0;

	mViewMode = SinglePageView;
	mLytMode = Continuous;

	QAction *zoomInAct = new QAction( this );
	zoomInAct->setShortcut( QKeySequence( "Ctrl++" ) );
	connect( zoomInAct, SIGNAL( triggered() ), this, SLOT( slotZoomIn() ) );
	addAction( zoomInAct );

	QAction *zoomOutAct = new QAction( this );
	zoomOutAct->setShortcut( QKeySequence( "Ctrl+-" ) );
	connect( zoomOutAct, SIGNAL( triggered() ), this, SLOT( slotZoomOut() ) );
	addAction( zoomOutAct );
};

void PdfView::setPdfDocument( PdfDocument *Pdf ) {

	PdfDoc = Pdf;
	if ( Pdf->pageCount() > 0 )
		currentPage = 0;

	setWindowTitle( "PdfView - " + PdfDoc->pdfName() );

	reshapeView();
	getCurrentPage();
};

qreal PdfView::zoom() {

	return mZoom;
};

void PdfView::setZoom( qreal zoom ) {

	mZoom = zoom;
	PdfDoc->setZoom( zoom );

	renderedImages.clear();
	pageRects.clear();

	reshapeView();
	getCurrentPage();

	repaint();
};

void PdfView::getCurrentPage() {

	if ( not PdfDoc or not PdfDoc->isReady() )
		return;

	/* Fetch and store the rendering of the current page */
	if ( PdfDoc->pageCount() <= 0 )
		return;

	QRectF viewRect = QRectF( QPointF( 0, verticalScrollBar()->value() ), viewport()->size() );

	if ( mLytMode == Continuous ) {
		currentPage = 0;

		/* Get the current page: maximum area => current page */
		qreal area = 0;
		for( int pg = 0; pg < PdfDoc->pageCount(); pg++ ) {
			QRectF xRect = pageRects[ pg ].intersected( viewRect );
			qreal newArea = abs( xRect.width() *  xRect.height() );
			if ( newArea > area ) {
				currentPage = pg;
				area = newArea;
			}
		}

		/* Render the images and store them */
		switch( mViewMode ) {
			case FitPageToWidth: {
				qreal width = viewport()->width() - style()->pixelMetric( QStyle::PM_ScrollBarExtent ) - 25;

				/* Render the current page if not rendered already */
				if ( renderedImages.value( currentPage ).isNull() )
					renderedImages[ currentPage ] = PdfDoc->renderPageForWidth( currentPage, width );

				/* Render the next page if not rendered */
				if ( currentPage + 1 < PdfDoc->pageCount() ) {
					if ( renderedImages.value( currentPage + 1 ).isNull() )
						renderedImages[ currentPage + 1 ] = PdfDoc->renderPageForWidth( currentPage + 1, width );
				}

				/* Render the previous page if not rendered */
				if ( currentPage - 1 >= 0 ) {
					if ( renderedImages.value( currentPage - 1 ).isNull() )
						renderedImages[ currentPage - 1 ] = PdfDoc->renderPageForWidth( currentPage - 1, width );
				}

				/* Render all other pages which are visible in the viewport */
				for( int i = 0; i < PdfDoc->pageCount(); i++ ) {
					if ( isPageVisible( i ) and renderedImages.value( i ).isNull() )
						renderedImages[ i ] = PdfDoc->renderPageForWidth( i, width );
				}

				break;
			}

			case FitPageToHeight: {
				qreal height = viewport()->height() - style()->pixelMetric( QStyle::PM_ScrollBarExtent ) - 25;

				/* Render the current page if not rendered already */
				if ( renderedImages.value( currentPage ).isNull() )
					renderedImages[ currentPage ] = PdfDoc->renderPageForHeight( currentPage, height );

				/* Render the next page if not rendered */
				if ( currentPage + 1 < PdfDoc->pageCount() ) {
					if ( renderedImages.value( currentPage + 1 ).isNull() )
						renderedImages[ currentPage + 1 ] = PdfDoc->renderPageForHeight( currentPage + 1, height );
				}

				/* Render the previous page if not rendered */
				if ( currentPage - 1 >= 0 ) {
					if ( renderedImages.value( currentPage - 1 ).isNull() )
						renderedImages[ currentPage - 1 ] = PdfDoc->renderPageForHeight( currentPage - 1, height );
				}

				/* Render all other pages which are visible in the viewport */
				for( int i = 0; i < PdfDoc->pageCount(); i++ ) {
					if ( isPageVisible( i ) and renderedImages.value( i ).isNull() )
						renderedImages[ i ] = PdfDoc->renderPageForHeight( i, height );
				}

				break;
			}

			default: {
				/* Render the current page if not rendered already */
				if ( renderedImages.value( currentPage ).isNull() )
					renderedImages[ currentPage ] = PdfDoc->renderPage( currentPage );

				/* Render the next page if not rendered */
				if ( currentPage + 1 < PdfDoc->pageCount() ) {
					if ( renderedImages.value( currentPage + 1 ).isNull() )
						renderedImages[ currentPage + 1 ] = PdfDoc->renderPage( currentPage + 1 );
				}

				/* Render the previous page if not rendered */
				if ( currentPage - 1 >= 0 ) {
					if ( renderedImages.value( currentPage - 1 ).isNull() )
						renderedImages[ currentPage - 1 ] = PdfDoc->renderPage( currentPage - 1 );
				}

				/* Render all other pages which are visible in the viewport */
				for( int i = 0; i < PdfDoc->pageCount(); i++ ) {
					if ( isPageVisible( i ) and renderedImages.value( i ).isNull() )
						renderedImages[ i ] = PdfDoc->renderPage( i );
				}

				break;
			}
		}
	}

	else {
		switch( mViewMode ) {
			case FitPageToWidth: {
				qreal width = viewport()->width() - style()->pixelMetric( QStyle::PM_ScrollBarExtent ) - 25;

				/* Render the current page if not rendered already */
				if ( renderedImages.value( currentPage ).isNull() )
					renderedImages[ currentPage ] = PdfDoc->renderPageForWidth( currentPage, width );

				/* Render the next page if not rendered */
				if ( currentPage + 1 < PdfDoc->pageCount() ) {
					if ( renderedImages.value( currentPage + 1 ).isNull() )
						renderedImages[ currentPage + 1 ] = PdfDoc->renderPageForWidth( currentPage + 1, width );
				}

				/* Render the previous page if not rendered */
				if ( currentPage - 1 >= 0 ) {
					if ( renderedImages.value( currentPage - 1 ).isNull() )
						renderedImages[ currentPage - 1 ] = PdfDoc->renderPageForWidth( currentPage - 1, width );
				}

				break;
			}

			case FitPageToHeight: {
				qreal height = viewport()->height() - style()->pixelMetric( QStyle::PM_ScrollBarExtent ) - 25;

				/* Render the current page if not rendered already */
				if ( renderedImages.value( currentPage ).isNull() )
					renderedImages[ currentPage ] = PdfDoc->renderPageForHeight( currentPage, height );

				/* Render the next page if not rendered */
				if ( currentPage + 1 < PdfDoc->pageCount() ) {
					if ( renderedImages.value( currentPage + 1 ).isNull() )
						renderedImages[ currentPage + 1 ] = PdfDoc->renderPageForHeight( currentPage + 1, height );
				}

				/* Render the previous page if not rendered */
				if ( currentPage - 1 >= 0 ) {
					if ( renderedImages.value( currentPage - 1 ).isNull() )
						renderedImages[ currentPage - 1 ] = PdfDoc->renderPageForHeight( currentPage - 1, height );
				}

				break;
			}

			default: {
				/* Render the current page if not rendered already */
				if ( renderedImages.value( currentPage ).isNull() )
					renderedImages[ currentPage ] = PdfDoc->renderPage( currentPage );

				/* Render the next page if not rendered */
				if ( currentPage + 1 < PdfDoc->pageCount() ) {
					if ( renderedImages.value( currentPage + 1 ).isNull() )
						renderedImages[ currentPage + 1 ] = PdfDoc->renderPage( currentPage + 1 );
				}

				/* Render the previous page if not rendered */
				if ( currentPage - 1 >= 0 ) {
					if ( renderedImages.value( currentPage - 1 ).isNull() )
						renderedImages[ currentPage - 1 ] = PdfDoc->renderPage( currentPage - 1 );
				}

				break;
			}
		}
	}
};

void PdfView::reshapeView() {

	if ( not PdfDoc or not PdfDoc->isReady() )
		return;

	pageRects.clear();
	renderedImages.clear();

	int minHeight = 5;
	int maxWidth = 0;

	verticalScrollBar()->setPageStep( height() / 4 * 3 );
	verticalScrollBar()->setSingleStep( 30 );

	if ( mLytMode == Continuous ) {
		switch( mViewMode ) {
			case SinglePageView: {

				for( int i = 0; i < PdfDoc->pageCount(); i++ ) {
					QSizeF pageSize = PdfDoc->pageSize( i );
					pageRects[ i ] = QRectF( QPointF( 0, minHeight ), pageSize );

					maxWidth = ( pageSize.width() > maxWidth ? pageSize.width() : maxWidth );

					minHeight += ( int )( pageSize.height() );
					minHeight += 5;
				}

				break;
			}

			case DoublePageView: {

				for( int i = 0; i < PdfDoc->pageCount(); i += 2 ) {

					qreal w = 0, h = 0;

					/* Size of first page */
					QSizeF pageSize = PdfDoc->pageSize( i );
					pageRects[ i ] = QRectF( QPointF( 0, minHeight ), pageSize );
					w += pageSize.width();
					h = pageSize.height();

					if ( ( i + 1 ) < PdfDoc->pageCount() ) {
						/* Size of second page */
						pageSize = PdfDoc->pageSize( i + 1 );
						pageRects[ i + 1 ] = QRectF( QPointF( pageRects[ i ].width() + 5, minHeight ), pageSize );

						w += pageSize.width() + 5;
						h = ( pageSize.height() > h ? pageSize.height() : h );
					}

					maxWidth = ( w > maxWidth ? w : maxWidth );

					minHeight += ( int )( h );
					minHeight += 5;
				}

				break;
			}

			case FitPageToWidth: {

				qreal width = viewport()->width() - style()->pixelMetric( QStyle::PM_ScrollBarExtent ) - 25;

				for( int i = 0; i < PdfDoc->pageCount(); i++ ) {
					QSizeF pageSize = PdfDoc->pageSize( i );

					qreal height = pageSize.height() * width / pageSize.width();
					pageRects[ i ] = QRectF( QPointF( 0, minHeight ), QSizeF( width, height ) );

					minHeight += ( int )( height );
					minHeight += 5;
				}

				maxWidth = width;

				break;
			}

			case FitPageToHeight: {

				qreal height = viewport()->height() - 10;

				for( int i = 0; i < PdfDoc->pageCount(); i++ ) {
					QSizeF pageSize = PdfDoc->pageSize( i );

					qreal width = pageSize.width() * height / pageSize.height();
					pageRects[ i ] = QRectF( QPointF( 0, minHeight ), QSizeF( width, height ) );

					maxWidth = ( width > maxWidth ? width : maxWidth );

					minHeight += ( int )( height );
					minHeight += 5;
				}

				break;
			}

			case BookView:{

				/*
					* First we scan the maximum page width, and maximum page height.
					* We shall use this value to render all the pages. All pages
					* will have uniform width and height.
				*/

				break;
			}
		}
	}

	else {
		switch( mViewMode ) {
			case SinglePageView: {

				QSizeF pageSize = PdfDoc->pageSize( currentPage );
				pageRects[ currentPage ] = QRectF( QPointF( 0, 5 ), pageSize );

				minHeight = pageSize.height() + 10;
				maxWidth = pageSize.width();

				break;
			}

			case FitPageToWidth: {

				QSizeF pageSize = PdfDoc->pageSize( currentPage );

				qreal width = viewport()->width() - style()->pixelMetric( QStyle::PM_ScrollBarExtent ) - 25;
				qreal height = pageSize.height() * width / pageSize.width();

				pageRects[ currentPage ] = QRectF( QPointF( 0, 5 ), QSizeF( width, height ) );

				minHeight = height + 10;
				maxWidth = width;

				break;
			}

			case FitPageToHeight: {


				QSizeF pageSize = PdfDoc->pageSize( currentPage );

				qreal height = viewport()->height() - 10;
				qreal width = pageSize.width() * height / pageSize.height();
				pageRects[ currentPage ] = QRectF( QPointF( 0, 5 ), QSizeF( width, height ) );

				maxWidth = width;
				minHeight = height;

				break;
			}

			case DoublePageView: {

				qreal w = 0, h = 0;

				/* Size of first page */
				QSizeF pageSize = PdfDoc->pageSize( currentPage );
				pageRects[ currentPage ] = QRectF( QPointF( 0, 5 ), pageSize );
				w += pageSize.width();
				h = pageSize.height();

				if ( ( currentPage + 1 ) < PdfDoc->pageCount() ) {
					/* Size of second page */
					pageSize = PdfDoc->pageSize( currentPage + 1 );
					pageRects[ currentPage + 1 ] = QRectF( QPointF( w + 5, 5 ), pageSize );

					w += pageSize.width() + 5;
					h = ( pageSize.height() > h ? pageSize.height() : h );
				}

				maxWidth = w;
				minHeight = h + 10;

				break;
			}

			case BookView:{

				/*
					* First we scan the maximum page width, and maximum page height.
					* We shall use this value to render all the pages. All pages
					* will have uniform width and height.
				*/

				break;
			}
		}
	}

	/* 20px border */
	widget()->setFixedSize( maxWidth + 20, minHeight );
	viewport()->update();

	if ( ( maxWidth + 20 ) < viewport()->width() )
		horizontalScrollBar()->hide();

	else
		horizontalScrollBar()->show();
};

bool PdfView::isPageVisible( int pgNo ) {

	QRectF pageRect = pageRects[ pgNo ];
	QRectF viewRect = QRectF( QPointF( 0, verticalScrollBar()->value() ), viewport()->size() );

	return viewRect.intersects( pageRect );
};

void PdfView::paintEvent( QPaintEvent *pEvent ) {

	/* If the document is not loaded, return */
	if ( currentPage == -1 )
		return;

	/* Init the painter */
	QPainter painter( viewport() );

	/* Draw the background */
	painter.fillRect( rect(), Qt::lightGray );

	/* Get the current page */
	getCurrentPage();

	/* ScrollBar Positions */
	int h = verticalScrollBar()->value();

	/* Start drawing the current page from where it starts */
	painter.translate( widget()->x() + 10, -h );
	painter.drawImage( pageRects[ currentPage ], renderedImages[ currentPage ] );

	if ( mLytMode == Continuous ) {
		for( int pg = 0; pg < PdfDoc->pageCount(); pg++ ) {
			if ( isPageVisible( pg ) )
				painter.drawImage( pageRects[ pg ], renderedImages[ pg ] );
		}

		/* Draw the current page rect */
		painter.setPen( Qt::black );
		if ( mViewMode != SinglePageView ) {
			painter.drawRect( pageRects[ currentPage ] );
			if ( currentPage + 1 < PdfDoc->pageCount() )
				painter.drawRect( pageRects[ currentPage + 1 ] );
		}

		else
			painter.drawRect( pageRects[ currentPage ] );
	}

	else {

		painter.drawRect( pageRects[ currentPage ] );
	}

	painter.end();

	pEvent->accept();
};

void PdfView::resizeEvent( QResizeEvent *rEvent ) {

	rEvent->accept();

	int width = rEvent->size().width() - style()->pixelMetric( QStyle::PM_ScrollBarExtent ) - 25;
	int height = rEvent->size().height() - style()->pixelMetric( QStyle::PM_ScrollBarExtent ) - 25;

	if ( rEvent->oldSize() != rEvent->size() ) {
		/* If the width has actually changed */
		if ( rEvent->oldSize().width() != rEvent->size().width() ) {
			/* Book View and Double Page View */
			switch( mViewMode ) {
				case SinglePageView: {
					setZoom( PdfDoc->zoomForWidth( 0, width ) );
					break;
				}

				case DoublePageView: {
					setZoom( PdfDoc->zoomForWidth( 0, ( width - 5.0 ) / 2 ) );
					break;
				}

				case BookView: {
					/* If the width changes, then we have nothing to do */
					break;
				}
			}
		}

		/* If the height has actually changed */
		if ( rEvent->oldSize().height() != rEvent->size().height() ) {
			/* Book View and Double Page View */
			switch( mViewMode ) {
				case SinglePageView:
				case DoublePageView: {
					/* If the height has changed, then we have nothing to do. */
					break;
				}

				case BookView: {
					setZoom( PdfDoc->zoomForHeight( 0, height / 2 ) );
					break;
				}
			}
		}

		/* We reshape the view only if there was a resize */
		reshapeView();
	}

	repaint();
};

void PdfView::wheelEvent( QWheelEvent *wEvent ) {

	QScrollArea::wheelEvent( wEvent );

	int cPos = verticalScrollBar()->value();

	int numDegrees = -wEvent->delta() / 8;
	verticalScrollBar()->setValue( cPos + numDegrees * verticalScrollBar()->singleStep() );

	wEvent->accept();

	viewport()->repaint();
};

void PdfView::keyPressEvent( QKeyEvent *kEvent ) {

	if ( mViewMode == DoublePageView ) {
		if ( kEvent->key() == Qt::Key_Right ) {
			if ( currentPage + 2 < PdfDoc->pageCount() )
				verticalScrollBar()->setValue( pageRects.value( currentPage + 1 ).top() - 5 );
		}

		else if ( kEvent->key() == Qt::Key_Left ) {
			if ( currentPage - 2 >= 0 )
				verticalScrollBar()->setValue( pageRects.value( currentPage - 1 ).top() - 5 );
		}
	}

	else {
		switch ( kEvent->key() ) {
			case Qt::Key_Right: {

				if ( currentPage + 1 < PdfDoc->pageCount() )
					verticalScrollBar()->setValue( pageRects.value( currentPage + 1 ).top() - 5 );

				if (mLytMode != Continuous ) {
					currentPage++;
					reshapeView();
				}

				break;
			};

			case Qt::Key_Left: {

				if ( currentPage - 1 >= 0 )
					verticalScrollBar()->setValue( pageRects.value( currentPage - 1 ).top() - 5 );

				if (mLytMode != Continuous ) {
					currentPage--;
					reshapeView();
				}

				break;
			};

			case Qt::Key_Home: {

				verticalScrollBar()->setValue( 0 );

				if (mLytMode != Continuous ) {
					currentPage = 0;
					reshapeView();
				}

				break;
			};

			case Qt::Key_End: {

				verticalScrollBar()->setValue( widget()->height() );

				if (mLytMode != Continuous ) {
					currentPage = PdfDoc->pageCount() - 1;
					reshapeView();
				}

				break;
			};

			default: {

				QScrollArea::keyPressEvent( kEvent );
				break;
			};
		}
	}

	repaint();
	kEvent->accept();
};
