TEMPLATE = subdirs

SUBDIRS += lib example mupdf mujs

mupdf.file = mupdf/source/mupdf.pro
mujs.file = mupdf/mujs/mujs.pro

lib.depends = mupdf
example.depends = lib
