/*
    *
    * This file is a part of PdfWidget.
    * PdfWidget is the default document viewer for the DesQ Suite
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

#pragma once

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

#include "Document.hpp"
#include "RenderOptions.hpp"

extern "C" {
	#include <mupdf/fitz.h>
}

class MuPage;

class MuPdfDocument : public PdfWidget::Document {
	Q_OBJECT;

	public:
		MuPdfDocument( QString pdfPath );

		/* Set a password */
		void setPassword( QString password );

		/* Pdf Info / Metadata */
		QString title() const;
		QString author() const;
		QString creator() const;
		QString producer() const;
		QString created() const;

	public Q_SLOTS:
		void load();
		void close();

	private:
		/* Pointer to our actual mupdf document */
		fz_context *mCtx;
		fz_document *mFzDoc;

		int mPageCount;

	Q_SIGNALS:
		void passwordChanged();
		void passwordRequired();
		void reloadDocument();
		void statusChanged( Document::Status status );
		void pageCountChanged( int pageCount );
};

class MuPage : public PdfWidget::Page {

	public:
		MuPage( fz_context *ctx, fz_page *pg, int );
		~MuPage();

		/* Size of the page */
		QSizeF pageSize( qreal zoom = 1.0 ) const;

		/* Thumbnail of the page */
		QImage thumbnail() const;

		/* Render and return a page */
		QImage render( QSize, PdfWidget::RenderOptions ) const;
		QImage render( qreal zoomFactor, PdfWidget::RenderOptions ) const;

		/* Page Text */
		QString pageText() const;

		/* Text of a Selection rectangle */
		QString text( QRectF ) const;

		/* Search for @query in @pageNo or all pages */
		QList<QRectF> search( QString query, PdfWidget::RenderOptions ) const;

	private:
		fz_page *mPage;
		fz_context *mCtx;
};
