/*
	*
	* ActionButtons.cpp - Minimize/Maximize/Close Button Widget Class
	*
*/

#pragma once

#include "Global.hpp"

class ActionButtons : public QWidget {
	Q_OBJECT

	public:
		ActionButtons( bool, bool, bool );
		~ActionButtons();

	private:
		QPainter *painter;

		bool maxBtnEnabled;
		bool minBtnEnabled;
		bool closeBtnEnabled;

		quint64 widgetWidth;

		QRect minRect;
		QRect maxRect;
		QRect clsRect;

	protected:
		void mousePressEvent( QMouseEvent * );
		void mouseMoveEvent( QMouseEvent * );
		void paintEvent( QPaintEvent * );

	Q_SIGNALS:
		void maximizeWindow();
		void minimizeWindow();
		void closeWindow();
};
