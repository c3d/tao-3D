# Tao3D - A language for dynamic 3D animations

Tao3D is a dynamic document description language designed specifically
for real-time 3D animations. It can be used to present complex information
in an entertaining and interactive way.

Tao3D includes support for the following features:

* A complete programming language, with loops, tests, functions, and more.
* Real-time, capable of displaying complex animations at 60FPS.
* Sophisticated 2D and 3D text layout, 2D and 3D shapes, extrusion.
* 3D objects from a variety of formats, including .OBJ and .3DS
* 3D point clouds supporting hundreds of thousands of points
* Integrated audio and movie support based on industry standard VLC
* Real-time updates based on external data files in .CSV format
* Fetch data, pictures or movies directly over HTTP

For a better overview of the software, please watch this YouTube video:
https://www.youtube.com/watch?v=Fvi29XAo4SI and the Taodyne channel
https://www.youtube.com/Taodyne.

A basic tutorial of the language can be found here: http://www.taodyne.com/presentation/tutorial-2.0.html. More tutorials, blogs and forums can be found on the Taodyne developers web site: http://www.taodyne.com/shop/dev/en/.


## Change log since 2.0-libre

* 81: Merge Libre edition and Pro code for Tao3D 1.60
* 72: Added sanity tests in the test directory
* 71: Add ROUNDED enpoint style
* 70: Add ability to specify endpoint styles using text
* 69: Fixed typo in source files for Pro version
* 68: Removed a few remaining references to TaoPresentations with Tao3D
* 67: Put web UI thumbnails in a separate hidden directory
* 66: Fix launch error on Linux - Replaced TaoPresentations with Tao3D
* 65: Put web editor thumbnails in hidden .thumbnails directory
* 64: Update background color even if it's black (ticket #26)
* 63: Update example list
* 62: Fix messages about QSslSocket on Windows with old OpenSSL
* 61: Fix rendering of 3D models with transparent colors, ticket #24
* 60: Implement indent and unindent with 'tab/shift-tab', ticket #23
* 59: Add explanation to "Shift" template
* 58: Fine-tune highlighting in text editor, ticket #21
* 57: Fix highlighting mechanism for overlapping regions, ticket #22
* 56: Fix highlighter for comments within string, ticket #22
* 55: Added missing French translations
* 54: Avoid splitting text when there's no writing direction change, ticket #21
* 53: Remove references to 'title text X', ticket #20
* 52: Added comments to Hello World example, ticket #20
* 51: Updated Hello World to make it easier to copy-paste smaller sections
* 50: Fetch examples directly from Taodyne web site, ticket #20
* 49: Disable transparent windows on Windows, it's broken, ticket #18


## Prerequisites

The software can be built on Windows, MacOSX and Linux.
To build Tao, you will need:

* A C++ compiler (Tested with g++ and clang)
* OpenGL at least version 2.1, and related development files.
  Tao does not presently support OpenGL ES, but we'd like this to happen.
* Git
* Qt version 4.8 or above (there is a crash on MacOSX with 5.3.2)
* LLVM version 2.9 (porting to 3.x is left as an exercise for the reader)

The software also downloads or includes a few extra packages, including:
* NodeJS
* CryptoPP (a somewhat dated set of files)
* GLC-Lib (a rather old version)
* VLC (you may need to build it yourself if you want multistream support)

Many thanks to all the authors who indirectly made Tao possible

## Build instructions

```
git clone https://github.com/c3d/tao-3D.git
cd tao-3D
git submodule update --init --recursive
./configure
make install
```

## Free Software (GPLv3) and Commercial License

This software is free, available under the GNU General Public License v3.
Commercial licenses are also available. The commercial version of the product
adds, among other things:

* Direct support for stereoscopic and most auto-stereoscopic displays
* Decoding the vast majority of 3D movie formats, tiled or multi-stream
* The ability to encrypt and digitally sign the source code
