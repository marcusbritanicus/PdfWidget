    /*
    *
    * This file is a part of DesQDocs.
    * DesQDocs is the default document viewer for the DesQ Suite
    * Copyright 2019-2021 Britanicus <marcusbritanicus@gmail.com>
    *

    *
    * This program is free software; you can redistribute it and/or modify
    * it under the terms of the GNU General Public License as published by
    * the Free Software Foundation; either version 2 of the License, or
    * at your option, any later version.
    *

    *
    * This program is distributed in the hope that it will be useful,
    * but WITHOUT ANY WARRANTY; without even the implied warranty of
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    * GNU General Public License for more details.
    *

    *
    * You should have received a copy of the GNU General Public License
    * along with this program; if not, write to the Free Software
    * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    * MA 02110-1301, USA.
    *
*/

#include "View.hpp"
#include "ViewImpl.hpp"
#include "Renderer.hpp"
#include "Document.hpp"
#include "PopplerDocument.hpp"
#include "Navigation.hpp"
#include "ViewWidgets.hpp"

#include <QGuiApplication>
#include <QScreen>
#include <QScrollBar>
#include <QScroller>

DesQDocs::ViewPrivate::ViewPrivate(): QAbstractScrollAreaPrivate() {

    m_document = nullptr;
    m_pageNavigation = nullptr;
    m_pageRenderer = nullptr;
    m_pageMode = DesQDocs::View::SinglePage;
    m_zoomMode = DesQDocs::View::CustomZoom;
    m_zoomFactor = 1.0;
    m_pageSpacing = 3;
    m_documentMargins = QMargins( 6, 6, 6, 6 );
    m_blockPageScrolling = false;
    m_screenResolution = QGuiApplication::primaryScreen()->logicalDotsPerInch() / 72.0;
};

void DesQDocs::ViewPrivate::init() {

    Q_Q( View );

    m_pageNavigation = new DesQDocs::Navigation( q );
    m_pageRenderer = new DesQDocs::Renderer( q );
};

void DesQDocs::ViewPrivate::documentStatusChanged() {

    Q_Q( View );

    updateDocumentLayout();
    q->viewport()->update();
};

void DesQDocs::ViewPrivate::currentPageChanged( int currentPage ) {

    Q_Q( View );

    if ( m_blockPageScrolling )
        return;

    q->verticalScrollBar()->setValue( yPositionForPage( currentPage ) );

    if ( m_pageMode == DesQDocs::View::SinglePage )
        invalidateDocumentLayout();
};

void DesQDocs::ViewPrivate::calculateViewport() {

    Q_Q( View );

    if ( not m_document )
        return;

    const int x = q->horizontalScrollBar()->value();
    const int y = q->verticalScrollBar()->value();
    const int width = q->viewport()->width();
    const int height = q->viewport()->height();

    setViewport( QRect( x, y, width, height ) );
};

void DesQDocs::ViewPrivate::setViewport( QRect viewport ) {

    Q_Q( View );

    if ( m_viewport == viewport )
        return;

    const QSize oldSize = m_viewport.size();

    m_viewport = viewport;

    if ( oldSize != m_viewport.size() ) {
        updateDocumentLayout();

        if ( m_zoomMode != DesQDocs::View::CustomZoom ) {
            q->viewport()->update();
        }
    }

    if ( m_pageMode == DesQDocs::View::MultiPage ) {
        // An imaginary, 2px height line at the upper half of the viewport, which is used to
        // determine which page is currently located there -> we propagate that as 'current' page
        // to the DesQDocs::Navigation object
        const QRect currentPageLine( m_viewport.x(), m_viewport.y() + m_viewport.height() * 0.4, m_viewport.width(), 2 );

        int currentPage = 0;
        for ( auto it = m_documentLayout.pageGeometries.cbegin(); it != m_documentLayout.pageGeometries.cend(); ++it ) {
            const QRect pageGeometry = it.value();
            if ( pageGeometry.intersects( currentPageLine ) ) {
                currentPage = it.key();
                break;
            }
        }

        if ( currentPage != m_pageNavigation->currentPage() ) {
            m_blockPageScrolling = true;
            m_pageNavigation->setCurrentPage( currentPage );
            m_blockPageScrolling = false;
        }
    }
};

