/******************************************************************************
  This file is part of w32dotool.

  Copyright 2021-2022 Richard Paul Baeck <richard.baeck@mailbox.org>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*******************************************************************************/

#ifndef W32DOTOOL_WINDOWACTIVATE_H
#define W32DOTOOL_WINDOWACTIVATE_H

#include <pcre.h>
#include <windows.h>

#include "cmd.h"

typedef struct wdt_windowactivate_s wdt_windowactivate_t;

struct wdt_windowactivate_s
{
    wdt_cmd_t cmd;

    int (* super_wdt_windowactivate_free)(wdt_cmd_t *cmd);
    int (* wdt_windowactivate_is_sync)(wdt_windowactivate_t *windowactivate);
    HWND (* wdt_windowactivate_get_winid)(wdt_windowactivate_t *windowactivate);

    int sync;
    HWND winid;
};

extern wdt_windowactivate_t *
wdt_windowactivate_new(int argc, char *argv[]);

extern int
wdt_windowactivate_is_sync(wdt_windowactivate_t *windowactivate);

extern HWND
wdt_windowactivate_get_winid(wdt_windowactivate_t *windowactivate);

#endif // W32DOTOOL_WINDOWACTIVATE_H
