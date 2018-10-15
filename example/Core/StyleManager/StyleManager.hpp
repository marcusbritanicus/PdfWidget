/*
	*
	* StyleManager.hpp - StyleManager.cpp Header
	*
*/

#pragma once

#include <Global.hpp>

class StyleManager {

	public:
		static QString getStyleSheet( QString );
		static QPalette getPalette();
		static QPalette transparentPalette();
};
