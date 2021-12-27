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

#include <stdio.h>
#include <string.h>

#include "cmd.h"
#include "search.h"
#include "windowactivate.h"

static int
print_help(void);

int
WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int error;
    LPWSTR *wargv;
    char **argv;
    int argc;
    int i;
    size_t size;
    int executed_any;
    wdt_cmd_t *search;
    wdt_cmd_t* windowactivate;

    executed_any = 0;

    wargv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (wargv) {
        argv = malloc(sizeof(char *) * argc);
        for (i = 0; i < argc; i++) {
            size = sizeof(char) * (sizeof(wchar_t) * wcslen(wargv[i]) + 1);
            argv[i] = malloc(size);
            memset(argv[i], 0, size);
            wcstombs(argv[i], wargv[i], size);
        }

        search = (wdt_cmd_t *) wdt_search_new(argc, argv);
        if (search) {
            executed_any = 1;
            error = wdt_cmd_exec(search);
        }

        windowactivate = (wdt_cmd_t *) wdt_windowactivate_new(argc, argv);
        if (windowactivate) {
            executed_any = 1;
            error = wdt_cmd_exec(windowactivate);
        }

        if (!executed_any) {
            error = 127;
            print_help();
        }

        if (search) {
            wdt_cmd_free(search);
        }

        if (windowactivate) {
            wdt_cmd_free(windowactivate);
        }

        free(argv);
        LocalFree(wargv);
    }

    return error;
}

int
print_help(void)
{
    fprintf(stderr, "Wrong command!");
    return 0;
}