void DesQDocs::ViewPrivate::updateScrollBars() {

    Q_Q( View );

    const QSize p = q->viewport()->size();
    const QSize v = m_documentLayout.documentSize;

    q->horizontalScrollBar()->setRange( 0, v.width() - p.width() );
    q->horizontalScrollBar()->setPageStep( p.width() );
    q->verticalScrollBar()->setRange( 0, v.height() - p.height() );
    q->verticalScrollBar()->setPageStep( p.height() );
};

void DesQDocs::ViewPrivate::invalidateDocumentLayout() {

    Q_Q( View );

    updateDocumentLayout();
    q->viewport()->update();
};

DesQDocs::ViewPrivate::DocumentLayout DesQDocs::ViewPrivate::calculateDocumentLayout() const {
    // The DocumentLayout describes a virtual layout where all pages are positioned inside
    //    - For SinglePage mode, this is just an area as large as the current page surrounded
    //      by the m_documentMargins.
    //    - For MultiPage mode, this is the area that is covered by all pages which are placed
    //      below each other, with m_pageSpacing inbetween and surrounded by m_documentMargins

    DocumentLayout documentLayout;

    if ( !m_document || m_document->status() != DesQDocs::Document::Ready )
        return documentLayout;

    QHash<int, QRect> pageGeometries;

    const int pageCount = m_document->pageCount();

    int totalWidth = 0;

    const int startPage = ( m_pageMode == DesQDocs::View::SinglePage ? m_pageNavigation->currentPage() : 0 );
    const int endPage = ( m_pageMode == DesQDocs::View::SinglePage ? m_pageNavigation->currentPage() + 1 : pageCount );

    // calculate page sizes
    for ( int page = startPage; page < endPage; ++page ) {
        QSize pageSize;
        if ( m_zoomMode == DesQDocs::View::CustomZoom ) {
            pageSize = QSizeF( m_document->pageSize( page ) * m_screenResolution * m_zoomFactor ).toSize();
        }

        else if ( m_zoomMode == DesQDocs::View::FitToWidth ) {
            pageSize = QSizeF( m_document->pageSize( page ) * m_screenResolution ).toSize();
            const qreal factor = ( qreal( m_viewport.width() - m_documentMargins.left() - m_documentMargins.right() ) / qreal( pageSize.width() ) );
            pageSize *= factor;
        }

        else if ( m_zoomMode == DesQDocs::View::FitInView ) {
            const QSize viewportSize( m_viewport.size() + QSize( -m_documentMargins.left() - m_documentMargins.right(), -m_pageSpacing ) );

            pageSize = QSizeF( m_document->pageSize( page ) * m_screenResolution ).toSize();
            pageSize = pageSize.scaled( viewportSize, Qt::KeepAspectRatio );
        }

        switch( m_renderOpts.rotation() ) {
            /* Normal */
            case DesQDocs::RenderOptions::Rotate0:
            case DesQDocs::RenderOptions::Rotate180: {
                totalWidth = qMax( totalWidth, pageSize.width() );
                pageGeometries[page] = QRect( QPoint( 0, 0 ), pageSize );
                break;
            }

            /* 90 degree rotated */
            case DesQDocs::RenderOptions::Rotate90:
            case DesQDocs::RenderOptions::Rotate270: {
                totalWidth = qMax( totalWidth, pageSize.height() );
                pageGeometries[page] = QRect( 0, 0, pageSize.height(), pageSize.width() );
                break;
            }
        }
    }

    totalWidth += m_documentMargins.left() + m_documentMargins.right();

    int pageY = m_documentMargins.top();

    // calculate page positions
    for ( int page = startPage; page < endPage; ++page ) {
        const QSize pageSize = pageGeometries[page].size();

        // center horizontal inside the viewport
        const int pageX = ( qMax( totalWidth, m_viewport.width() ) - pageSize.width() ) / 2;
        pageGeometries[page].moveTopLeft( QPoint( pageX, pageY ) );
        pageY += pageSize.height() + m_pageSpacing;
    }

    pageY += m_documentMargins.bottom();

    documentLayout.pageGeometries = pageGeometries;

    // calculate overall document size
    documentLayout.documentSize = QSize( totalWidth, pageY );

    return documentLayout;
};

qreal DesQDocs::ViewPrivate::yPositionForPage( int pageNumber ) const {

    const auto it = m_documentLayout.pageGeometries.constFind( pageNumber );
    if ( it == m_documentLayout.pageGeometries.cend() )
        return 0.0;

    return ( *it ).y();
};

void DesQDocs::ViewPrivate::updateDocumentLayout() {

    m_documentLayout = calculateDocumentLayout();

    updateScrollBars();
};

