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

#include "search.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <windows.h>

#define WDT_SEARCH_GETOPT_OPTIONS ""

static struct option WDT_SEARCH_GETOPT_LONG_OPTIONS[] = {
/*   NAME          ARGUMENT           FLAG  SHORTNAME */
    {"any",        no_argument,       NULL, '1'},
    {"all",        no_argument,       NULL, '2'},
    {"sync",       no_argument,       NULL, 's'},
    {"class",      no_argument,       NULL, 'c'},
    {"name",       no_argument,       NULL, 'n'},
    {NULL,         0,                 NULL, 0}
};

#define WDT_SEARCH_CLASS_BUFLEN 1024

#define WDT_SEARCH_NAME_BUFLEN 1024

static int
wdt_search_free_impl(wdt_cmd_t *cmd);

static int
wdt_search_exec_impl(wdt_cmd_t *cmd);

static int
wdt_search_is_any_impl(wdt_search_t *search);

static int
wdt_search_is_all_impl(wdt_search_t *search);

static int
wdt_search_is_sync_impl(wdt_search_t *search);

static int
wdt_search_is_class_impl(wdt_search_t *search);

static int
wdt_search_is_name_impl(wdt_search_t *search);


static BOOL CALLBACK
wdt_search_enum_windows(HWND window_handler, LPARAM param);

wdt_search_t *
wdt_search_new(int argc, char *argv[])
{
    wdt_search_t *search;
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

    search = malloc(sizeof(wdt_search_t));
    if (search) {
        memset(search, 0, sizeof(wdt_search_t));

        cmd = wdt_cmd_new();
        memcpy(search, cmd, sizeof(wdt_cmd_t));
        free(cmd); /* Just free the top level element */

        search->super_wdt_search_free = search->cmd.wdt_cmd_free;
        search->cmd.wdt_cmd_free = wdt_search_free_impl;
        search->cmd.wdt_cmd_exec = wdt_search_exec_impl;
        search->wdt_search_is_any = wdt_search_is_any_impl;
        search->wdt_search_is_all = wdt_search_is_all_impl;
        search->wdt_search_is_sync = wdt_search_is_sync_impl;
        search->wdt_search_is_class = wdt_search_is_class_impl;
        search->wdt_search_is_name = wdt_search_is_name_impl;

        /**
         * Default is Any
         */
        search->any = 1;
    }

    found_at = 0;
    found_end = found_at;
    for (i = found_at; i < argc; i++) {
        if (strcmp(argv[i], "search") == 0) {
            found_at = i;
            found_end = found_at;
        } else if (strcmp(argv[i], "--any")
                   || strcmp(argv[i], "--all")
                   || strcmp(argv[i], "--sync")
                   || strcmp(argv[i], "--class")
                   || strcmp(argv[i], "--name")) {
           found_end = i;
        }
    }
    found_end++;

    error = 0;
    if (found_at) {

        option_index = 0;
        while ((opt = getopt_long(found_end - found_at, argv + found_at,
                                  WDT_SEARCH_GETOPT_OPTIONS,
                                  WDT_SEARCH_GETOPT_LONG_OPTIONS,
                                  &option_index))
               != -1) {
            switch(opt) {
                case '1':
                    search->any = 1;
                    break;

                case '2':
                    search->all = 1;
                    search->any = 0;
                    break;

                case 's':
                    search->sync = 1;
                    break;

                case 'c':
                    search->class = 1;
                    break;

                case 'n':
                    search->name = 1;
                    break;

                default:
                    // TODO?
                    error = 1;
            }
        }

        /**
         * Set default option if neither --class nor --name was supplied
         */
        if (search->class == 0
            && search->name == 0) {
            search->class = 1;
            search->name = 1;
        }
    } else {
        error = 1;
    }

    optind++;
    if (error && optind >= argc) {
        // TODO error
        error = 1;
    }

    if (!error) {
        pattern = argv[optind];

        if (!error) {
            search->re_compiled = pcre_compile(pattern, 0, &pcre_err_str, &pcre_err_offset, NULL);

            if(search->re_compiled == NULL) {
                //wbk_logger_log(&logger, SEVERE, "Could not compile '%s': %s\n", pattern, pcre_err_str);
                // TODO error
                error = 1;
            }
        }

        if (!error) {
            search->re_extra = pcre_study(search->re_compiled, 0, &pcre_err_str);

            if (pcre_err_str) {
                //wbk_logger_log(&logger, SEVERE, "Could not compile '%s': %s\n", pattern, pcre_err_str);
                // TODO error
                error = 2;
            }
        }
    }

    if (error) {
        wdt_cmd_free((wdt_cmd_t *) search);
        search = NULL;
    }

    return search;
}

