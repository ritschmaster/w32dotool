# w32dotool

xdotool for Windows

## Dependencies

* [MinGW](http://mingw.org/)

## Compiling

### On Windows using Msys2

You will need a functioning MinGW environment. This sections describes the compilation using [Msys2](https://www.msys2.org/).

First install the dependencies:

    pacman -S gcc make cmake automake autoconf mingw-w64-x86_64-gcc

Use the __MSYS2 MinGW 64-bit__ Terminal for the next actions and everytime you want to re-compile w32bindkeys.

Then compile w32dotool:

    export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/mingw64/lib/pkgconfig
    ./autogen.sh
    make

Afterwards you may install w32dotool by performing:

    make install

### On Fedora Linux (64 bit)

First install MinGW:

    sudo dnf install -y mingw64-gcc mingw64-gcc-c++ mingw64-winpthreads-static cmake automake autoconf autoconf-archive libtool

Then compile w32dotool:

    mingw64-configure
    make

## Using w32dotool

### Using precompiled releases

1. Head over to the [Release](https://github.com/ritschmaster/w32bindkeys/releases) page and download the latest zip file.
2. Decompress it somwhere


