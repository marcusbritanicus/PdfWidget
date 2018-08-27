#!/bin/sh

# Total cleaning of the source
rm -rf build/ generated/ include/mupdf/pdf/name-table.h source/pdf/pdf-name-table.h

# We will build just the library, we don't need other things
make -f Makefile -j$(nproc) libs XCFLAGS="-DJBIG_NO_MEMENTO -fPIC -DTOFU -DNOCJK" HAVE_GLUT=no
