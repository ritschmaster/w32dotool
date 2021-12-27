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

#include "windowactivate.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#define WDT_WINDOWACTIVATE_GETOPT_OPTIONS ""

static struct option WDT_WINDOWACTIVATE_GETOPT_LONG_OPTIONS[] = {
/*   NAME          ARGUMENT           FLAG  SHORTNAME */
    {"sync",       no_argument,       NULL, 's'},
    {NULL,         0,                 NULL, 0}
};

static int
wdt_windowactivate_free_impl(wdt_cmd_t *cmd);

static int
wdt_windowactivate_exec_impl(wdt_cmd_t *cmd);

static int
wdt_windowactivate_is_sync_impl(wdt_windowactivate_t *windowactivate);

static HWND
wdt_windowactivate_get_winid_impl(wdt_windowactivate_t *windowactivate);

static BOOL CALLBACK
wdt_windowactivate_enum_windows(HWND window_handler, LPARAM param);

wdt_windowactivate_t *
wdt_windowactivate_new(int argc, char *argv[])
{
    wdt_windowactivate_t *windowactivate;
    wdt_cmd_t *cmd;
    int i;
    int found_at;
    int found_end;
    int opt;
    int option_index;
    int error;
    const char *window_handler_str;

    windowactivate = malloc(sizeof(wdt_windowactivate_t));
    if (windowactivate) {
        memset(windowactivate, 0, sizeof(wdt_windowactivate_t));

        cmd = wdt_cmd_new();
        memcpy(windowactivate, cmd, sizeof(wdt_cmd_t));
        free(cmd); /* Just free the top level element */

        windowactivate->super_wdt_windowactivate_free = windowactivate->cmd.wdt_cmd_free;
        windowactivate->cmd.wdt_cmd_free = wdt_windowactivate_free_impl;
        windowactivate->cmd.wdt_cmd_exec = wdt_windowactivate_exec_impl;
        windowactivate->wdt_windowactivate_is_sync = wdt_windowactivate_is_sync_impl;
        windowactivate->wdt_windowactivate_get_winid = wdt_windowactivate_get_winid_impl;
    }

    found_at = 0;
    found_end = 0;
    for (i = found_at; i < argc; i++) {
        if (strcmp(argv[i], "windowactivate") == 0) {
            found_at = i;
            found_end = found_at;
        } else if (strcmp(argv[i], "--sync")) {
           found_end = i;
        }
    }
    found_end++;

    error = 0;
    if (found_at) {
        option_index = 0;
        while ((opt = getopt_long(found_end - found_at, argv + found_at,
                                  WDT_WINDOWACTIVATE_GETOPT_OPTIONS,
                                  WDT_WINDOWACTIVATE_GETOPT_LONG_OPTIONS,
                                  &option_index))
               != -1) {
            switch(opt) {
                case 's':
                    windowactivate->sync = 1;
                    break;

                default:
                    // TODO?
                    error = 1;
            }
        }
    } else {
        error = 1;
    }

    if (!error && optind >= argc) {
        // TODO error
        error = 1;
    }

    if (!error) {
        window_handler_str = argv[optind];
        windowactivate->winid = (HWND) strtol(window_handler_str, NULL, 16);
    }

    if (error) {
        wdt_cmd_free((wdt_cmd_t *) windowactivate);
        windowactivate = NULL;
    }

    return windowactivate;
}

int
wdt_windowactivate_is_sync(wdt_windowactivate_t *windowactivate)
{
    return windowactivate->wdt_windowactivate_is_sync(windowactivate);
}

HWND
wdt_windowactivate_get_winid(wdt_windowactivate_t *windowactivate)
{
    return windowactivate->wdt_windowactivate_get_winid(windowactivate);
}

int
wdt_windowactivate_free_impl(wdt_cmd_t *cmd)
{
    wdt_windowactivate_t *windowactivate;

    windowactivate = (wdt_windowactivate_t *) cmd;

    return windowactivate->super_wdt_windowactivate_free((wdt_cmd_t *) windowactivate);
}

int
wdt_windowactivate_exec_impl(wdt_cmd_t *cmd)
{
    wdt_windowactivate_t *windowactivate;
    HWND window_handler;
    int error;
    int i;
    DWORD this_tid;
    DWORD foreground_tid;

    windowactivate = (wdt_windowactivate_t *) cmd;

    window_handler = wdt_windowactivate_get_winid(windowactivate);

    error = 0;

    foreground_tid = GetCurrentThreadId();
    this_tid = GetWindowThreadProcessId(GetForegroundWindow(), 0);

    if(this_tid != foreground_tid) {
        AttachThreadInput(this_tid, foreground_tid, TRUE);
        AllowSetForegroundWindow(ASFW_ANY);
    }

    /**
     * The WIN32 API needs to be spammed to actually update the focus.
     */
    int upper;
    upper = 10000;
    for (i = 0; i < upper && GetActiveWindow() != window_handler; i++) {
        SetForegroundWindow(window_handler);
        SetActiveWindow(window_handler);
        SetFocus(window_handler);
        Sleep(0.5);
    }

    if(this_tid != foreground_tid) {
        AttachThreadInput(this_tid, foreground_tid, FALSE);
    }

    return error;
}

int
wdt_windowactivate_is_sync_impl(wdt_windowactivate_t *windowactivate)
{
    return windowactivate->sync;
}

HWND
wdt_windowactivate_get_winid_impl(wdt_windowactivate_t *windowactivate)
{
    return windowactivate->winid;
}