qreal DesQDocs::ViewPrivate::zoomFactor() const {

    int page = m_pageNavigation->currentPage();

    QSize pageSize;
    if ( m_zoomMode == DesQDocs::View::CustomZoom ) {
        return m_zoomFactor;
    }

    else if ( m_zoomMode == DesQDocs::View::FitToWidth ) {
        pageSize = QSizeF( m_document->pageSize( page ) * m_screenResolution ).toSize();
        const qreal factor = ( qreal( m_viewport.width() - m_documentMargins.left() - m_documentMargins.right() ) / qreal( pageSize.width() ) );
        return factor;
    }

    else if ( m_zoomMode == DesQDocs::View::FitInView ) {
        const QSize viewportSize( m_viewport.size() + QSize( -m_documentMargins.left() - m_documentMargins.right(), -m_pageSpacing ) );

        pageSize = QSizeF( m_document->pageSize( page ) * m_screenResolution ).toSize();
        pageSize = pageSize.scaled( viewportSize, Qt::KeepAspectRatio );

        return pageSize.width() / m_document->pageSize( page ).width();
    }

    return 1.0;
};

DesQDocs::View::View( QWidget *parent ) : QAbstractScrollArea( *new ViewPrivate(), parent ) {

    Q_D( View );

    d->init();

    /* Setup Page Navigation */
    connect( d->m_pageNavigation, &DesQDocs::Navigation::currentPageChanged, this, [d]( int page ){ d->currentPageChanged( page ); } );

    /* Setup Page Renderer */
    connect(
        d->m_pageRenderer, &DesQDocs::Renderer::pageRendered, [=]( int pageNo ) {

            viewport()->update();
        }
    );

    /* Zoom buttons */
    mZoomBtn = new Zoom( this );
    if ( d->m_zoomMode == CustomZoom ) {
        mZoomBtn->show();
        mZoomBtn->setEnlargeEnabled( d->m_zoomFactor < 4.0 ? true : false );
        mZoomBtn->setDwindleEnabled( d->m_zoomFactor > 0.1 ? true : false );
    }

    else {
        mZoomBtn->hide();
    }

    connect(
        mZoomBtn, &Zoom::clicked, [=]( QString action ) {
            if ( action == "enlarge" )
                setZoomFactor( d->m_zoomFactor * 1.10 );

            else
                setZoomFactor( d->m_zoomFactor / 1.10 );

            mZoomBtn->setEnlargeEnabled( d->m_zoomFactor < 4.0 ? true : false );
            mZoomBtn->setDwindleEnabled( d->m_zoomFactor > 0.1 ? true : false );
        }
    );

    /* Page buttons */
	mPagesBtn = new PageWidget( this );
	connect( d->m_pageNavigation, &DesQDocs::Navigation::currentPageChanged, mPagesBtn, &PageWidget::setCurrentPage );
	connect( mPagesBtn, &PageWidget::loadPage, d->m_pageNavigation, &DesQDocs::Navigation::setCurrentPage );

    /* ProgressBar */
    progress = new QProgressBar( this );
    progress->move( 5, 5 );
    progress->setFixedSize( 50, 10 );
    progress->setRange( 0, 100 );
    progress->setStyle( QStyleFactory::create( "fusion" ) );
    progress->setFormat( "" );

    /* Does this work? */
    verticalScrollBar()->setSingleStep( 20 );
    horizontalScrollBar()->setSingleStep( 20 );

    /* First load */
    d->calculateViewport();

    /* Don't draw a frame around the QScrollArea */
	setFrameStyle( QFrame::NoFrame );

    /* 100% Zoom */
    QShortcut *shortcut = new QShortcut( this );
    shortcut->setKey( QKeySequence( Qt::CTRL + Qt::Key_0 ) );
    connect(
        shortcut, &QShortcut::activated, [=]() {
            setZoomFactor( 1.0 );
        }
    );
};

DesQDocs::View::View( ViewPrivate &dd, QWidget *parent ) : QAbstractScrollArea( dd, parent ) {
};

DesQDocs::View::~View() {
};

