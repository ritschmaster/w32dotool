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

#ifndef W32DOTOOL_SEARCH_H
#define W32DOTOOL_SEARCH_H

#include <pcre.h>

#include "cmd.h"

typedef struct wdt_search_s wdt_search_t;

struct wdt_search_s
{
    wdt_cmd_t cmd;

    int (* super_wdt_search_free)(wdt_cmd_t *cmd);
    int (* wdt_search_is_any)(wdt_search_t *search);
    int (* wdt_search_is_all)(wdt_search_t *search);
    int (* wdt_search_is_sync)(wdt_search_t *search);
    int (* wdt_search_is_class)(wdt_search_t *search);
    int (* wdt_search_is_name)(wdt_search_t *search);

    int any;
    int all;
    int sync;
    int class;
    int name;

    pcre *re_compiled;
    pcre_extra *re_extra;
};

extern wdt_search_t *
wdt_search_new(int argc, char *argv[]);

extern int
wdt_search_is_any(wdt_search_t *search);

extern int
wdt_search_is_all(wdt_search_t *search);

extern int
wdt_search_is_sync(wdt_search_t *search);

extern int
wdt_search_is_class(wdt_search_t *search);

extern int
wdt_search_is_name(wdt_search_t *search);

#endif // W32DOTOOL_SEARCH_H