int
wdt_search_is_any(wdt_search_t *search)
{
    return search->wdt_search_is_any(search);
}

int
wdt_search_is_all(wdt_search_t *search)
{
    return search->wdt_search_is_all(search);
}

int
wdt_search_is_sync(wdt_search_t *search)
{
    return search->wdt_search_is_sync(search);
}

int
wdt_search_is_class(wdt_search_t *search)
{
    return search->wdt_search_is_class(search);
}

int
wdt_search_is_name(wdt_search_t *search)
{
    return search->wdt_search_is_name(search);
}

int
wdt_search_free_impl(wdt_cmd_t *cmd)
{
    wdt_search_t *search;

    search = (wdt_search_t *) cmd;

    if (search->re_compiled) {
        pcre_free(search->re_compiled);
        search->re_compiled = NULL;
    }

    if (search->re_extra) {
#ifdef PCRE_CONFIG_JIT
        pcre_free_study(search->re_extra);
#else
        pcre_free(search->re_extra);
#endif
        search->re_extra = NULL;
    }

    return search->super_wdt_search_free((wdt_cmd_t *) search);
}

int
wdt_search_exec_impl(wdt_cmd_t *cmd)
{
    wdt_search_t *search;

    search = (wdt_search_t *) cmd;

    EnumWindows(wdt_search_enum_windows, (LPARAM) search);

    return 0;
}

int
wdt_search_is_any_impl(wdt_search_t *search)
{
    return search->any;
}

int
wdt_search_is_all_impl(wdt_search_t *search)
{
    return search->all;
}

int
wdt_search_is_sync_impl(wdt_search_t *search)
{
    return search->sync;
}

int
wdt_search_is_class_impl(wdt_search_t *search)
{
    return search->class;
}

int
wdt_search_is_name_impl(wdt_search_t *search)
{
    return search->name;
}

BOOL CALLBACK
wdt_search_enum_windows(HWND window_handler, LPARAM param)
{
    wdt_search_t *search;
    int is_any;
    int is_all;
    int is_class;
    int is_name;
    char class[WDT_SEARCH_CLASS_BUFLEN];
    char name[WDT_SEARCH_NAME_BUFLEN];
    int pcre_rc;
    int class_applies;
    int name_applies;

    search = (wdt_search_t *) param;

    is_any = wdt_search_is_any(search);
    is_all = wdt_search_is_all(search);
    is_class = wdt_search_is_class(search);
    is_name = wdt_search_is_name(search);

    class_applies = 0;
    if (is_class) {
        GetClassName(window_handler, class, WDT_SEARCH_CLASS_BUFLEN);
        pcre_rc = pcre_exec(search->re_compiled, search->re_extra,
                            class, strlen(class),
                            0, 0, NULL, 0);
        if (pcre_rc >= 0) {
            class_applies = 1;
        }
    }

    name_applies = 0;
    if (is_name) {
        GetWindowText(window_handler, name, WDT_SEARCH_NAME_BUFLEN);
        pcre_rc = pcre_exec(search->re_compiled, search->re_extra,
                            name, strlen(name),
                            0, 0, NULL, 0);
        if (pcre_rc >= 0) {
            name_applies = 1;
        }
    }

    if ((is_any && ((is_class && class_applies) || (is_name && name_applies)))
        || (is_all && ((is_class && class_applies) && (is_name && name_applies)))) {
        fprintf(stdout, "%p\n", window_handler);
        fflush(stdout);
    }

    return TRUE;
}