void DesQDocs::View::loadDocument( QString path ) {

	DesQDocs::Document *doc = new PopplerDocument( path );

    progress->show();
    connect(
        doc, &Document::loading, [=]( int pc ) {
            progress->setValue( pc );

            if ( pc == 100 )
                progress->hide();

            qApp->processEvents();
        }
    );

	doc->load();

    if ( doc->passwordNeeded() ) {
        bool ok = true;
        int count = 0;
        do {
            QString passwd = QInputDialog::getText(
                this,
                "DesQDocs | Encrypted Document",
                QString( "%1Please enter the document password:" ).arg( count ? "You may have entered the wrong password.<br>" : "" ),
                QLineEdit::Password,
                QString(),
                &ok,
                Qt::WindowFlags(),
                Qt::ImhSensitiveData
            );

            doc->setPassword( passwd );
            count++;
        } while ( ok == true and doc->passwordNeeded() );

        /* User cancelled loading the document */
        if ( not ok )
            return;
    }

    /* Password has been supplied (if needed), document ready to be loaded */
    setDocument( doc );
};

void DesQDocs::View::setDocument( DesQDocs::Document *document ) {

    Q_D( View );

    if ( d->m_document == document )
        return;

    if ( d->m_document ) {
        disconnect( d->m_documentStatusChangedConnection );
        disconnect( d->m_reloadDocumentConnection );
	}

    d->m_document = document;
    emit documentChanged( d->m_document );

    if ( d->m_document ) {
        d->m_documentStatusChangedConnection = connect(
            d->m_document, &DesQDocs::Document::statusChanged, this, [=]( DesQDocs::Document::Status sts ) {
                d->documentStatusChanged();

                if ( sts == DesQDocs::Document::Loading ) {
                    progress->show();
                }

                else {
                    progress->hide();
                }
            }
        );

        d->m_reloadDocumentConnection = connect(
            d->m_document, &DesQDocs::Document::reloadDocument, this, [ d ]() {
                d->m_pageRenderer->reload();
            }
        );
    }

    d->m_pageNavigation->setDocument( d->m_document );
    d->m_pageRenderer->setDocument( d->m_document );

    d->documentStatusChanged();

	mPagesBtn->setMaximumPages( document->pageCount() );
	mPagesBtn->setCurrentPage( d->m_pageNavigation->currentPage() );
};

DesQDocs::Document *DesQDocs::View::document() const {

    Q_D( const View );

    return d->m_document;
};

DesQDocs::Navigation *DesQDocs::View::pageNavigation() const {

    Q_D( const View );

    return d->m_pageNavigation;
};

DesQDocs::View::PageMode DesQDocs::View::pageMode() const {

    Q_D( const View );

    return d->m_pageMode;
};

void DesQDocs::View::setPageMode( PageMode mode ) {

    Q_D( View );

    if ( d->m_pageMode == mode )
        return;

    d->m_pageMode = mode;
    d->invalidateDocumentLayout();

    emit pageModeChanged( d->m_pageMode );
};

DesQDocs::View::ZoomMode DesQDocs::View::zoomMode() const {

    Q_D( const View );

    return d->m_zoomMode;
};

void DesQDocs::View::setZoomMode( ZoomMode mode ) {

    Q_D( View );

    if ( d->m_zoomMode == mode )
        return;

    d->m_zoomMode = mode;
    d->invalidateDocumentLayout();

    if ( d->m_zoomMode == CustomZoom )
        mZoomBtn->show();

    else
        mZoomBtn->hide();

    emit zoomModeChanged( d->m_zoomMode );
};

qreal DesQDocs::View::zoomFactor() const {

    Q_D( const View );

    return d->m_zoomFactor;
};

void DesQDocs::View::setRenderOptions( DesQDocs::RenderOptions opts ) {

    Q_D( View );

    if ( d->m_renderOpts == opts )
        return;

    d->m_renderOpts = opts;
    d->invalidateDocumentLayout();

    emit renderOptionsChanged( d->m_renderOpts );
};

DesQDocs::RenderOptions DesQDocs::View::renderOptions() const {

    Q_D( const View );
    return d->m_renderOpts;
};

void DesQDocs::View::setZoomFactor( qreal factor ) {

    Q_D( View );

    if ( d->m_zoomFactor == factor )
        return;

    if ( factor > 4.0 )
        factor = 4.0;

    if ( factor < 0.1 )
        factor = 0.1;

    d->m_zoomFactor = factor;
    d->invalidateDocumentLayout();

    emit zoomFactorChanged( d->m_zoomFactor );
};

int DesQDocs::View::pageSpacing() const {

    Q_D( const View );

    return d->m_pageSpacing;
};

