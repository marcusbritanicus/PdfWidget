/*
	*
	* Copyright 2019-2021 Britanicus <marcusbritanicus@gmail.com>
	*
	* This file is a part of DesQ project (https://gitlab.com/desq/)
	*

	*
	* This program is free software; you can redistribute it and/or modify
	* it under the terms of the GNU General Public License as published by
	* the Free Software Foundation; either version 3 of the License, or
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

#include <desq/DesQUtils.hpp>

#include "Document.hpp"

/*
	*
	* Generic class to handle document
	*
*/

PdfWidget::Document::Document( QString path ) : QObject() {

	mZoom = 1.0;
	mStatus = Null;
	mDocError = NoError;
	mPassNeeded = false;

	mDocPath = QFileInfo( path ).absoluteFilePath();

	QFileSystemWatcher *fsw = new QFileSystemWatcher();
	fsw->addPath( mDocPath );
	connect(
		fsw, &QFileSystemWatcher::fileChanged, [=]( QString file ) {
			/* File deleted and created again: add it to the watcher */
			if ( not fsw->files().contains( file ) and QFile::exists( file ) )
				fsw->addPath( file );

			reload();
		}
	);
};

QString PdfWidget::Document::fileName() const {

	return DesQ::Utils::baseName( mDocPath );
};

QString PdfWidget::Document::filePath() const {

	return DesQ::Utils::dirName( mDocPath );
};

bool PdfWidget::Document::passwordNeeded() const {

	return mPassNeeded;
};

int PdfWidget::Document::pageCount() const {

	return mPages.count();
};

QSizeF PdfWidget::Document::pageSize( int pageNo ) const {

	return mPages.at( pageNo )->pageSize() * mZoom;
};

void PdfWidget::Document::reload() {

	mStatus = Null;
	mPages.clear();

	load();

	if ( mStatus == Ready ) {
		qDebug() << "Reload your pages..";
		emit reloadDocument();
	}
};

PdfWidget::Document::Status PdfWidget::Document::status() const {

	return mStatus;
};

QImage PdfWidget::Document::renderPage( int pageNo, QSize size, PdfWidget::RenderOptions opts ) const {

	if ( pageNo >= mPages.count() )
		return QImage();

	return mPages.at( pageNo )->render( size, opts );
};

QImage PdfWidget::Document::renderPage( int pageNo, qreal zoomFactor, PdfWidget::RenderOptions opts ) const {

	if ( pageNo >= mPages.count() )
		return QImage();

	return mPages.at( pageNo )->render( zoomFactor, opts );
};

PdfWidget::Pages PdfWidget::Document::pages() const {

	return mPages;
};

PdfWidget::Page* PdfWidget::Document::page( int pageNo ) const {

	if ( pageNo >= mPages.count() )
		return nullptr;

	return mPages.at( pageNo );
};

QImage PdfWidget::Document::pageThumbnail( int pageNo ) const {

	if ( pageNo >= mPages.count() )
		return QImage();

	return mPages.at( pageNo )->thumbnail();
};

QString PdfWidget::Document::pageText( int pageNo ) const {

	return mPages.at( pageNo )->text( QRectF() );
};

QString PdfWidget::Document::text( int pageNo, QRectF rect ) const {

	return mPages.at( pageNo )->text( rect );
};

QList<QRectF> PdfWidget::Document::search( QString query, int pageNo, PdfWidget::RenderOptions opts ) const {

	searchRects.clear();

	if ( pageNo >= 0 or pageNo < mPages.count() ) {
		for( QRectF rect: mPages.at( pageNo )->search( query, opts ) ) {
			searchRects << QRect( rect.x() * mZoom, rect.y() * mZoom, rect.width() * mZoom, rect.height() * mZoom );
		}
	}

	return searchRects;
};

qreal PdfWidget::Document::zoomForWidth( int pageNo, qreal width ) const {

	if ( pageNo >= mPages.count() )
		return 0.0;

	return 1.0 * width / mPages.at( pageNo )->pageSize().width();
};

qreal PdfWidget::Document::zoomForHeight( int pageNo, qreal height ) const {

	if ( pageNo >= mPages.count() )
		return 0.0;

	return 1.0 * height / mPages.at( pageNo )->pageSize().height();
};

void PdfWidget::Document::setZoom( qreal zoom ) {

	mZoom = zoom;
};

/*
	*
	* Generic class to handle document page
	*
*/

PdfWidget::Page::Page( int pgNo ) {

	mPageNo = pgNo;
};

int PdfWidget::Page::pageNo() {

	return mPageNo;
};
