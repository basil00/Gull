The LazyGull Chess Engine
=========================

LazyGull is a free UCI chess engine derived from Gull 3.  Compared to Gull,
LazyGull implements the following new features:

* Syzygy Tablebase support;
* LazySMP implementation;
* PDEP Bitboards for modern CPUs;
* Bug fixes; and
* Portable to Windows, Linux and MacOSX.

To build simply run the following commands in the `src` directory:

    make -f Makefile.linux
    make -f Makefile.windows

And follow the instructions.

Pre-built binaries are available here: https://github.com/basil00/Gull/releases

License
-------

The original Gull 3 source code is "public domain".  New modifications are
released under the MIT License.

