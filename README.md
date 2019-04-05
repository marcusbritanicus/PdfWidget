# PdfWidget
## A nascent PDF view written based on Qt

Its very few features are
- Two render backends: MuPdf and Poppler
- Encryption support
- Zoom in and Zoom out (10% to 400%)

### Qt4 and Qt5 builds are supported

### Notes for compiling - linux:

* Download the sources
   - Git: `git clone https://gitlab.com/marcusbritanicus/PdfWidget PdfWidget-master`
   - Tar: `wget https://gitlab.com/marcusbritanicus/PdfWidget/archive/master.tar.gz && tar -xf master.tar.gz`
* Enter `PdfWidget-master`
* Open the terminal and type: `qmake && make`

Because there are a lot of versions of mupdf out there and some of the older versions have now-outdated function names,
mupdf source in included in the build tree. The MuPDF source is picked from Debian/Sid (Source Version 1.13.0+ds1-2).

Because Noto, Droid and Hangul fonts bloat the library size to 45 MiB, I have removed them from the build tree.
We also disable them using the flags `-DNOCJK -DTOFU`. To disable gl, we use `HAVE_GLUT=np` and use `-fPIC` so that
we can link it with other shared objects. Though the source is nearly 11 MiB in size the library is ~8 MiB
and the libpdfwidget.so will be after linking to mupdf ~3 MiB which is a decent size.

### Known issues
- In the example implementation, changing the zoom causes the document to scroll to page 1.
- Changing the zoom in the example implementation causes my system to freeze up at times.