void DesQDocs::View::setPageSpacing( int spacing ) {

    Q_D( View );

    if ( d->m_pageSpacing == spacing )
        return;

    d->m_pageSpacing = spacing;
    d->invalidateDocumentLayout();

    emit pageSpacingChanged( d->m_pageSpacing );
};

QMargins DesQDocs::View::documentMargins() const {

    Q_D( const View );

    return d->m_documentMargins;
};

void DesQDocs::View::setDocumentMargins( QMargins margins ) {

    Q_D( View );

    if ( d->m_documentMargins == margins )
        return;

    d->m_documentMargins = margins;
    d->invalidateDocumentLayout();

    emit documentMarginsChanged( d->m_documentMargins );
};

void DesQDocs::View::paintEvent( QPaintEvent *event ) {

    Q_D( View );

    QPainter painter( viewport() );
    painter.fillRect( event->rect(), palette().brush( QPalette::Dark ) );
    painter.translate( -d->m_viewport.x(), -d->m_viewport.y() );

    for ( auto it = d->m_documentLayout.pageGeometries.cbegin(); it != d->m_documentLayout.pageGeometries.cend(); ++it ) {
        const QRect pageGeometry = it.value();
        if ( pageGeometry.intersects( d->m_viewport ) ) {
            painter.fillRect( pageGeometry, Qt::white );

            const int page = it.key();
            QImage img = d->m_pageRenderer->requestPage( page, pageGeometry.size(), d->m_renderOpts );

            if ( img.width() and img.height() )
                painter.drawImage( pageGeometry.topLeft(), img );
        }
    }
};

void DesQDocs::View::resizeEvent( QResizeEvent *event ) {

    Q_D( View );

    QAbstractScrollArea::resizeEvent( event );
    qApp->processEvents();

    if ( mZoomBtn and mZoomBtn->isVisible() )
		mZoomBtn->move( 5, viewport()->height() - mZoomBtn->height() - 5 );


    if ( mPagesBtn )
		mPagesBtn->move( viewport()->width() - mPagesBtn->width() - 5, viewport()->height() - mPagesBtn->height() - 5 );


    d->updateScrollBars();

    qApp->processEvents();

    if ( d->pendingResize )
        return;

    d->pendingResize = true;
    QTimer::singleShot(
        250, [d]() {
            d->calculateViewport();
            d->pendingResize = false;
        }
    );
};

void DesQDocs::View::scrollContentsBy( int dx, int dy ) {

    Q_D( View );

    QAbstractScrollArea::scrollContentsBy( dx, dy );

    d->calculateViewport();
};

void DesQDocs::View::keyPressEvent( QKeyEvent *kEvent ) {

    Q_D( View );

    switch( kEvent->key() ) {
        case Qt::Key_Right: {
            /* Go to next page */
            d->m_pageNavigation->setCurrentPage( d->m_pageNavigation->currentPage() + 1 );
            break;
        }

        case Qt::Key_Left: {
            /* Go to previous page */
            d->m_pageNavigation->setCurrentPage( d->m_pageNavigation->currentPage() - 1 );
            break;
        }

        case Qt::Key_Space: {
            /* Move by approximately 90% of the viewport height */
            verticalScrollBar()->setValue( verticalScrollBar()->value() + viewport()->height() * 0.9 );
            break;
        }

        case Qt::Key_Home: {
            /* Go to first page */
            verticalScrollBar()->setValue( 0 );
            break;
        }

        case Qt::Key_End: {
            /* Go to end of the document */
            verticalScrollBar()->setValue( verticalScrollBar()->maximum() );
            break;
        }

        case Qt::Key_Plus: {
            /* Zoom In */
            setZoomFactor( d->m_zoomFactor * 1.10 );
            break;
        }

        case Qt::Key_Minus: {
            /* Zoom Out */
            setZoomFactor( d->m_zoomFactor / 1.10 );
            break;
        }

        default: {
            /* Default action */
            QAbstractScrollArea::keyPressEvent( kEvent );
            break;
        }
    }
};

void DesQDocs::View::wheelEvent( QWheelEvent *wEvent ) {

    Q_D( View );

    if ( wEvent->modifiers() & Qt::ControlModifier ) {
        QPoint numDegrees = wEvent->angleDelta() / 8;

        int steps = numDegrees.y() / 15;

        if ( steps > 0 )
            setZoomFactor( d->m_zoomFactor * 1.10 );

        else
            setZoomFactor( d->m_zoomFactor / 1.10 );

        return;
    }

    QAbstractScrollArea::wheelEvent( wEvent );
};
