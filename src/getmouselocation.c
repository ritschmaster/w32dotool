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

#include "getmouselocation.h"

#include <processenv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <windows.h>
#include <wingdi.h>

#define WDT_GETMOUSELOCATION_GETOPT_OPTIONS ""

static struct option WDT_GETMOUSELOCATION_GETOPT_LONG_OPTIONS[] = {
/*   NAME          ARGUMENT           FLAG  SHORTNAME */
    {"shell",      no_argument,       NULL, '1'},
    {NULL,         0,                 NULL, 0}
};

typedef struct wdt_getmouselocation_comm_s
{
    POINT *point;
    char *screen;
} wdt_getmouselocation_comm_t;

static int
wdt_getmouselocation_free_impl(wdt_cmd_t *cmd);

static int
wdt_getmouselocation_exec_impl(wdt_cmd_t *cmd);

static int
wdt_getmouselocation_is_shell_impl(wdt_getmouselocation_t *getmouselocation);

static BOOL CALLBACK
wdt_getmouselocation_enum_monitors(HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM data);

wdt_getmouselocation_t *
wdt_getmouselocation_new(int argc, char *argv[])
{
    wdt_getmouselocation_t *getmouselocation;
    wdt_cmd_t *cmd;
    int i;
    int found_at;
    int found_end;
    int opt;
    int option_index;
    int error;
    const char *pattern;
    const char *pcre_err_str;
    int pcre_err_offset;

    getmouselocation = malloc(sizeof(wdt_getmouselocation_t));
    if (getmouselocation) {
        memset(getmouselocation, 0, sizeof(wdt_getmouselocation_t));

        cmd = wdt_cmd_new();
        memcpy(getmouselocation, cmd, sizeof(wdt_cmd_t));
        free(cmd); /* Just free the top level element */

        getmouselocation->super_wdt_getmouselocation_free = getmouselocation->cmd.wdt_cmd_free;
        getmouselocation->cmd.wdt_cmd_free = wdt_getmouselocation_free_impl;
        getmouselocation->cmd.wdt_cmd_exec = wdt_getmouselocation_exec_impl;
        getmouselocation->wdt_getmouselocation_is_shell = wdt_getmouselocation_is_shell_impl;
    }

    found_at = 0;
    found_end = found_at;
    for (i = found_at; i < argc; i++) {
        if (strcmp(argv[i], "getmouselocation") == 0) {
            found_at = i;
            found_end = found_at;
        } else if (strcmp(argv[i], "--shell")) {
           found_end = i;
        }
    }
    found_end++;

    error = 0;
    if (found_at) {
        option_index = 0;
        while ((opt = getopt_long(found_end - found_at, argv + found_at,
                                  WDT_GETMOUSELOCATION_GETOPT_OPTIONS,
                                  WDT_GETMOUSELOCATION_GETOPT_LONG_OPTIONS,
                                  &option_index))
               != -1) {
            switch(opt) {
                case '1':
                    getmouselocation->shell = 1;
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

    if (error) {
        wdt_cmd_free((wdt_cmd_t *) getmouselocation);
        getmouselocation = NULL;
    }

    return getmouselocation;
}

int
wdt_getmouselocation_is_shell(wdt_getmouselocation_t *getmouselocation)
{
    return getmouselocation->wdt_getmouselocation_is_shell(getmouselocation);
}

int
wdt_getmouselocation_free_impl(wdt_cmd_t *cmd)
{
    wdt_getmouselocation_t *getmouselocation;

    getmouselocation = (wdt_getmouselocation_t *) cmd;

    return getmouselocation->super_wdt_getmouselocation_free((wdt_cmd_t *) getmouselocation);
}

int
wdt_getmouselocation_exec_impl(wdt_cmd_t *cmd)
{
    wdt_getmouselocation_t *getmouselocation;
    POINT point;
    HWND window_handler;
    char screen[CCHDEVICENAME];
    wdt_getmouselocation_comm_t comm;
    char buf[50];

    getmouselocation = (wdt_getmouselocation_t *) cmd;

    GetCursorPos(&point);
    window_handler = WindowFromPoint(point);

    comm.point = &point;
    comm.screen = screen;
    EnumDisplayMonitors(NULL, NULL, wdt_getmouselocation_enum_monitors, (LPARAM) &comm);

    fprintf(stdout, "X=%d\nY=%d\nSCREEN=%s\nWINDOW=%p", point.x, point.y, screen, window_handler);

    if (wdt_getmouselocation_is_shell(getmouselocation)) {
        sprintf(buf, "%d", point.x);
        SetEnvironmentVariable("X", buf);

        sprintf(buf, "%d", point.y);
        SetEnvironmentVariable("Y", buf);

        SetEnvironmentVariable("SCREEN", screen);

        sprintf(buf, "%p", window_handler);
        SetEnvironmentVariable("WINDOW_HANDLER", buf);
    }

    return 0;
}

int
wdt_getmouselocation_is_shell_impl(wdt_getmouselocation_t *getmouselocation)
{
    return getmouselocation->shell;
}

BOOL CALLBACK
wdt_getmouselocation_enum_monitors(HMONITOR monitor, HDC hdc, LPRECT rect, LPARAM data)
{
    wdt_getmouselocation_comm_t *comm;
    MONITORINFOEX monitor_info;

    comm = (wdt_getmouselocation_comm_t *) data;

    monitor_info.cbSize = sizeof(MONITORINFOEX);
    GetMonitorInfo(monitor, (LPMONITORINFO) &monitor_info);

    if (PtInRect(&(monitor_info.rcMonitor), *(comm->point))) {
        strncpy(comm->screen, monitor_info.szDevice, CCHDEVICENAME);
    }

    return TRUE;
}
