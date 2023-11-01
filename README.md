# w32dotool

![GitHub](https://img.shields.io/github/license/ritschmaster/w32dotool)
![GitHub All Releases](https://img.shields.io/github/downloads/ritschmaster/w32dotool/total)
[![Donate with Bitcoin](https://en.cryptobadges.io/badge/micro/3DF2eTL9KSndqbuQTokWvTbUpDNQ6RuwxU)](#donations)

xdotool for Windows

## Current state

I have moved on to use macOS exclusively. As a result I completely dropped using xdotool/w32dotool and also stopped developing w32dotool. Therefore I am open to transfer the ownership to anyone who is willing to actively maintain this piece of software. Just write me an e-mail.

## Description 

w32dotool is a clone of [xdotool](https://www.semicomplete.com/projects/xdotool/) but targeting Windows. Its ultimate goal is to fully replicate ALL commands supplied by xdotool.

The list of fully supported commands: still empty

The list of partially supported commands:
1. `getmouselocation`
 1. Does not respect the window stack
2. `search`
 1. Does not respect the window stack
3. `windowactivate`
 1. Does not respect the window stack

## Dependencies

* [MinGW](http://mingw.org/)

## Compiling

### On Windows using Msys2

You will need a functioning MinGW environment. This sections describes the compilation using [Msys2](https://www.msys2.org/).

First install the dependencies:

    pacman -S gcc make automake autoconf mingw-w64-x86_64-gcc

Use the __MSYS2 MinGW 64-bit__ Terminal for the next actions and everytime you want to re-compile w32dotool.

Next up get w32dotool:

    git clone https://github.com/ritschmaster/w32dotool
    cd w32dotool

Finally compile w32dotool:

    export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:/mingw64/lib/pkgconfig
    ./autogen.sh
    make

Afterwards you may install w32dotool by performing:

    make install

### On Fedora Linux (64 bit)

First install MinGW:

    sudo dnf install -y mingw64-gcc mingw64-gcc-c++ mingw64-winpthreads-static cmake automake autoconf autoconf-archive libtool

Then get w32dotool:

    git clone https://github.com/ritschmaster/w32dotool
    cd w32dotool

Finally compile w32dotool:

    export PKG_CONFIG_PATH=/usr/x86_64-w64-mingw32/lib/pkgconfig/
    ./configure --host=x86_64-w64-mingw32
    make

## Using w32dotool

### Using precompiled releases

1. Head over to the [Release](https://github.com/ritschmaster/w32dotool/releases) page and download the latest zip file.
2. Decompress it somwhere
3. Execute `w32dotool.exe`


## Author

Richard Bäck <richard.baeck@mailbox.org>


## License

MIT License


## Donations

Show me that you really like it by donating something. Thanks for any amount!

My Bitcoin address: 3DF2eTL9KSndqbuQTokWvTbUpDNQ6RuwxU
