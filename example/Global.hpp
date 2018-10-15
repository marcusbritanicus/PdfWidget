/*
    *
    * Global.hpp - Globally used header
    *
*/

#pragma once

#include <iostream>
#include <cstring>
#include <libgen.h>

#include <QtGui>
#include <QtCore>
#include <QtNetwork>

#include <QtWidgets>
#include <QtPrintSupport>
#include <QMimeDatabase>

static QSettings settings( "MiniPDF", "MiniPDF" );
static QMimeDatabase mimeDb;
