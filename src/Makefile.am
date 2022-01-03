################################################################################
# This file is part of w32dotool.
#
# Copyright 2020-2022 Richard Paul Baeck <richard.baeck@mailbox.org>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
################################################################################

AM_YFLAGS = -d

if DEBUG
AM_CFLAGS = -O0
AM_CFLAGS += -D DEBUG_ENABLED=1
AM_LDFLAGS = -g -O0
else
AM_CFLAGS = -O2
AM_LDFLAGS = -O2
endif

bin_PROGRAMS = w32dotool

w32dotool_SOURCES = main.c
w32dotool_SOURCES += cmd.h cmd.c
w32dotool_SOURCES += getmouselocation.h getmouselocation.c
w32dotool_SOURCES += search.h search.c
w32dotool_SOURCES += windowactivate.h windowactivate.c

w32dotool_CFLAGS = $(AM_CFLAGS)

w32dotool_LDFLAGS = $(AM_LDFLAGS)
w32dotool_LDFLAGS += -static
w32dotool_LDFLAGS += -mwindows

w32dotool_LDADD = @libpcre_LIBS@
