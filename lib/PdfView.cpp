/*
	*
	* PdfView - Pdf Document Viewer for PdfWidget
	*
*/

#include "PdfView.hh"

PdfView::PdfView( QWidget *parent ) : QScrollArea( parent ) {

	basicInit();
};

PdfView::PdfView( QString pdfPath, QWidget *parent ) : QScrollArea( parent ) {

	basicInit();

	setPdfDocument( new PdfDocument( pdfPath ) );
};

void PdfView::basicInit() {

	setWidgetResizable( true );
	setAlignment( Qt::AlignCenter );
	setWidget( new QWidget() );

	setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOn );
	setHorizontalScrollBarPolicy( Qt::ScrollBarAsNeeded );

	setMinimumSize( QSize( 640, 480 ) );

	currentPage = -1;
	mZoom = 1.0;

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
	if ( Pdf->pages() > 0 )
		currentPage = 0;

	reshapeView();
	lookAround();
};

void PdfView::load( QString pdfPath ) {

	setPdfDocument( new PdfDocument( pdfPath ) );
};

qreal PdfView::zoom() {

	return mZoom;
};

void PdfView::setZoom( qreal zoom ) {

	mZoom = zoom;

	renderedImages.clear();
	pageRects.clear();

	reshapeView();
	lookAround();

	repaint();
};

QString PdfView::pageText( int pageNo ) {

	return PdfDoc->page( pageNo )->text( QRectF() );
};

void PdfView::reshapeView() {

	pageRects.clear();
	renderedImages.clear();

	int minHeight = 10;
	int viewWidth = viewport()->width() - verticalScrollBar()->width() - 10;			// 10px left size leeway

	verticalScrollBar()->setPageStep( height() / 4 * 3 );
	verticalScrollBar()->setSingleStep( 30 );

	for( int i = 0; i < PdfDoc->pages(); i++ ) {
		QSize pageSize = PdfDoc->page( i )->pageSize();
		pageRects[ i ] = QRect( 0, minHeight, viewWidth * mZoom, mZoom * viewWidth * pageSize.height() / pageSize.width() );

		minHeight += ( int )( mZoom * viewWidth * pageSize.height() / pageSize.width() );
		minHeight += 5;
	}

	widget()->setFixedSize( mZoom * viewport()->width(), minHeight );
	viewport()->update();
};

float PdfView::getResolution( int pageNo ) {

	int viewWidth = viewport()->width() - verticalScrollBar()->width();
	return viewWidth * 72 / PdfDoc->page( pageNo )->pageSize().width() * mZoom;
};

void PdfView::getCurrentPage() {

	/* Fetch and store the rendering of the current page */
	if ( PdfDoc->pages() <= 0 )
		return;

	int viewWidth = viewport()->width() - verticalScrollBar()->width();
	int h = verticalScrollBar()->value();

	/* Set the current page */
	for( int i = 0; i < PdfDoc->pages(); i++ ) {
		if ( pageRects[ i ].contains( QPoint( viewWidth / 2, h ) ) ) {
			currentPage = i;
			break;
		}
	}

	/* Render the current page if not rendered already */
	float pageRes = getResolution( currentPage );
	if ( not renderedImages.keys().contains( currentPage ) )
		renderedImages[ currentPage ] = PdfDoc->page( currentPage )->renderToImage( pageRes, pageRes );

	lookAround();
};

void PdfView::lookAround() {
	/* We will be rendering 5 pages before and after the current page */

	if ( currentPage == -1 )
		return;

	PdfPages renderList;
	int i = 1;
	while ( i <= 5 ) {
		if ( currentPage + i < PdfDoc->pages() ) {
			float pageRes = getResolution( currentPage + i );
			if ( not renderedImages.keys().contains( currentPage + i ) )
				renderedImages[ currentPage + i ] = PdfDoc->page( currentPage + i )->renderToImage( pageRes, pageRes );
		}

		if ( currentPage - i > 0 ) {
			float pageRes = getResolution( currentPage - i );
			if ( not renderedImages.keys().contains( currentPage - i ) )
				renderedImages[ currentPage - i ] = PdfDoc->page( currentPage - i )->renderToImage( pageRes, pageRes );
		}

		i++;
	}
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
	int w = horizontalScrollBar()->value();

	/* Start drawing the current page from where it starts */
	painter.translate( widget()->x() + 10, -h );
	painter.drawImage( pageRects[ currentPage ], renderedImages[ currentPage ] );

	/* If the  */
	if ( pageRects[ currentPage ].y() + height() < h + pageRects[ currentPage ].height() )
		painter.drawImage( pageRects[ currentPage + 1 ], renderedImages[ currentPage + 1 ] );

	/* Draw the current page rect */
	painter.setPen( Qt::black );
	if ( pageRects[ currentPage ].y() + pageRects[ currentPage ].height() - h < height() / 2 )
		painter.drawRect( pageRects[ currentPage + 1 ] );

	else
		painter.drawRect( pageRects[ currentPage ] );

	painter.end();

	pEvent->accept();
};

void PdfView::resizeEvent( QResizeEvent *rEvent ) {

	rEvent->accept();
	reshapeView();
};

void PdfView::wheelEvent( QWheelEvent *wEvent ) {

	QScrollArea::wheelEvent( wEvent );

	// int cPos = verticalScrollBar()->value();

	// int numDegrees = -wEvent->delta() / 8;
	// verticalScrollBar()->setValue( cPos + numDegrees * vScroll->singleStep() );

	// wEvent->accept();

	// viewport()->repaint();
};
